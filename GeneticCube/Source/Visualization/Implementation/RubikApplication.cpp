//
// Created by Георгий Круглов on 17.05.2021.
//

#include "../RubikApplication.hpp"

#include <filesystem>

using namespace Visualization;

void RubikApplication::Setup() {
#ifdef __DEBUG__
    engineParameters_[Urho3D::EP_FULL_SCREEN] = false;
    engineParameters_[Urho3D::EP_WINDOW_RESIZABLE] = true;
#else
    engineParameters_[Urho3D::EP_FULL_SCREEN] = false;
#endif

    engineParameters_[Urho3D::EP_WINDOW_WIDTH] = 1280;
    engineParameters_[Urho3D::EP_WINDOW_HEIGHT] = 720;
    engineParameters_[Urho3D::EP_RESOURCE_PREFIX_PATHS] = URHO_PREFIX;

    engine_->SetMaxFps(60);
}

void RubikApplication::Start() {
    auto *cache = GetSubsystem<Urho3D::ResourceCache>();
    GetSubsystem<Urho3D::UI>()->GetRoot()->SetDefaultStyle(cache->GetResource<Urho3D::XMLFile>("UI/DefaultStyle.xml"));

    scene_ = new Urho3D::Scene(context_);
    scene_->CreateComponent<Urho3D::Octree>();
    scene_->CreateComponent<Urho3D::DebugRenderer>();

    /** Models **/

    Urho3D::Node *skyNode = scene_->CreateChild("Sky");
    skyNode->SetScale(500.0f);
    auto *skybox = skyNode->CreateComponent<Urho3D::Skybox>();
    skybox->SetModel(cache->GetResource<Urho3D::Model>("Models/Box.mdl"));
    skybox->SetMaterial(cache->GetResource<Urho3D::Material>("Materials/Skybox.xml"));

    cameraNode_ = scene_->CreateChild("Camera");
    auto *camera = cameraNode_->CreateComponent<Urho3D::Camera>();
    camera->SetFarClip(2000);

    animator_ = new CubeAnimator(new Visualization::Cube(scene_, cache), engine_->GetMaxFps());

    cameraNode_->SetPosition(animator_->Center());
    cameraNode_->Translate({0, 0, 10});
    cameraNode_->RotateAround(animator_->Center(), {135, {0, 1, 0}}, Urho3D::TS_WORLD);
    cameraNode_->RotateAround(animator_->Center(), {30, {1, 0, 0}}, Urho3D::TS_WORLD);

    defaultCameraDirection = (cameraNode_->GetPosition() - animator_->Center()).Normalized();

    /** UI **/
    auto *randomizeButton = new Urho3D::Button(context_);
    GetSubsystem<Urho3D::UI>()->GetRoot()->AddChild(randomizeButton);
    randomizeButton->SetName("RandomizeButton");
    randomizeButton->SetStyle("Button");
    randomizeButton->SetSize(200, 100);
    randomizeButton->SetPosition(70, 60);
    SubscribeToEvent(randomizeButton, Urho3D::E_RELEASED, URHO3D_HANDLER(RubikApplication, HandleRandomize));

    auto *randomizeText = new Urho3D::Text(context_);
    randomizeButton->AddChild(randomizeText);
    randomizeText->SetName("RandomizeText");
    randomizeText->SetText("RANDOMIZE");
    randomizeText->SetFont(cache->GetResource<Urho3D::Font>("Fonts/ALSSchlangesans-Bold.ttf"), 25);
    randomizeText->SetColor({1, 1, 1});
    randomizeText->SetAlignment(Urho3D::HA_CENTER, Urho3D::VA_CENTER);

    auto *solveButton = new Urho3D::Button(context_);
    GetSubsystem<Urho3D::UI>()->GetRoot()->AddChild(solveButton);
    solveButton->SetName("RandomizeButton");
    solveButton->SetStyle("Button");
    solveButton->SetSize(200, 100);
    solveButton->SetPosition(70, 170);
    SubscribeToEvent(solveButton, Urho3D::E_RELEASED, URHO3D_HANDLER(RubikApplication, HandleSolve));

    auto *solveText = new Urho3D::Text(context_);
    solveButton->AddChild(solveText);
    solveText->SetName("RandomizeText");
    solveText->SetText("SOLVE");
    solveText->SetFont(cache->GetResource<Urho3D::Font>("Fonts/ALSSchlangesans-Bold.ttf"), 25);
    solveText->SetColor({1, 1, 1});
    solveText->SetAlignment(Urho3D::HA_CENTER, Urho3D::VA_CENTER);

    {
        Urho3D::Node *lightNode = scene_->CreateChild();
        lightNode->SetDirection(Urho3D::Vector3::FORWARD);
        lightNode->Yaw(50);     // horizontal
        lightNode->Pitch(10);   // vertical
        auto *light = lightNode->CreateComponent<Urho3D::Light>();
        light->SetLightType(Urho3D::LIGHT_DIRECTIONAL);
        light->SetBrightness(1.6);
        light->SetColor(Urho3D::Color(1.0, .6, 0.3, 1));
        light->SetCastShadows(true);
    }
    // Create a blue point light
    {
        auto *lightNode = scene_->CreateChild("Light");
        lightNode->SetPosition(Urho3D::Vector3(-10, 2, 5));
        auto *light = lightNode->CreateComponent<Urho3D::Light>();
        light->SetLightType(Urho3D::LIGHT_POINT);
        light->SetRange(25);
        light->SetBrightness(1.7);
        light->SetColor(Urho3D::Color(0.5, .5, 1.0, 1));
        light->SetCastShadows(true);
    }

    auto *renderer = GetSubsystem<Urho3D::Renderer>();
    Urho3D::SharedPtr<Urho3D::Viewport> viewport(
            new Urho3D::Viewport(context_, scene_, cameraNode_->GetComponent<Urho3D::Camera>()));
    renderer->SetViewport(0, viewport);

    SubscribeToEvent(Urho3D::E_BEGINFRAME, URHO3D_HANDLER(RubikApplication, HandleBeginFrame));
    SubscribeToEvent(Urho3D::E_KEYDOWN, URHO3D_HANDLER(RubikApplication, HandleKeyDown));
    SubscribeToEvent(Urho3D::E_UPDATE, URHO3D_HANDLER(RubikApplication, HandleUpdate));
}

void RubikApplication::HandleKeyDown(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    using namespace Urho3D;
    int key = eventData[KeyDown::P_KEY].GetInt();
    if (key == KEY_ESCAPE)
        engine_->Exit();

    auto *input = GetSubsystem<Input>();
    bool shift = input->GetKeyDown(Urho3D::KEY_SHIFT);

    switch (key) {
        case KEY_TAB:
            input->SetMouseVisible(!GetSubsystem<Input>()->IsMouseVisible());
            break;

        case KEY_RIGHT:
            if (animator_->IsHorizontal())
                animator_->AddRotation(
                        shift ? RotationConfiguration::CounterHalfTurn : RotationConfiguration::CounterQuarterTurn);
            else
                animator_->TryMovePosition(1);
            break;

        case KEY_LEFT:
            if (animator_->IsHorizontal())
                animator_->AddRotation(shift ? RotationConfiguration::HalfTurn : RotationConfiguration::QuarterTurn);
            else
                animator_->TryMovePosition(-1);
            break;

        case KEY_UP:
            if (animator_->IsHorizontal())
                animator_->TryMovePosition(1);
            else
                animator_->AddRotation(shift ? RotationConfiguration::HalfTurn : RotationConfiguration::QuarterTurn);
            break;

        case KEY_DOWN:
            if (animator_->IsHorizontal())
                animator_->TryMovePosition(-1);
            else
                animator_->AddRotation(
                        shift ? RotationConfiguration::CounterHalfTurn : RotationConfiguration::CounterQuarterTurn);
            break;

        case KEY_RSHIFT:
            animator_->TryFlipDimension();
            break;

        case KEY_F:
            cameraNode_->SetPosition(
                    animator_->Center() +
                    defaultCameraDirection * (animator_->Center() - cameraNode_->GetPosition()).Length());
            break;

        case '/':
            animator_->TryFlipOrientation();
            break;
    }
}

void RubikApplication::HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    float timeStep = eventData[Urho3D::Update::P_TIMESTEP].GetFloat();
    float MOVE_SPEED = 10.0f;
    float MOUSE_SENSITIVITY = 0.1f;

    auto *input = GetSubsystem<Urho3D::Input>();
    if (input->GetKeyDown(Urho3D::KEY_SHIFT))
        MOUSE_SENSITIVITY *= 10;

    if (!input->IsMouseVisible()) {
        Urho3D::IntVector2 mouseMove = input->GetMouseMove();
        float yaw_ = MOUSE_SENSITIVITY * (float) mouseMove.x_;
        float pitch_ = MOUSE_SENSITIVITY * (float) mouseMove.y_;
        float scroll = MOUSE_SENSITIVITY * (float) input->GetMouseMoveWheel();


        if (yaw_ != 0 && scroll == 0) {
            Urho3D::Quaternion yawRotation = {yaw_, {0, 1, 0}};
            cameraNode_->RotateAround(animator_->Center(), yawRotation, Urho3D::TS_WORLD);
        }

        if (pitch_ != 0 && scroll == 0) {
            Urho3D::Quaternion pitchRotation = {pitch_, {1, 0, 0}};
            cameraNode_->RotateAround(animator_->Center(), -pitchRotation, Urho3D::TS_WORLD);
        }

        if (scroll != 0) {
            auto translation = (animator_->Center() - cameraNode_->GetPosition()).Normalized() * scroll;
            float distance = (animator_->Center() - cameraNode_->GetPosition() - translation).Length();

            float direction = (animator_->Center() - cameraNode_->GetPosition()).DotProduct(translation);

            if (distance >= minDistance || direction > 0)
                cameraNode_->Translate(translation);
        }

        cameraNode_->LookAt(animator_->Center());
    }
}

void RubikApplication::HandleRandomize(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    Logic::Cube c(true);
    animator_->AddMoves(c.History());
}

void RubikApplication::HandleSolve(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    animator_->Solve();
}
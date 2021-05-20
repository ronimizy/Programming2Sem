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
    engineParameters_[Urho3D::EP_RESOURCE_PREFIX_PATHS] = "/Users/george/Documents/Programming2Sem/GeneticCube/Urho3D/bin/";

    engine_->SetMaxFps(60);
}

void RubikApplication::Start() {
    auto *cache = GetSubsystem<Urho3D::ResourceCache>();
    GetSubsystem<Urho3D::UI>()->GetRoot()->SetDefaultStyle(cache->GetResource<Urho3D::XMLFile>("UI/DefaultStyle.xml"));

    scene_ = new Urho3D::Scene(context_);
    scene_->CreateComponent<Urho3D::Octree>();
    scene_->CreateComponent<Urho3D::DebugRenderer>();

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
    cameraNode_->RotateAround(animator_->Center(), {-15, {0, 1, 0}}, Urho3D::TS_WORLD);
    cameraNode_->RotateAround(animator_->Center(), {-20, {1, 0, 0}}, Urho3D::TS_WORLD);

    defaultCameraDirection = (cameraNode_->GetPosition() - animator_->Center()).Normalized();

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

    static std::vector<Moves> moves = {U, UR, U2, U2,
                                       F, FR, F2, F2,
                                       D, DR, D2, D2,
                                       B, BR, B2, B2,
                                       L, LR, L2, L2,
                                       R, RR, R2, R2,
                                       M, MR, M2, M2,
                                       E, ER, E2, E2,
                                       S, SR, S2, S2};

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
                    defaultCameraDirection * (animator_->Center() - cameraNode_->GetPosition()).Length());
            break;

        case KEY_H:
            animator_->AddMoves(movesFromString(
                    "D R2 E D S2 U' B' U D' D' E' R' L' B2 M R' S2 D B' D2 B F2 R' L' E F' U' S' B2 R S F' U2 U' D' D M2 U L' S F2 U E2 D E' E2 D' R2 L' R L2 L L2 U2 D2 U2 R2 E2 E L2 U2 E2 M2 M' M' F2 S' M2 U2 B2 R2 B F U' F' M U' M' F2 B R' R2 F2 R B D2 B' B' F2 R2 F2 E' E L2 L' B2 E B' S' U2 E2 L R2 S2 F E' F L2 S' B F2 D2 L2 F L2 R2 M' U' E2 R2 D R2 E D S2 U' B' U D' D' E' R' L' B2 M R' S2 D B' D2 B F2 R' L' E F' U' S' B2 R S F' U2 U' D' D M2 U L' S F2 U E2 D E' E2 D' R2 L' R L2 L L2 U2 D2 U2 R2 E2 E L2 U2 E2 M2 M' M' F2 S' M2 U2 B2 R2 B F U' F' M U' M' F2 B R' R2 F2 R B D2 B' B' F2 R2 F2 E' E L2 L' B2 E B' S' U2 E2 L R2 S2 F E' F L2 S' B F2 D2 L2 F L2 R2 M' U' E2 R2"));
            break;

        case KEY_U:
            animator_->AddMove(moves.front());
            moves.erase(moves.begin());
            break;

        case KEY_Y:
            animator_->Solve();
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
            cameraNode_->RotateAround(animator_->Center(), pitchRotation, Urho3D::TS_WORLD);
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
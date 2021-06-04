//
// Created by Георгий Круглов on 17.05.2021.
//

#include "../RubikApplication.hpp"
#include <sstream>
#include <Urho3D/UI/MessageBox.h>

using namespace Visualization;

void RubikApplication::Setup() {
#ifdef __DEBUG__
    engineParameters_[Urho3D::EP_FULL_SCREEN] = false;
    engineParameters_[Urho3D::EP_WINDOW_RESIZABLE] = true;
#else
    engineParameters_[Urho3D::EP_FULL_SCREEN] = false;
#endif
    engineParameters_[Urho3D::EP_WINDOW_RESIZABLE] = true;
    engineParameters_[Urho3D::EP_WINDOW_WIDTH] = 1920;
    engineParameters_[Urho3D::EP_WINDOW_HEIGHT] = 1080;
    engineParameters_[Urho3D::EP_RESOURCE_PREFIX_PATHS] = URHO_PREFIX;
    engineParameters_[Urho3D::EP_WINDOW_TITLE] = "Rubik's Cube";

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
    cameraNode_->Translate({0, 0, minDistance});
    cameraNode_->RotateAround(animator_->Center(), {135, {0, 1, 0}}, Urho3D::TS_WORLD);
    cameraNode_->RotateAround(animator_->Center(), {30, {1, 0, 0}}, Urho3D::TS_WORLD);

    defaultCameraDirection = (cameraNode_->GetPosition() - animator_->Center()).Normalized();

    /** UI **/
    {
        auto *randomizeButton = new Urho3D::Button(context_);
        GetSubsystem<Urho3D::UI>()->GetRoot()->AddChild(randomizeButton);
        randomizeButton->SetName("RandomizeButton");
        randomizeButton->SetStyle("Button");
        randomizeButton->SetSize(250, 100);
        randomizeButton->SetPosition(70, 60);
        SubscribeToEvent(randomizeButton, Urho3D::E_RELEASED, URHO3D_HANDLER(RubikApplication, HandleRandomize));

        auto *randomizeText = new Urho3D::Text(context_);
        randomizeButton->AddChild(randomizeText);
        randomizeText->SetName("RandomizeText");
        randomizeText->SetText("RANDOMIZE");
        randomizeText->SetFont(cache->GetResource<Urho3D::Font>("Fonts/Arial.ttf"), 25);
        randomizeText->SetColor({1, 1, 1});
        randomizeText->SetAlignment(Urho3D::HA_CENTER, Urho3D::VA_CENTER);


        auto *solveButton = new Urho3D::Button(context_);
        randomizeButton->AddChild(solveButton);
        solveButton->SetName("RandomizeButton");
        solveButton->SetStyle("Button");
        solveButton->SetSize(250, 100);
        solveButton->SetPosition(0, 130);
        SubscribeToEvent(solveButton, Urho3D::E_RELEASED, URHO3D_HANDLER(RubikApplication, HandleSolve));

        auto *solveText = new Urho3D::Text(context_);
        solveButton->AddChild(solveText);
        solveText->SetName("RandomizeText");
        solveText->SetText("SOLVE");
        solveText->SetFont(cache->GetResource<Urho3D::Font>("Fonts/Arial.ttf"), 25);
        solveText->SetColor({1, 1, 1});
        solveText->SetAlignment(Urho3D::HA_CENTER, Urho3D::VA_CENTER);

        auto *clearButton = new Urho3D::Button(context_);
        solveButton->AddChild(clearButton);
        clearButton->SetName("ClearButton");
        clearButton->SetStyle("Button");
        clearButton->SetSize(250, 100);
        clearButton->SetPosition(0, 130);
        SubscribeToEvent(clearButton, Urho3D::E_RELEASED, URHO3D_HANDLER(RubikApplication, HandleClear));

        auto *clearText = new Urho3D::Text(context_);
        clearButton->AddChild(clearText);
        clearText->SetName("ClearText");
        clearText->SetText("CLEAR");
        clearText->SetFont(cache->GetResource<Urho3D::Font>("Fonts/Arial.ttf"), 25);
        clearText->SetColor({1, 1, 1});
        clearText->SetAlignment(Urho3D::HA_CENTER, Urho3D::VA_CENTER);

        saveButton_ = new Urho3D::Button(context_);
        clearButton->AddChild(saveButton_);
        saveButton_->SetName("SaveButton");
        saveButton_->SetStyle("Button");
        saveButton_->SetSize(250, 100);
        saveButton_->SetPosition(0, 130);
        SubscribeToEvent(saveButton_, Urho3D::E_RELEASED, URHO3D_HANDLER(RubikApplication, HandleSave));

        auto *saveText = new Urho3D::Text(context_);
        saveButton_->AddChild(saveText);
        saveText->SetName("SaveText");
        saveText->SetText("SAVE");
        saveText->SetFont(cache->GetResource<Urho3D::Font>("Fonts/Arial.ttf"), 25);
        saveText->SetColor({1, 1, 1});
        saveText->SetAlignment(Urho3D::HA_CENTER, Urho3D::VA_CENTER);
    }

    {
        auto *quitButton = new Urho3D::Button(context_);
        GetSubsystem<Urho3D::UI>()->GetRoot()->AddChild(quitButton);
        quitButton->SetName("QuitButton");
        quitButton->SetStyle("Button");
        quitButton->SetSize(250, 100);
        quitButton->SetAlignment(Urho3D::HA_LEFT, Urho3D::VA_BOTTOM);
        quitButton->SetPosition(70, -60);
        SubscribeToEvent(quitButton, Urho3D::E_RELEASED, URHO3D_HANDLER(RubikApplication, HandleQuit));

        auto *quitText = new Urho3D::Text(context_);
        quitButton->AddChild(quitText);
        quitText->SetName("QuitText");
        quitText->SetText("QUIT");
        quitText->SetFont(cache->GetResource<Urho3D::Font>("Fonts/Arial.ttf"), 25);
        quitText->SetColor({1, 1, 1});
        quitText->SetAlignment(Urho3D::HA_CENTER, Urho3D::VA_CENTER);
    }

    {
        durationSlider_ = new Urho3D::Slider(context_);
        GetSubsystem<Urho3D::UI>()->GetRoot()->AddChild(durationSlider_);
        durationSlider_->SetName("DurationSlider");
        durationSlider_->SetAlignment(Urho3D::HA_CENTER, Urho3D::VA_BOTTOM);
        durationSlider_->SetPosition(0, -30);
        durationSlider_->SetRange(0.8);
        durationSlider_->SetStyleAuto();
        durationSlider_->SetSize(1000, 100);

        auto *text = new Urho3D::Text(context_);
        durationSlider_->AddChild(text);
        text->SetName("DurationText");
        text->SetText("Animation duration");
        text->SetFont(cache->GetResource<Urho3D::Font>("Fonts/Arial.ttf"), 25);
        text->SetColor({0, 0, 0});
        text->SetAlignment(Urho3D::HA_CENTER, Urho3D::VA_TOP);
        text->SetPosition(0, -50);
    }

    {
        helpText_ = new Urho3D::Text(context_);
        GetSubsystem<Urho3D::UI>()->GetRoot()->AddChild(helpText_);
        helpText_->SetName("HelpText");
        helpText_->SetText(
                ""
                "Toggle mouse\n"
                "Switch Horizontal/Vertical selection                              \n"
                "Switch Front/Side selection\n"
                "Rotate/Move\n"
                "Reset camera angle\n"
                "Toggle help\n"
                "Zoom In/Out\n\n"
                "To select configuration file, drop it onto the window.\n"
                "You will be prompted to load or save current configuration from/to it.\n"
                "If you just want for it to be selected and save later, just close the alert\n"
                "the save button will be enabled after that");
        helpText_->SetFont(cache->GetResource<Urho3D::Font>("Fonts/Arial.ttf"), 25);
        helpText_->SetColor({0, 0, 0});
        helpText_->SetAlignment(Urho3D::HA_RIGHT, Urho3D::VA_TOP);
        helpText_->SetPosition({-40, 40});

        auto *keys = new Urho3D::Text(context_);
        helpText_->AddChild(keys);
        keys->SetName("HelpTextKeys");
        keys->SetText("[TAB]\n[RSHIFT]\n[/]\n[←↓→↑]\n[F]\n[H]\n[⇅]");
        keys->SetFont(cache->GetResource<Urho3D::Font>("Fonts/Arial.ttf"), 25);
        keys->SetColor({0, 0, 0});
        keys->SetAlignment(Urho3D::HA_RIGHT, Urho3D::VA_TOP);
    }

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
    {
        Urho3D::Node *lightNode = scene_->CreateChild("Light");
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
    SubscribeToEvent(Urho3D::E_DROPFILE, URHO3D_HANDLER(RubikApplication, HandleFileDrop));
}

void RubikApplication::HandleKeyDown(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    using namespace Urho3D;
    int key = eventData[KeyDown::P_KEY].GetInt();
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

        case KEY_H:
            helpText_->SetVisible(!helpText_->IsVisible());
            break;
    }
}

void RubikApplication::HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    float timeStep = eventData[Urho3D::Update::P_TIMESTEP].GetFloat();
    float MOVE_SPEED = 10.0f;
    float MOUSE_SENSITIVITY = 0.1f;

    int value = (int) (10 * durationSlider_->GetValue());
    if (value % 2 == 1)
        ++value;
    durationSlider_->SetValue(((float) value) / 10);

    animator_->SetDuration(durationSlider_->GetValue());

    saveButton_->SetEnabled(!filePath.empty());

    auto *input = GetSubsystem<Urho3D::Input>();
    if (input->GetKeyDown(Urho3D::KEY_SHIFT))
        MOUSE_SENSITIVITY *= 10;

    if (!input->IsMouseVisible()) {
        Urho3D::IntVector2 mouseMove = input->GetMouseMove();
        float yaw_ = MOUSE_SENSITIVITY * (float) mouseMove.x_;
        float pitch_ = MOUSE_SENSITIVITY * (float) mouseMove.y_;
        float scroll = MOUSE_SENSITIVITY * (float) input->GetMouseMoveWheel();


        if (abs(yaw_) > 0.15 && scroll == 0) {
            Urho3D::Quaternion yawRotation = {yaw_, {0, 1, 0}};
            cameraNode_->RotateAround(animator_->Center(), yawRotation, Urho3D::TS_WORLD);
        }

        if (abs(pitch_) > 0.15 && scroll == 0) {
            Urho3D::Quaternion pitchRotation = {pitch_, {1, 0, 0}};
            cameraNode_->RotateAround(animator_->Center(), -pitchRotation, Urho3D::TS_WORLD);
        }

        if (scroll != 0) {
            auto translation = (animator_->Center() - cameraNode_->GetPosition()).Normalized() * scroll;
            float distance = (animator_->Center() - cameraNode_->GetPosition() - translation).Length();

            float direction = (animator_->Center() - cameraNode_->GetPosition()).DotProduct(translation);

            if (distance >= minDistance || direction < 0)
                cameraNode_->Translate(translation);
        }

        cameraNode_->LookAt(animator_->Center());
    }
}

void RubikApplication::HandleRandomize(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    Logic::CubeIndividual c(true);
    animator_->AddMoves(c.History());
}

void RubikApplication::HandleSolve(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    animator_->Solve();
}

void RubikApplication::HandleQuit(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    engine_->Exit();
}

void RubikApplication::HandleFileDrop(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    auto *messageBox = new Urho3D::MessageBox(context_, "Do you want to load state from file, or save to it?",
                                              "Choose the file action");
    auto *window = messageBox->GetWindow();
    window->SetSize(400 * 3, 83 * 3);
    auto *title = window->GetChildDynamicCast<Urho3D::Text>("TitleText", true);
    title->SetFont(GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::Font>("Fonts/Arial.ttf"), 25);

    auto *message = window->GetChildDynamicCast<Urho3D::Text>("MessageText", true);
    message->SetFont(GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::Font>("Fonts/Arial.ttf"), 25);
//
//    auto titlePanel = title->GetParent();
//    titlePanel->SetMaxSize(2147483647, 30);
//
//    auto messagePanel = title->GetParent();
//    messagePanel->SetMaxSize(2147483647, 30);


    auto *saveButton = window->GetChildDynamicCast<Urho3D::Button>("OkButton", true);
    auto *loadButton = window->GetChildDynamicCast<Urho3D::Button>("CancelButton", true);
    auto *closeButton = window->GetChildDynamicCast<Urho3D::Button>("CloseButton", true);

    auto *buttonPanel = loadButton->GetParent();
    buttonPanel->SetMaxSize(window->GetSize().x_ - 80, 100);

    ((Urho3D::Text *) saveButton->GetChildren().Front().Get())->SetText("Save");
    ((Urho3D::Text *) saveButton->GetChildren().Front().Get())->SetFont(
            GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::Font>("Fonts/Arial.ttf"), 25);
    saveButton->SetMaxSize(100, 50);
    saveButton->SetFocus(false);

    ((Urho3D::Text *) loadButton->GetChildren().Front().Get())->SetText("Load");
    ((Urho3D::Text *) loadButton->GetChildren().Front().Get())->SetFont(
            GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::Font>("Fonts/Arial.ttf"), 25);
    loadButton->SetMaxSize(100, 50);
    loadButton->SetVisible(true);
    loadButton->SetPosition(loadButton->GetPosition().x_ - 60, loadButton->GetPosition().y_);


    filePath = eventData[Urho3D::DropFile::P_FILENAME].ToString().CString();

    SubscribeToEvent(saveButton, Urho3D::E_PRESSED, URHO3D_HANDLER(RubikApplication, HandleSave));
    SubscribeToEvent(loadButton, Urho3D::E_PRESSED, URHO3D_HANDLER(RubikApplication, HandleLoad));
    SubscribeToEvent(closeButton, Urho3D::E_PRESSED, URHO3D_HANDLER(RubikApplication, HandleClearFilePath));
}

void RubikApplication::HandleClear(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    animator_->SetUnwrap(Logic::CubeIndividual {});
}

void RubikApplication::HandleSave(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    std::fstream file(filePath, std::ios::out);

    file << animator_->GetCubeString();
}

void RubikApplication::HandleLoad(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    std::fstream file(filePath, std::ios::in);
    std::stringstream stream;

    while (!file.eof()) {
        std::string s;
        file >> s;
        stream << s;
    }

    try {
        animator_->SetUnwrap(Logic::CubeIndividual {stream.str().c_str()});
    } catch (std::invalid_argument &exception) {
        auto *messageBox = new Urho3D::MessageBox(context_, exception.what(), "Failed to import cube");
        auto *window = messageBox->GetWindow();
        window->SetSize(400 * 3, 83 * 3);
        window->GetChildDynamicCast<Urho3D::Text>("TitleText", true)->SetFont(
                GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::Font>("Fonts/Arial.ttf"), 25);
        window->GetChildDynamicCast<Urho3D::Text>("MessageText", true)->SetFont(
                GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::Font>("Fonts/Arial.ttf"), 25);
    }
}

void RubikApplication::HandleClearFilePath(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    filePath = "";
}
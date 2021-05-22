//
// Created by Георгий Круглов on 17.05.2021.
//

#ifndef GENETICCUBE_RUBIKAPPLICATION_HPP
#define GENETICCUBE_RUBIKAPPLICATION_HPP

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Skybox.h>

#include "Cube.hpp"
#include "CubeAnimator.hpp"

namespace Visualization {
    class RubikApplication : public Urho3D::Application {
        Urho3D::SharedPtr<Urho3D::Scene> scene_;
        Urho3D::SharedPtr<Urho3D::Node> cameraNode_;

        CubeAnimator *animator_;

        const float minDistance = 10.0f;
        Urho3D::Vector3 defaultCameraDirection;

    public:
        RubikApplication(Urho3D::Context *context)
                : Urho3D::Application(context) {};

        void Setup() override;

        void Start() override;

        void HandleKeyDown(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

        void HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

        inline void
        HandleBeginFrame(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) { animator_->RenderFrame(); }

        void HandleRandomize(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

        void HandleSolve(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

        ~RubikApplication() {
            delete animator_;
        }
    };
}


#endif //GENETICCUBE_RUBIKAPPLICATION_HPP

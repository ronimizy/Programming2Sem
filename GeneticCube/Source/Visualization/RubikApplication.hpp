//
// Created by Георгий Круглов on 17.05.2021.
//

#ifndef GENETICCUBE_RUBIKAPPLICATION_HPP
#define GENETICCUBE_RUBIKAPPLICATION_HPP

#include "Urho3DAll.hpp"
#include <Urho3D/UI/Slider.h>

#include "Cube.hpp"
#include "CubeAnimator.hpp"

namespace Visualization {
    class RubikApplication : public Urho3D::Application {
        Urho3D::SharedPtr<Urho3D::Scene> scene_;
        Urho3D::SharedPtr<Urho3D::Node> cameraNode_;
        Urho3D::SharedPtr<Urho3D::Slider> durationSlider_;
        Urho3D::SharedPtr<Urho3D::Text> helpText_;

        CubeAnimator *animator_;

        const float minDistance = 20.0f;
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

        void HandleQuit(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

        void HandleFile(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

        void HandleClear(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

        ~RubikApplication() {
            delete animator_;
        }
    };
}


#endif //GENETICCUBE_RUBIKAPPLICATION_HPP

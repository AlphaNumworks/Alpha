#ifndef SPRITES_APP_H
    #define SPRITES_APP_H
    #include "apps/shared/input_event_handler_delegate_app.h"
    #include "apps/shared/shared_app.h"
    #include <escher.h>

namespace Sprites
{

class App : public Shared::InputEventHandlerDelegateApp
{

    public:
        class Descriptor : public Shared::InputEventHandlerDelegateApp::Descriptor
        {
            public:
                I18n::Message                     name() override;
                I18n::Message                     upperName() override;
                App::Descriptor::ExaminationLevel examinationLevel() override;
                const Image *                     icon() override;
        };

        class Snapshot : public SharedApp::Snapshot {
            public:
                Snapshot();
                App *        unpack(Container * container) override;
                Descriptor * descriptor() override;
        };

        static App * app() {
            return static_cast<App *>(Container::activeApp());
        }

        ~App();
        TELEMETRY_ID("Sprites");

        bool prepareForExit() override;

        bool handleEvent(Ion::Events::Event event) override;
        void willExitResponderChain(Responder * nextFirstResponder) override;

    private:
        App(Snapshot * snapshot);
};

}

#endif /* SPRITES_APP_H */

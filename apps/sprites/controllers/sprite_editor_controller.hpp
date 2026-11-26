#ifndef SPRITES_SPRITE_EDITOR_CONTROLLER_H
    #define SPRITES_SPRITE_EDITOR_CONTROLLER_H
    #include "apps/sprites/sprite/sprite.h"
#include "apps/sprites/views/CanvaView.hpp"
#include "escher/view.h"
#include "escher/view_controller.h"

namespace Sprites
{

    class SpriteEditorController : public ViewController
    {
        public:
            // NOTE: 16x16
            static constexpr int k_colorBufferSize = 256;

            SpriteEditorController();

            void setSprite(Sprite sprite);

            View *       view() override { return &m_view; };
            const char * title() override;
            bool         handleEvent(Ion::Events::Event evt) override;

            void save(void);

        private:
            CanvaView m_view;
            Sprite    m_sprite;

            uint8_t   m_actualColor{0};

            uint8_t   m_buff[k_colorBufferSize];

    };

}

#endif /* SPRITES_SPRITE_EDITOR_CONTROLLER_H */

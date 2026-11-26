#include "sprite_editor_controller.hpp"
#include <cstring>

namespace Sprites
{

    SpriteEditorController::SpriteEditorController(
        // None
    ) : ViewController(nullptr),
        m_view(m_buff)
    {
        // Init goes brr
    }

    void SpriteEditorController::setSprite(Sprite sprite)
    {
        assert(!sprite.isNull());
        m_sprite = sprite;
        Sprite::Data d = sprite.value();
        memcpy(m_buff, d.buffer, d.size);
    }

    const char * SpriteEditorController::title()
    {
        return m_sprite.fullName();
    }

    bool SpriteEditorController::handleEvent(Ion::Events::Event evt)
    {
        if (evt == Ion::Events::Back) {
            save();
            return false;
        }
        if (evt == Ion::Events::Sto) {
            save();
            return true;
        }
        if (evt == Ion::Events::OK || evt == Ion::Events::EXE) {
            m_buff[m_view.cursorY() * CanvaView::k_pixelSize + m_view.cursorX()] = m_actualColor;
            m_view.reloadOnCursor();
            return true;
        }
        if (evt == Ion::Events::Down) {
            m_view.moveCursorByOffset(0, 1);
            return true;
        } else if (evt == Ion::Events::Up) {
            m_view.moveCursorByOffset(0, -1);
            return true;
        }
        if (evt == Ion::Events::Left) {
            m_view.moveCursorByOffset(-1, 0);
            return true;
        } else if (evt == Ion::Events::Right) {
            m_view.moveCursorByOffset(1, 0);
            return true;
        }
        return false;
    }

    void SpriteEditorController::save(void)
    {
        assert(!m_sprite.isNull());
        Sprite::Data d = m_sprite.value();
        memcpy((char *)d.buffer, m_buff, d.size);
    }

}

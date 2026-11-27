#include "sprite_name_cell.h"
#include "app.h"
#include <assert.h>

namespace Sprites {

    void SpriteNameCell::setEven(bool even)
    {
        EvenOddCell::setEven(even);
        m_textField.setBackgroundColor(backgroundColor());
    }

    void SpriteNameCell::setHighlighted(bool highlight)
    {
        EvenOddCell::setHighlighted(highlight);
        m_textField.setBackgroundColor(backgroundColor());
    }

    const char * SpriteNameCell::text() const
    {
        if (!m_textField.isEditing()) {
            return m_textField.text();
        }
        return nullptr;
    }

    KDSize SpriteNameCell::minimalSizeForOptimalDisplay() const
    {
        return m_textField.minimalSizeForOptimalDisplay();
    }

    void SpriteNameCell::didBecomeFirstResponder() 
    {
        Container::activeApp()->setFirstResponder(&m_textField);
    }

    void SpriteNameCell::layoutSubviews(bool force)
    {
        KDRect cellBounds = bounds();
        m_textField.setFrame(KDRect(cellBounds.x() + k_leftMargin,
                cellBounds.y(),
                cellBounds.width() - k_leftMargin,
                cellBounds.height()),
            force);
    }

}

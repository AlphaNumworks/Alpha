#include "CanvaView.hpp"
#include "kandinsky/color.h"
#include "kandinsky/rect.h"

namespace Sprites
{

    CanvaView::CanvaView(const uint8_t * pxs) : m_buff(pxs)
    {
        // Aha
    }

    void CanvaView::drawRect(KDContext * ctx, KDRect rect) const
    {
        ctx->fillRect(rect, KDColorWhite);
        for (size_t y = 0; y < k_pixelSize; ++y) {
            for (size_t x = 0; x < k_pixelSize; ++x) {
                auto cuint = m_buff[y * k_pixelSize + x];
                KDColor clr = KDColor::RGB888((cuint >> 6) * 85, ((cuint >> 4) & 3) * 85, ((cuint >> 2) & 3) * 85);
                ctx->fillRect(cellAt(x, y), clr);
            }
        }
        // NOTE: To only draw bodrders
        ctx->strokeRect(cellAt(m_x, m_y), KDColorBlack);
    }

    void CanvaView::moveCursorByOffset(int offsetX, int offsetY)
    {
        markRectAsDirty(cellAt(m_x, m_y));
        if (!(m_x + offsetX < 0 || m_x + offsetX >= k_pixelSize)) {
            m_x += offsetX;
        }
        if (!(m_y + offsetY < 0 || m_y + offsetY >= k_pixelSize)) {
            m_y += offsetY;
        }
        markRectAsDirty(cellAt(m_x, m_y));
    }

    KDRect CanvaView::cellAt(int x, int y) const
    {
        return KDRect(x * k_cellSize, y * k_cellSize, k_cellSize, k_cellSize);
    }

}

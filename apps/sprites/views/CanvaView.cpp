#include "CanvaView.hpp"
#include "kandinsky/color.h"
#include "kandinsky/rect.h"
#include <escher/palette.h>

namespace Sprites
{

    CanvaView::CanvaView(const uint8_t * pxs) : m_buff(pxs)
    {
        // Aha
    }

    void CanvaView::drawRect(KDContext * ctx, KDRect rect) const
    {
        ctx->fillRect(rect, Palette::GrayBright);
        for (size_t y = 0; y < k_pixelSize; ++y) {
            for (size_t x = 0; x < k_pixelSize; ++x) {
                uint8_t cuint = m_buff[y * k_pixelSize + x];
                KDRect r = cellAt(x, y);
                if ((cuint & 3) == 0) {
                    constexpr KDCoordinate h = k_cellSize / 2;
                    ctx->fillRect(r, KDColorWhite);
                    ctx->fillRect(KDRect(r.x(), r.y(), h, h), Palette::GrayBright);
                    ctx->fillRect(KDRect(r.x() + h, r.y() + h, h, h), Palette::GrayBright);
                } else {
                    ctx->fillRect(r, KDColor::RGB888((cuint >> 6) * 85, ((cuint >> 4) & 3) * 85, ((cuint >> 2) & 3) * 85));
                }
            }
        }

        // NOTE: To only draw bodrders
        auto r = cellAt(m_x, m_y);
        ctx->strokeRect(r, KDColorBlack);
        ctx->strokeRect(KDRect(r.left() + 1, r.top() + 1, r.width() - 2, r.height() - 2), KDColorWhite);
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

#include "CanvaView.hpp"
#include "kandinsky/color.h"
#include "kandinsky/rect.h"

namespace Sprites
{

    void CanvaView::drawRect(KDContext * ctx, KDRect rect) const
    {
        for (size_t y = 0; y < k_pixelSize; ++y) {
            for (size_t x = 0; x < k_pixelSize; ++x) {
                ctx->fillRect(cellAt(x, y), KDColorWhite);
            }
        }
        // NOTE: To only draw bodrders
        ctx->strokeRect(cellAt(m_x, m_y), KDColorBlack);
    }

    void CanvaView::moveCursorByOffset(int offsetX, int offsetY)
    {
        if (!(m_x + offsetX < 0 || m_x + offsetX >= k_pixelSize)) {
            m_x += offsetX;
        }
        if (!(m_y + offsetY < 0 || m_y + offsetY >= k_pixelSize)) {
            m_y += offsetY;
        }
    }

    KDRect CanvaView::cellAt(int x, int y) const
    {
        return KDRect(x * k_cellSize, y * k_cellSize, k_cellSize, k_cellSize);
    }

}

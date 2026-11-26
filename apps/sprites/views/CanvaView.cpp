#include "CanvaView.hpp"
#include "kandinsky/color.h"
#include "kandinsky/rect.h"
#include <escher/palette.h>

namespace Sprites
{

    constexpr uint8_t CanvaView::k_paletteOrder[64];

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
                    ctx->fillRect(r, toKDColor(cuint));
                }
            }
        }
        for (size_t y = 0; y < k_paletteNumberOfCells; ++y) {
            for (size_t x = 0; x < k_paletteNumberOfCells; ++x) {
                ctx->fillRect(paletteAt(x, y), toKDColor((k_paletteOrder[y * k_paletteNumberOfCells + x] << 2) | 3));
            }
        }

        // NOTE: To only draw bodrders
        auto r = cellAt(m_x, m_y);
        ctx->strokeRect(r, KDColorBlack);
        ctx->strokeRect(KDRect(r.left() + 1, r.top() + 1, r.width() - 2, r.height() - 2), KDColorWhite);

        auto rp = paletteAt(m_px, m_py);
        ctx->strokeRect(rp, KDColorBlack);
        ctx->strokeRect(KDRect(rp.left() + 1, rp.top() + 1, r.width() - 2, r.height() - 2), KDColorWhite);
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

    void CanvaView::movePaletteByOffset(int offsetX, int offsetY)
    {
        markRectAsDirty(paletteAt(m_px, m_py));
        if (!(m_px + offsetX < 0 || m_px + offsetX >= k_paletteNumberOfCells)) {
            m_px += offsetX;
        }
        if (!(m_py + offsetY < 0 || m_py + offsetY >= k_paletteNumberOfCells)) {
            m_py += offsetY;
        }
        markRectAsDirty(paletteAt(m_px, m_py));
    }

    KDRect CanvaView::cellAt(int x, int y) const
    {
        return KDRect(x * k_cellSize, y * k_cellSize, k_cellSize, k_cellSize);
    }

    KDRect CanvaView::paletteAt(int x, int y) const
    {
        return KDRect(k_paletteOffsetX + (x * k_paletteCellSize), y * k_paletteCellSize, k_paletteCellSize, k_paletteCellSize);
    }

}

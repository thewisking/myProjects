#pragma once

#include "screen_grid_geometry.h" // again don't ask intellisense problem :>

template <std::size_t Dimension>
typename ScreenGridGeometry<Dimension>::Coord
ScreenGridGeometry<Dimension>::logical_to_screen_nd(const Index &indices) const noexcept {
    Coord result{};

    for (std::size_t d = 0; d < Dimension; ++d) {
        const int cell = m_cellSize[d];
        result[d] = m_topLeft[d] + static_cast<int>(indices[d]) * cell + cell / 2;
    }

    return result;
}

template <std::size_t Dimension>
ScreenPoint
ScreenGridGeometry<Dimension>::logical_to_screen_point_2d(const Index &indices) const noexcept {
    ScreenPoint p{};

    if constexpr (Dimension >= 2) {
        // Convention:
        // indices[0] = row (vertical)
        // indices[1] = col (horizontal)
        const std::size_t row = indices[0];
        const std::size_t col = indices[1];

        const int cellWidth = m_cellSize[0];
        const int cellHeight = m_cellSize[1];

        const int x0 = m_topLeft[0];
        const int y0 = m_topLeft[1];

        p.x = x0 + static_cast<int>(col) * cellWidth + cellWidth / 2;
        p.y = y0 + static_cast<int>(row) * cellHeight + cellHeight / 2;

    } else if constexpr (Dimension == 1) {
        // Degenerate case: 1D line mapped horizontally from top_left[0]
        const std::size_t col = indices[0];
        const int cellWidth = m_cellSize[0];

        const int x0 = m_topLeft[0];
        const int y0 = m_topLeft[1];

        p.x = x0 + static_cast<int>(col) * cellWidth + cellWidth / 2;
        p.y = y0;
    }

    return p;
}

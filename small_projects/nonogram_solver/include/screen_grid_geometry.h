#pragma once

#include <array>
#include <cstddef>

// 2D screen point
struct ScreenPoint {
    int x = 0;
    int y = 0;
};

template <std::size_t Dimension>
class ScreenGridGeometry {
  public:
    using Index = std::array<std::size_t, Dimension>;
    using Coord = std::array<int, Dimension>;
    using DimSizes = std::array<std::size_t, Dimension>;

    ScreenGridGeometry() = default;

    // getters and setters

    const Coord &top_left() const noexcept { return m_topLeft; }
    void set_top_left(const Coord &coord) noexcept { m_topLeft = coord; }

    const DimSizes &dimension_sizes() const noexcept { return m_dimensionSizes; }
    void set_dimension_sizes(const DimSizes &sizes) noexcept { m_dimensionSizes = sizes; }

    const Coord &cell_size() const noexcept { return m_cellSize; }
    void set_cell_size(const Coord &size) noexcept { m_cellSize = size; }

    // General N-D mapping: each axis d is:
    Coord logical_to_screen_nd(const Index &indices) const noexcept;

    // Convenience 2D mapping:
    ScreenPoint logical_to_screen_point_2d(const Index &indices) const noexcept;

  private:
    Coord m_topLeft{};           // Coordinates: e.g. {x, y, ...}
    DimSizes m_dimensionSizes{}; // Size of dimensions: e.g. {rows, cols, ...}
    Coord m_cellSize{};          // Cell size: e.g. {cell_width, cell_height, ...}
};

#include "screen_grid_geometry.tpp"

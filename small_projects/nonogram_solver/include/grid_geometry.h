#pragma once

#include "screenshot.h"
#include <cstddef>
#include <optional>

struct CellCoord {
    std::size_t row;
    std::size_t col;
};

struct Point2i {
    int x;
    int y;
};

class GridGeometry {
  public:
    GridGeometry(std::size_t rows,
                 std::size_t cols,
                 int imageWidth,
                 int imageHeight);

    std::size_t rows() const { return m_rows; }
    std::size_t cols() const { return m_cols; }

    std::optional<Point2i> cell_center(std::size_t row, std::size_t col) const;

    bool is_valid() const { return m_valid; }

  private:
    std::size_t m_rows = 0;
    std::size_t m_cols = 0;
    double m_cellWidth = 0.0;
    double m_cellHeight = 0.0;
    bool m_valid = false;
};

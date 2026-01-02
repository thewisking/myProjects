#include "../include/grid_geometry.h"
#include <iostream>

GridGeometry::GridGeometry(std::size_t rows,
                           std::size_t cols,
                           int imageWidth,
                           int imageHeight)
    : m_rows(rows),
      m_cols(cols) {

    if (rows == 0 || cols == 0 || imageWidth <= 0 || imageHeight <= 0) {
        std::cerr << "GridGeometry: invalid dimensions\n";
        m_valid = false;
        return;
    }

    m_cellWidth = static_cast<double>(imageWidth) / static_cast<double>(cols);
    m_cellHeight = static_cast<double>(imageHeight) / static_cast<double>(rows);

    if (m_cellWidth <= 0.0 || m_cellHeight <= 0.0) {
        std::cerr << "GridGeometry: non-positive cell size\n";
        m_valid = false;
        return;
    }

    m_valid = true;
}

std::optional<Point2i> GridGeometry::cell_center(std::size_t row,
                                                 std::size_t col) const {
    if (!m_valid)
        return std::nullopt;
    if (row >= m_rows || col >= m_cols)
        return std::nullopt;

    // We assume the grid fills the entire screenshot for now:
    // x in [0, imageWidth), y in [0, imageHeight)
    double cx = (static_cast<double>(col) + 0.5) * m_cellWidth;
    double cy = (static_cast<double>(row) + 0.5) * m_cellHeight;

    Point2i p;
    p.x = static_cast<int>(cx);
    p.y = static_cast<int>(cy);
    return p;
}

#pragma once

#include "cell_state.h"
#include <array>
#include <vector>

// NonogramSolution
// Holds the current state of a nonogram grid in N dimensions.
// Dimension is a template parameter so we are not locked to 2D.
template <std::size_t Dimension>
class NonogramSolution {
  public:
    using Cell = CellState;
    using Line = std::vector<Cell>;                        // one line
    using AxisLines = std::vector<Line>;                   // all lines along one axis
    using AllAxesLines = std::array<AxisLines, Dimension>; // optional per-axis view
    using Coordinate = std::array<std::size_t, Dimension>;
    using BatchUpdate = std::vector<std::pair<Coordinate, CellState>>;

    // axisLengths[axis] = size along that axis (e.g. width, height, depth...)
    explicit NonogramSolution(const std::array<std::size_t, Dimension> &axisLengths);

    // return the whole solution
    const std::array<std::size_t, Dimension> &dimensions() const noexcept;

    // Read a single cell;
    CellState cell_at(const Coordinate &coord) const;

    // change a cell's value
    void set_cell(const Coordinate &coord, const CellState &state);

    // add a single point
    void add_point(const Coordinate &coord, const CellState &state);

    // Add a batch of points
    void add_points(const BatchUpdate &updates);

    // Reset everything back to unknown
    void clear();

    // derive a line view along a given axis
    Line get_line(std::size_t axis, std::size_t lineIndex) const;

  private:
    std::array<std::size_t, Dimension> m_dimensions; // the size of all dimensions
    std::vector<Cell> m_cells;                       // single canonical ND grid (flattened)

    std::size_t index_from_coord(const Coordinate &coord) const;
};

#include "nonogram_solution.tpp"
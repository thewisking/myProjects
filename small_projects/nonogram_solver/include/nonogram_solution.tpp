#pragma once

#include "nonogram_solution.h" // for Intellisense use. mine complains if I dont do this
#include <stdexcept>

// Constructor
template <std::size_t Dimension>
NonogramSolution<Dimension>::NonogramSolution(
    const std::array<std::size_t, Dimension> &axisLengths)
    : m_dimensions(axisLengths) {
    std::size_t total = 1;
    for (std::size_t len : m_dimensions) {
        total *= len;
    }
    m_cells.assign(total, Cell::unknown());
}

template <std::size_t Dimension>
const std::array<std::size_t, Dimension> &
NonogramSolution<Dimension>::dimensions() const noexcept {
    return m_dimensions;
}

template <std::size_t Dimension>
std::size_t
NonogramSolution<Dimension>::index_from_coord(const Coordinate &coord) const {
    std::size_t index = 0;
    std::size_t stride = 1;

    for (std::size_t axis = 0; axis < Dimension; ++axis) {
        if (coord[axis] >= m_dimensions[axis]) {
            throw std::out_of_range("NonogramSolution: coordinate out of bounds");
        }
        index += coord[axis] * stride;
        stride *= m_dimensions[axis];
    }

    return index;
}

template <std::size_t Dimension>
typename NonogramSolution<Dimension>::Cell
NonogramSolution<Dimension>::cell_at(const Coordinate &coord) const {
    return m_cells[index_from_coord(coord)];
}

template <std::size_t Dimension>
void NonogramSolution<Dimension>::set_cell(const Coordinate &coord, const Cell &state) {
    m_cells[index_from_coord(coord)] = state;
}

template <std::size_t Dimension>
void NonogramSolution<Dimension>::add_point(const Coordinate &coord, const Cell &state) {
    set_cell(coord, state);
}

template <std::size_t Dimension>
void NonogramSolution<Dimension>::add_points(const BatchUpdate &updates) {
    for (const auto &[coord, state] : updates) {
        set_cell(coord, state);
    }
}

template <std::size_t Dimension>
void NonogramSolution<Dimension>::clear() {
    for (auto &cell : m_cells) {
        cell = Cell::unknown();
    }
}

template <std::size_t Dimension>
typename NonogramSolution<Dimension>::Line
NonogramSolution<Dimension>::get_line(std::size_t axis, std::size_t lineIndex) const {
    if (axis >= Dimension) {
        throw std::out_of_range("NonogramSolution: axis out of bounds");
    }

    Line line;
    const auto &dims = m_dimensions;

    if (lineIndex >= dims[axis]) {
        throw std::out_of_range("NonogramSolution: lineIndex out of bounds");
    }

    // Build a coordinate and vary along axis 0 by default (for now).
    Coordinate coord{};
    for (std::size_t i = 0; i < Dimension; ++i) {
        coord[i] = 0;
    }
    coord[axis] = lineIndex;

    for (std::size_t pos = 0; pos < dims[0]; ++pos) {
        coord[0] = pos;
        line.push_back(cell_at(coord));
    }

    return line;
}

#pragma once

#include <stdexcept>

template <std::size_t Dimension>
NonogramClues<Dimension>::NonogramClues(
    const typename NonogramClues<Dimension>::AllClues &puzzleClues)
    : m_size{}, m_clues(puzzleClues) {
    for (std::size_t axis = 0; axis < Dimension; ++axis) {
        m_size[axis] = m_clues[axis].size();
    }
}

template <std::size_t Dimension>
const typename NonogramClues<Dimension>::AxisClues &
NonogramClues<Dimension>::axis_clues(std::size_t axis) const {
    if (axis >= Dimension) {
        throw std::out_of_range("NonogramClues::axis_clues: axis out of range");
    }
    return m_clues[axis];
}

template <std::size_t Dimension>
std::size_t NonogramClues<Dimension>::axis_count(std::size_t axis) const {
    if (axis >= Dimension) {
        throw std::out_of_range("NonogramClues::axis_count: axis out of range");
    }
    return m_size[axis];
}

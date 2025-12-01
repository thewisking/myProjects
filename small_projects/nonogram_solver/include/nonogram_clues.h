#pragma once

#include <array>
#include <cstddef>
#include <vector>

template <std::size_t Dimension>
class NonogramClues {
  public:
    using LineClues = std::vector<int>;                // one line
    using AxisClues = std::vector<LineClues>;          // all lines along one axis
    using AllClues = std::array<AxisClues, Dimension>; // all axes

    NonogramClues() = default;
    // needs an array of vectors, one for each axes. each axes is a vector of vectors of ints.
    explicit NonogramClues(const AllClues &puzzleClues);

    // Number of lines along a given axis (rows, columns, etc.)
    std::size_t axis_count(std::size_t axis) const;

    // All line clues along a given axis
    const AxisClues &axis_clues(std::size_t axis) const;

  private:
    std::array<std::size_t, Dimension> m_size{}; // number of lines per axis
    AllClues m_clues{};
};

#include "nonogram_clues.tpp"
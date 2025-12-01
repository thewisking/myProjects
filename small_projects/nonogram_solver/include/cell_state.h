#pragma once
#include <cstdint>

// Represents the state of a single nonogram cell, including colour.
enum class CellOccupancy {
    Unknown, // not decided yet
    Empty,   // definitely not filled
    Filled   // filled with some colour
};

struct CellState {
    CellOccupancy occupancy{CellOccupancy::Unknown};

    // Logical colour index. Only meaningful when occupancy == Filled.
    // For black-and-white puzzles, just use 0 for "black"
    std::int32_t colourId{-1};

    static CellState unknown() {
        return {CellOccupancy::Unknown, -1};
    }

    static CellState empty() {
        return {CellOccupancy::Empty, -1};
    }

    static CellState filled(std::int32_t colourId) {
        return {CellOccupancy::Filled, colourId};
    }

    bool is_unknown() const { return occupancy == CellOccupancy::Unknown; }
    bool is_empty() const { return occupancy == CellOccupancy::Empty; }
    bool is_filled() const { return occupancy == CellOccupancy::Filled; }
};

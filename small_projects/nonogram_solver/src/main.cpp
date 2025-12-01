#include "../include/nonogram_solution.h"
#include <iostream>
int main() {
    system("cls");

    const std::size_t Dimension = 2;

    using Coordinate = std::array<std::size_t, Dimension>;
    using BatchUpdate = NonogramSolution<2>::BatchUpdate;

    NonogramSolution<2> solution({5, 5});
    solution.add_point({0, 1}, CellState::filled(0));

    BatchUpdate batchOne;
    batchOne.reserve(5 * 3);

    // clang-format off
std::vector<Coordinate> coords = {
    {0, 1}, {0, 2}, {0, 3},
    {1, 1}, {1, 3}, {1, 4},
    {2, 0}, {2, 1}, {2, 3},
    {3, 1}, {3, 3}, {3, 4},
    {4, 1}, {4, 2}, {4, 3}
};
    // clang-format on

    for (const auto &coord : coords) {
        batchOne.emplace_back(coord, CellState::filled(0));
    }
    solution.add_points(batchOne);

    const auto &dims = solution.dimensions();
    using Coordinate = NonogramSolution<Dimension>::Coordinate;

    // How many “slices” when you collapse all dimensions >= 3
    std::size_t sliceCount = 1;
    for (std::size_t axis = 2; axis < Dimension; ++axis) {
        sliceCount *= dims[axis];
    }

    if (sliceCount == 0) {
        sliceCount = 1; // for Dimension == 1 or weird degenerate cases
    }

    // Outer: for each slice across higher dimensions (or 1 slice if Dimension <= 2)
    for (std::size_t slice = 0; slice < sliceCount; ++slice) {

        if (Dimension > 2) {
            std::cout << "Slice " << slice << '\n';
        }

        for (std::size_t row = 0; row < dims[0]; ++row) {
            for (std::size_t col = 0; col < dims[1]; ++col) {
                Coordinate coord{};
                coord[0] = row; // index 0 = row
                coord[1] = col; // index 1 = col

                CellState cell = solution.cell_at(coord);
                std::cout << (cell.is_filled() ? '#' : '.');
            }
            std::cout << '\n';
        }

        if (Dimension > 2) {
            std::cout << '\n'; // blank line between slices
        }
    }
}

#pragma once

#include "../nonogram_solver_strategy.h"

template <std::size_t Dimension>
class GessingNonogramSolver : public NonogramSolverStrategy<Dimension> {
  public:
    using Clues = NonogramClues<Dimension>;
    using Solution = NonogramSolution<Dimension>;
    Solution solve(const Clues &clues) override;
};
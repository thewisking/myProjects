#pragma once

#include "nonogram_clues.h"
#include "nonogram_solution.h"

template <std::size_t Dimension>
class NonogramSolverStrategy {
  public:
    using Clues = NonogramClues<Dimension>;
    using Solution = NonogramSolution<Dimension>;
   
    virtual ~NonogramSolverStrategy() = default;
    virtual Solution solve(const Clues &clues) = 0;
};
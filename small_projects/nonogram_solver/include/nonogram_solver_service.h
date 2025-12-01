#pragma once

#include <cstddef>
#include <memory>

#include "nonogram_clues.h"
#include "nonogram_solution.h"
#include "nonogram_solver_strategy.h"

template <std::size_t Dimension>
class NonogramSolverService {
  public:
    using Clues = NonogramClues<Dimension>;
    using Solution = NonogramSolution<Dimension>;
    using Strategy = NonogramSolverStrategy<Dimension>;
    enum class SolverKind {
        Logical,
        Guessing
    };
    explicit NonogramSolverService(SolverKind defaultKind = SolverKing::Logical);
    void set_solver_kind(SolverKind kind);
    Solution solve(const Clues &clues);

  private:
    SolverKind m_solverKind;
    std::unique_ptr<Strategy> m_strategy;
    void recreate_strategy_if_needed();
};

#include "nonogram_solver_service.h"
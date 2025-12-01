#pragma once

#include "nonogram_solver_service.h"
#include "nonogram_solvers/guessing_nonogram_solver.h"
#include "nonogram_solvers/logical_nonogram_solver.h"

template <std::size_t Dimension>
NonogramSolverService<Dimension>::NonogramSolverService(SolverKind defaultKind)
    : m_solverKind(defaultKind) {
    recreate_strategy_if_needed();
}

template <std::size_t Dimension>
void NonogramSolverService<Dimension>::set_solver_kind(SolverKind kind) {
    if (m_solverKind == kind) {
        return;
    }

    m_solverKind = kind;
    recreate_strategy_if_needed();
}

template <std::size_t Dimension>
typename NonogramSolverService<Dimension>::Solution
NonogramSolverService<Dimension>::solve(const Clues &clues) {
    if (!m_strategy) {
        recreate_strategy_if_needed();
    }

    return m_strategy->solve(clues);
}

template <std::size_t Dimension>
void NonogramSolverService<Dimension>::recreate_strategy_if_needed() {
    switch (m_solverKind) {
    case SolverKind::Logical:
        m_strategy = std::make_unique<LogicalNonogramSolver<Dimension>>();
        break;

    case SolverKind::Guessing:
        m_strategy = std::make_unique<GuessingNonogramSolver<Dimension>>();
        break;
    }
}

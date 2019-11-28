#pragma once

#include "Solver.h"
using namespace azgra;
struct Student : Individual
{

};

class TLBOSolver : public Solver
{
private:
    std::vector<Student> m_students;
    Student m_teacher;

    void generate_initial_students();
    void set_teacher();
    [[nodiscard]] std::vector<f64> get_current_students_mean() const;
    std::uniform_int_distribution<size_t> m_randomStudent;
    f64 average_fitness() const;

public:
    TLBOSolver(const OptimalizationProblem &problem, size_t populationSize);

    OptimizationResult solve();
};
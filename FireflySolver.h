#pragma once

#include "Solver.h"

struct Firefly : Individual
{
    bool isBest = false;

    Firefly() = default;

    explicit Firefly(Individual &individual)
    {
        attributes = std::move(individual.attributes);
        fitness = individual.fitness;
    }

    f64 distance(const Firefly &other)
    {
        always_assert(attributes.size() == other.attributes.size());
        f64 sum = 0.0;
        for (size_t i = 0; i < attributes.size(); ++i)
        {
            sum += pow((attributes[i] - other.attributes[i]), 2.0);
        }
        const f64 result = sqrt(sum);
        return result;
    }


    bool operator<(const Firefly &other)
    {
        return (this->fitness < other.fitness);
    }
};


class FireflySolver : public Solver
{
private:
    f64 m_beta0 = 1.9;
    f64 m_alpha = 0.3;
    f64 m_gamma = 0.01;

    std::vector<Firefly> m_fireflies;

    void initialize_fireflies();

    void move_firefly(Firefly &current, const Firefly &other);

    void move_best_firefly(Firefly &best);

    void set_best_firefly();

    [[nodiscard]] Firefly const &get_best_firefly() const;

    f64 average_fitness() const;

public:

    explicit FireflySolver(const OptimalizationProblem &problem,
                           const size_t populationSize,
                           const f64 beta0 = 1.9,
                           const f64 alpha = 0.3,
                           const f64 gamma = 0.01);

    OptimizationResult solve();

};
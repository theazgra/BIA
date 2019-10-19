#pragma once

#include <random>
#include <azgra/matrix.h>
#include <azgra/geometry/point.h>
#include <azgra/collection/vector_linq.h>
#include <algorithm>
#include <azgra/geometry/plot.h>

namespace tsp
{

    struct TspSolution
    {
        std::vector<size_t> bestSolution;
        std::vector<std::vector<size_t>> solutionHistory;
    };

    using namespace azgra;

    class TspSolver
    {
    private:
        size_t m_cityCount;
        size_t m_populationSize;
        Matrix<f64> m_cityDistance;
        std::vector<geometry::Point2D<f64>> m_cities;
        std::vector<std::vector<size_t>> m_currentPopulation;

        std::discrete_distribution<int> m_mutationDistribution;
        std::uniform_int_distribution<size_t> m_cityCountDistribution;
        std::uniform_int_distribution<size_t> m_populationDistribution;
        std::uniform_int_distribution<size_t> m_crossoverDistribution;

        std::pair<size_t, size_t> generate_random_pair(std::mt19937 &generator, std::uniform_int_distribution<size_t> &distribution);

        void initialize_distance_matrix();

        void generate_random_population(const size_t startingCity = 0);

        std::vector<size_t> breed(const size_t parentA, const size_t parentB, std::mt19937 &generator);

        std::vector<std::vector<size_t>> generate_offsprings();

        void replace_population(const std::vector<std::vector<size_t>> &nextPopulation);

        [[nodiscard]] f64 cell_cost(const std::vector<size_t> &cell) const;

        [[nodiscard]] f64 current_population_average_distance() const;

        [[nodiscard]] std::vector<size_t> get_best_from_population() const;

        [[nodiscard]] std::vector<azgra::geometry::Point2D<f64>> generate_random_cities(size_t cityCount, f64 maxX, f64 maxY) const;

    public:
        explicit TspSolver(std::vector<geometry::Point2D<f64>> cities, size_t populationSize);

        explicit TspSolver(const size_t cityCout, const f64 maxX, const f64 maxY, size_t populationSize);

        TspSolution solve(const size_t generationCount, const f64 mutationProbability);

        std::vector<geometry::Point2D<f64>> const &get_cities() const;
    };

}
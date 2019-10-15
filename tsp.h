#pragma once

#include <random>
#include <azgra/matrix.h>
#include <azgra/geometry/point.h>
#include <azgra/collection/vector_linq.h>
#include <algorithm>

namespace tsp
{
    using namespace azgra;
    class TspSolver
    {
    private:
        size_t m_cityCount;
        size_t m_populationSize;
        Matrix<f64> m_cityDistance;
        std::vector<geometry::Point2D<f64>> m_cities;
        std::vector<std::vector<size_t>> m_currentPopulation;

        void initialize_distance_matrix();
        void generate_random_population(const size_t startingCity = 0);

    public:
        explicit TspSolver(std::vector<geometry::Point2D<f64>> cities, size_t populationSize);

        std::vector<geometry::Point2D<f64>> solve(const size_t generationCount, const f64 mutationProbability);
    };

}
#include "tsp.h"

#include <random>

namespace tsp
{
    TspSolver::TspSolver(std::vector<geometry::Point2D<f64>> cities, size_t populationSize)
    {
        m_cities = std::vector<geometry::Point2D<f64>>(std::move(cities));
        m_cityCount = m_cities.size();
        m_populationSize = populationSize;
        m_currentPopulation.resize(m_populationSize);
        initialize_distance_matrix();
    }

    void TspSolver::initialize_distance_matrix()
    {
        m_cityDistance = Matrix<f64>(m_cityCount, m_cityCount, 0.0);
        for (size_t row = 0; row < m_cityCount; ++row)
        {
            for (size_t col = row + 1; col < m_cityCount; ++col)
            {
                f64 distance = m_cities[row].euclidean_distance(m_cities[col]);
                m_cityDistance.at(row, col) = distance;
                m_cityDistance.at(col, row) = distance;
            }
        }
    }

    void TspSolver::generate_random_population(const size_t startingCity)
    {
        // All cells must start at the same city
        std::vector<size_t> basicCell(m_cityCount - 1);
        size_t index = 0;
        for (size_t i = 0; i < m_cityCount; ++i)
        {
            if (i == startingCity)
                continue;
            basicCell[index++] = i;
        }

        auto mtGenerator = std::mt19937(std::random_device()());
        for (size_t cellIndex = 0; cellIndex < m_populationSize; ++cellIndex)
        {
            std::shuffle(basicCell.begin(), basicCell.end(), mtGenerator);
            // NOTE(Moravec): Should we check if basicCell shuffle was successful?
            m_currentPopulation[cellIndex].push_back(startingCity);
            m_currentPopulation[cellIndex].insert(m_currentPopulation[cellIndex].end(),basicCell.begin(),basicCell.end());
            assert(m_currentPopulation[cellIndex].size() == m_cityCount);
        }
    }

    std::vector<geometry::Point2D<f64>> TspSolver::solve(const size_t generationCount, const f64 mutationProbability)
    {
        generate_random_population();

        for (size_t generation = 0; generation < generationCount; ++generation)
        {
            // for each cell in current population select random partner
            // create offspring ( two way breeding )
            // mutation of offspring with probability
            // compare offspring with current cell -> replace?
            //      comparing based on fitness -> minimum distance
        }


        return std::vector<geometry::Point2D<f64>>();
    }


}
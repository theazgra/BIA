#include "tsp.h"

#include <random>

namespace tsp
{
    std::vector<azgra::geometry::Point2D<f64>> generate_random_cities(size_t cityCount, f64 radius)
    {
        geometry::Point2D<f64> center(500, 500);
        std::vector<azgra::geometry::Point2D<f64>> cities(cityCount);
        f64 x, y;
        f64 angleStep = (2.0 * M_PI) / cityCount;
        f64 angle = 0;
        for (size_t i = 0; i < cityCount; ++i)
        {
            x = center.x + (radius * cos(angle));
            y = center.y + (radius * sin(angle));
            cities[i] = azgra::geometry::Point2D<f64>(x, y);

            angle += angleStep;
        }

        return cities;
    }

    TspSolver::TspSolver(std::vector<geometry::Point2D<f64>> cities, size_t populationSize)
    {
        m_cities = std::vector<geometry::Point2D<f64>>(std::move(cities));
        m_cityCount = m_cities.size();
        m_populationSize = populationSize;
        m_currentPopulation.resize(m_populationSize);
        initialize_distance_matrix();
    }

    TspSolver::TspSolver(const size_t cityCout, const f64 maxX, const f64 maxY, size_t populationSize) :
            TspSolver(generate_random_cities(cityCout), populationSize)
    {
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

            m_currentPopulation[cellIndex].insert(m_currentPopulation[cellIndex].end(), basicCell.begin(), basicCell.end());
            always_assert(m_currentPopulation[cellIndex].size() == m_cityCount);
            always_assert(m_currentPopulation[cellIndex][0] == startingCity);
        }
    }

    TspSolution TspSolver::solve(const size_t generationCount, const f64 mutationProbability)
    {
        TspSolution solution = {};
        generate_random_population();
        m_mutationDistribution = std::discrete_distribution<int>({1.0 - mutationProbability, mutationProbability});
        m_cityCountDistribution = std::uniform_int_distribution<size_t>(0, m_cityCount - 1);
        m_populationDistribution = std::uniform_int_distribution<size_t>(0, m_populationSize - 1);
        m_crossoverDistribution = std::uniform_int_distribution<size_t>(1, m_cityCount - 1);

        f64 currentCost = current_population_average_distance();
        fprintf(stdout, "New generation cost: %.6f\n", currentCost);
        for (size_t generation = 0; generation < generationCount; ++generation)
        {
            auto nextGeneration = generate_offsprings();
            replace_population(nextGeneration);
            currentCost = current_population_average_distance();
            fprintf(stdout, "New generation cost: %.6f\n", currentCost);

            solution.solutionHistory.push_back(std::move(get_best_from_population()));
        }

        solution.bestSolution = std::move(get_best_from_population());

        return solution;
    }

    std::vector<size_t> TspSolver::get_best_from_population() const
    {
        std::vector<size_t> best;
        f64 bestCost = std::numeric_limits<f64>::max();
        for (size_t cellIndex = 0; cellIndex < m_populationSize; ++cellIndex)
        {
            f64 currentCellCost = cell_cost(m_currentPopulation[cellIndex]);
            if (currentCellCost < bestCost)
            {
                bestCost = currentCellCost;
                best = m_currentPopulation[cellIndex];
            }
        }
        return best;
    }

    void TspSolver::replace_population(const std::vector<std::vector<size_t>> &nextPopulation)
    {
        for (size_t cellIndex = 0; cellIndex < m_populationSize; ++cellIndex)
        {
            f64 currentCellCost = cell_cost(m_currentPopulation[cellIndex]);
            f64 offspringCellCost = cell_cost(nextPopulation[cellIndex]);
            if (offspringCellCost <= currentCellCost)
            {
                m_currentPopulation[cellIndex] = nextPopulation[cellIndex];
            }
        }
    }


    std::vector<std::vector<size_t>> TspSolver::generate_offsprings()
    {
        std::random_device m_randomDevice;
        std::mt19937 MTgenerator(m_randomDevice());

        std::vector<std::vector<size_t>> nextPopulation(m_populationSize);
        for (size_t cellIndex = 0; cellIndex < m_populationSize; ++cellIndex)
        {
            size_t partnerIndex = m_populationDistribution(MTgenerator);
            while (partnerIndex == cellIndex)
            {
                partnerIndex = m_populationDistribution(MTgenerator);
            }
            nextPopulation[cellIndex] = breed(cellIndex, partnerIndex, MTgenerator);
        }
        return nextPopulation;
    }

    std::pair<size_t, size_t> TspSolver::generate_random_pair(std::mt19937 &generator, std::uniform_int_distribution<size_t> &distribution)
    {
        size_t p1 = distribution(generator);
        size_t p2 = distribution(generator);
        while (p2 == p1)
        {
            p2 = distribution(generator);
        }
        return std::make_pair(p1, p2);
    }

    f64 TspSolver::cell_cost(const std::vector<size_t> &cell) const
    {
        assert(cell.size() == m_cityCount);
        f64 distance = 0.0;
        for (size_t i = 0; i < m_cityCount; ++i)
        {
            distance += m_cityDistance.at(cell[i], cell[(i + 1) % m_cityCount]);
        }
        return distance;
    }

    std::vector<size_t> TspSolver::breed(const size_t parentA, const size_t parentB, std::mt19937 &generator)
    {
        always_assert(m_currentPopulation[parentA][0] == 0);
        always_assert(m_currentPopulation[parentB][0] == 0);

        auto points = generate_random_pair(generator, m_crossoverDistribution);
        size_t crossoverP1 = points.first;
        size_t crossoverP2 = points.second;
        if (crossoverP2 < crossoverP1)
        {
            auto tmp = crossoverP1;
            crossoverP1 = crossoverP2;
            crossoverP2 = tmp;
        }
        always_assert(crossoverP1 < crossoverP2);

        std::vector<size_t> offspring(m_cityCount);
        always_assert(m_currentPopulation[parentA][0] == m_currentPopulation[parentB][0]);
        offspring[0] = m_currentPopulation[parentA][0];
        for (size_t i = 1; i < m_cityCount; ++i)
        {
            if (i < crossoverP1)
            {
                offspring[i] = m_currentPopulation[parentA][i];
            }
            else if (i >= crossoverP2)
            {
                offspring[i] = m_currentPopulation[parentA][i];
            }
        }

        size_t parentBGeneIndex = 0;
#if DEBUG
        std::vector<size_t> geneHistory;
#endif
        for (size_t secondPartIndex = crossoverP1; secondPartIndex < crossoverP2; ++secondPartIndex)
        {
            size_t gene = m_currentPopulation[parentB][parentBGeneIndex++];
#if DEBUG
            geneHistory.push_back(gene);
#endif
            while (collection::contains(offspring, gene))
            {
                gene = m_currentPopulation[parentB][parentBGeneIndex++];
            }
            always_assert(gene < m_cityCount);
            offspring[secondPartIndex] = gene;
        }

        // mutate
        if (m_mutationDistribution(generator))
        {
            auto swapIndices = generate_random_pair(generator, m_crossoverDistribution);
            auto tmp = offspring[swapIndices.first];
            offspring[swapIndices.first] = offspring[swapIndices.second];
            offspring[swapIndices.second] = tmp;
        }
        return offspring;
    }

    f64 TspSolver::current_population_average_distance() const
    {
        f64 avgDistance = 0;
        for (size_t i = 0; i < m_populationSize; ++i)
        {
            avgDistance += cell_cost(m_currentPopulation[i]);
        }
        avgDistance /= static_cast<f64>(m_populationSize);
        return avgDistance;
    }

    std::vector<geometry::Point2D<f64>> const &TspSolver::get_cities() const
    {
        return m_cities;
    }
}
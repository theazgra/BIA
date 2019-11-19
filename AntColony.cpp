#include "AntColony.h"

using namespace azgra;
using namespace azgra::collection;

AntColony::AntColony(std::vector<geometry::Point2D<f64>> &cities, size_t antCount, const f64 pheromoneImportance,
                     const f64 distanceImportance, const f64 evaporationRate, const size_t iterationCount)
{

    m_cities = std::move(cities);
    m_antCount = antCount;
    m_pheromoneImportance = pheromoneImportance;
    m_distanceImportance = distanceImportance;
    m_iterationCount = iterationCount;
    m_evaporationRate = evaporationRate;

    m_cityCount = m_cities.size();
    always_assert(m_cityCount > 1);

    m_pheromoneMatrix = Matrix<f64>(m_cityCount, m_cityCount, 1.0);
    m_distanceMatrix = Matrix<f64>(m_cityCount, m_cityCount, 0.0);
    for (size_t row = 0; row < m_cityCount; ++row)
    {
        for (size_t col = row + 1; col < m_cityCount; ++col)
        {
            f64 distance = m_cities[row].euclidean_distance(m_cities[col]);
            m_distanceMatrix.at(row, col) = distance;
            m_distanceMatrix.at(col, row) = distance;
        }
    }

    m_generator = std::mt19937(m_rd());
}

void AntColony::ant_navigation(Ant &ant)
{
    while (ant.path.size() != m_cityCount)
    {
        std::vector<f64> cityWeights;
        cityWeights.reserve(ant.unvisitedCities.size());
        for (const CityId &unvisited : ant.unvisitedCities)
        {
            const f64 numerator = pow(m_pheromoneMatrix.at(ant.currentCity, unvisited), m_pheromoneImportance) *
                                  pow((1.0 / m_distanceMatrix.at(ant.currentCity, unvisited)), m_distanceImportance);

            cityWeights.push_back(numerator);
        }
        const f64 totalWeight = std::accumulate(cityWeights.begin(), cityWeights.end(), 0.0);
        for (f64 &cityWeight : cityWeights)
        {
            cityWeight /= totalWeight;
        }
        std::discrete_distribution<size_t> randomCity(cityWeights.begin(), cityWeights.end());
        CityId cityIndex = ant.unvisitedCities[randomCity(m_generator)];
        ant.unvisitedCities.erase(ant.unvisitedCities.begin() + azgra::collection::get_index(ant.unvisitedCities, cityIndex));
        ant.path.push_back(cityIndex);
        ant.currentCity = cityIndex;
    }
    ant.pathCost = ant_path_cost(ant.path);
}

f64 AntColony::ant_path_cost(const std::vector<CityId> &cities) const
{
    always_assert(cities.size() == m_cityCount);
    f64 distance = 0.0;
    for (size_t i = 0; i < m_cityCount; ++i)
    {
        distance += m_distanceMatrix.at(cities[i], cities[(i + 1) % m_cityCount]);
    }
    return distance;
}

void AntColony::initialize_ants()
{
    m_ants.resize(m_antCount);
    const auto allCities = Enumerable<CityId>::range(1, m_cityCount).to_vector();
    for (Ant &ant : m_ants)
    {
        ant.currentCity = 0;
        ant.path.push_back(0);
        ant.unvisitedCities = std::vector<CityId>(allCities.begin(), allCities.end());
        ant.pathCost = std::numeric_limits<f64>::max();
    }
}


tsp::TspSolution AntColony::solve()
{
    tsp::TspSolution solution = {};
    // Initialize ants.

    initialize_ants();

    for (size_t it = 0; it < m_iterationCount; ++it)
    {
        // Hey ant find your way.
        for (Ant &ant : m_ants)
        {
            ant_navigation(ant);
        }
    }

    return solution;
}




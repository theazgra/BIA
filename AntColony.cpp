#include "AntColony.h"

using namespace azgra;
using namespace azgra::collection;

AntColony::AntColony(std::vector<geometry::Point2D<f64>> &cities, size_t antCount, const f64 pheromoneImportance,
                     const f64 distanceImportance, const size_t iterationCount)
{
    m_cities = std::move(cities);
    m_antCount = antCount;
    m_pheromoneImportance = pheromoneImportance;
    m_distanceImportance = distanceImportance;
    m_iterationCount = iterationCount;

    m_cityCount = m_cities.size();
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
}

std::vector<CityId> AntColony::get_unvisited_cities(const Ant &ant) const
{
    const auto allCities = Enumerable<CityId>::range(0, m_cityCount).to_vector();
    std::vector<CityId> unvisited = except(allCities.begin(), allCities.end(),
                                           ant.cities.begin(), ant.cities.end());
    return std::vector<CityId>();
}

void AntColony::ant_navigation(Ant &ant) const
{
    CityId currentCity = 0;
    ant.cities.insert(currentCity);
    while (ant.cities.size() != m_cityCount)
    {

    }
}

tsp::TspSolution AntColony::solve()
{
    tsp::TspSolution solution = {};
    // Initialize ants.
    m_ants.resize(m_antCount);

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



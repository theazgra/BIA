#pragma once

#include "tsp.h"
#include <unordered_set>
#include <azgra/collection/enumerable.h>
using namespace azgra;

typedef size_t CityId;
struct Ant
{
    std::unordered_set<CityId> cities;
    f64 pathCost;
};

class AntColony
{
private:
    f64 m_pheromoneImportance;
    f64 m_distanceImportance;
    size_t m_antCount;
    size_t m_cityCount;
    size_t m_iterationCount;
    std::vector<geometry::Point2D<f64>> m_cities;
    std::vector<Ant> m_ants;

    Matrix<f64> m_distanceMatrix;
    Matrix<f64> m_pheromoneMatrix;

    void ant_navigation(Ant &ant) const;
    std::vector<CityId> get_unvisited_cities(const Ant &ant) const;
public:

    explicit AntColony(std::vector<geometry::Point2D<f64>> &cities, size_t antCount,
                       const f64 pheromoneImportance, const f64 distanceImportance,
                       const size_t iterationCount);

    tsp::TspSolution solve();
};

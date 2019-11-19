#pragma once

#include "tsp.h"
#include <azgra/collection/enumerable.h>
#include <bits/unordered_set.h>
#include <random>
using namespace azgra;

typedef size_t CityId;
struct Ant
{
    CityId currentCity;
    std::vector<CityId> path;
    std::vector<CityId> unvisitedCities;
    f64 pathCost;
};

class AntColony
{
private:
    f64 m_pheromoneImportance;
    f64 m_distanceImportance;
    f64 m_evaporationRate;
    size_t m_antCount;
    size_t m_cityCount;
    size_t m_iterationCount;
    std::vector<geometry::Point2D<f64>> m_cities;
    std::vector<Ant> m_ants;

    Matrix<f64> m_distanceMatrix;
    Matrix<f64> m_pheromoneMatrix;

    std::random_device m_rd;
    std::mt19937 m_generator;

    void ant_navigation(Ant &ant);
    f64 ant_path_cost(const std::vector<CityId > &cities) const;
    void initialize_ants();
public:

    explicit AntColony(std::vector<geometry::Point2D<f64>> &cities, size_t antCount,
                       const f64 pheromoneImportance, const f64 distanceImportance,
                       const f64 evaporationRate, const size_t iterationCount);

    tsp::TspSolution solve();
};

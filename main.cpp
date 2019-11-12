#include <iostream>
#include "optimalization_functions_vis.h"
#include <azgra/cli/cli_arguments.h>
#include "tsp.h"
#include "DumpPermutations.h"
#include <azgra/utilities/Stopwatch.h>
#include <azgra/collection/vector_utilities.h>
#include "PSOSolver.h"
#include <azgra/geometry/plot.h>

static void draw_cities(const tsp::TspSolution &solution,
                        const std::vector<azgra::geometry::Point2D<f64>> &points)
{

    using namespace azgra::geometry;
#if 0
    std::vector<std::vector<std::vector<Point2D<f64>>>> linesInTime;
    for (const auto &order : solution.solutionHistory)
    {
        std::vector<std::vector<Point2D<f64>>> time;
        size_t orderSize = order.size();

        for (size_t i = 0; i < orderSize; ++i)
        {
            auto srcPoint = points[order[i]];
            auto dstPoint = points[order[(i + 1) % orderSize]];
            time.push_back({srcPoint, dstPoint});

        }
        linesInTime.push_back(time);
    }
    Plot::animate_lines("tmp", linesInTime, 0.01);
#else
    std::vector<std::vector<Point2D<f64>>> linesInTime;
    for (const auto &order : solution.solutionHistory)
    {
        size_t orderSize = order.size();
        std::vector<Point2D<f64>> time;

        for (size_t i = 0; i <= orderSize; ++i)
        {
            time.push_back(points[order[i % orderSize]]);
        }
        linesInTime.push_back(time);
    }
    Plot::animate_lines(linesInTime, 0.01, 5);
#endif
}

std::vector<std::vector<azgra::geometry::Point3D<f64>>>
individuals_to_points(const OptimizationResult &result,
                      const std::function<azgra::f64(const std::vector<azgra::f64> &)> &fn)
{

    std::vector<std::vector<azgra::geometry::Point3D<f64>>> points(result.invidualsInTime.size());
    for (size_t i = 0; i < result.invidualsInTime.size(); ++i)
    {
        const auto &migration = result.invidualsInTime[i];
        always_assert(migration[0].size() == 2);
        std::vector<azgra::geometry::Point3D<f64>> migrationPoints(migration.size());
        for (size_t j = 0; j < migration.size(); ++j)
        {
            migrationPoints[j] = azgra::geometry::Point3D<f64>(migration[j][0], migration[j][1], fn(migration[j]));
        }
        points[i] = migrationPoints;
    }
    return points;
}


static void print_permutation(const std::vector<int> &permutation)
{
    for (size_t i = 0; i < permutation.size(); ++i)
    {
        fprintf(stdout, "%i  ", permutation[i]);
    }
    fprintf(stdout, "\n");
}


int main(int argc, char **argv)
{
    const size_t dimension = 2;

    optimalization::OptimalizationProblem ackleyProb(ackley_simple, 1, dimension, generate_limits(dimension, -32.0, 32.0));
    optimalization::OptimalizationProblem schwefelProb(schwefel, 1, dimension, generate_limits(dimension, -500.0, 500.0));

//    SOMASolver soma(schwefelProb, 50, 50);
//    auto solution = soma.solve();
//    auto points3d = soma_individuals_to_points(solution,schwefel);

    PSOSolver psoSolver(schwefelProb, 50, 50);
    auto solution = psoSolver.solve();
    auto points3d = individuals_to_points(solution, schwefel);


    azgra::geometry::dump_3d_points_history(points3d, "pso_schwefel.pts");
    fprintf(stdout, "done\n");

    return 0;
}

#include <iostream>
#include <azgra/geometry/plot.h>
#include "FireflySolver.h"
#include "TLBOSolver.h"
#include "optimalization_functions.h"
#include "nsga2_solver.h"

#if 0
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
    Plot::animate_lines(linesInTime, 0.2, 1);
#endif
}
#endif
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

std::vector<azgra::geometry::Point2D<double>> to_points(const std::vector<std::pair<float,float>> &pts)
{
    std::vector<azgra::geometry::Point2D<double>> points(pts.size());

    for (size_t i = 0; i < pts.size(); i++)
    {
        points[i] = azgra::geometry::Point2D<double>(pts[i].first, pts[i].second);
    }
    return points;
}


int main(int argc, char **argv)
{
    const size_t dimension = 2;
    // NSGA2
    auto limits = generate_limits(dimension, -5.12, 5.12);
    OptimizationProblem problem_sphere(sphere, 50, dimension, limits);
    OptimizationProblem problem_rastrigin(rastrigin, 50, dimension, limits);

    NSGA2Solver nsga2({problem_sphere, problem_rastrigin}, 20);
    const auto solution = nsga2.solve();
    const auto points = to_points(solution);
    azgra::geometry::Plot("Pareto").add_2d_points(points).display_window();

    /*
    // Last session results
    OptimizationProblem ackleyProb(ackley_simple, 100, dimension, generate_limits(dimension, -32.0, 32.0));
    OptimizationProblem schwefelProb(schwefel, 1000, dimension, generate_limits(dimension, -500.0, 500.0));

    // Teaching learning base 
    TLBOSolver tlbo(ackleyProb, 40);
    auto solution = tlbo.solve();
    auto points3d = individuals_to_points(solution, ackley_simple);
    azgra::geometry::dump_3d_points_history(points3d, "tlbo_ackley.pts");

    // Fireflies
    FireflySolver fireflySolver(schwefelProb, 40);
    const auto solution = fireflySolver.solve();
    auto points3d = individuals_to_points(solution, schwefel);
    azgra::geometry::dump_3d_points_history(points3d, "firefly_schwefel.pts");
    */
    return 0;
}

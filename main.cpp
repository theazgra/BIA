#include <iostream>
#include "optimalization_functions_vis.h"
#include <azgra/cli/cli_arguments.h>
#include "tsp.h"
#include "DumpPermutations.h"
#include <azgra/utilities/Stopwatch.h>
#include <azgra/collection/vector_utilities.h>
#include "SOMASolver.h"

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
    const size_t dimension = 10;
 //   optimalization::OptimalizationProblem problem(griewank, 1, dimension, generate_limits(dimension, -600, 600));
    optimalization::OptimalizationProblem problem(rastrigin, 1, dimension, generate_limits(dimension, -5.12, 5.12));
    SOMASolver soma(problem, 150, 1000);
    soma.edit_parameters(2.1, 0.21, 0.8, 0.01);
    soma.solve();
//    using namespace azgra::collection;
//    Stopwatch stopwatch;
//    for (int permutationSize = 3; permutationSize <= 9; ++permutationSize)
//    {
//        std::vector<int> items(permutationSize);
//        for (int i = 0; i < permutationSize; ++i)
//        {
//            items[i] = i;
//        }
//        stopwatch.reset();
//        stopwatch.start();
//        size_t count = 0;
//        DumpPermutations<int> permGen(std::set<int>(items.begin(), items.end()));
//        do
//        {
//            const std::vector<int> &perm = permGen.get_current_permutation();
//            ++count;
//            //print_permutation(perm);
//        } while (permGen.next_permutation());
//        stopwatch.stop();
//        fprintf(stdout, "Permutation of size %i, generated %lu permutations in : took %.5f ms\n",
//                permutationSize,
//                count,
//                stopwatch.elapsed_milliseconds());
//    }
    return 0;
}

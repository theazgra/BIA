#include <iostream>
#include "optimalization_functions_vis.h"
#include "search_algorithms.h"
#include <azgra/cli/cli_arguments.h>
#include "tsp.h"

static void draw_cities(const tsp::TspSolution &solution,
                        const std::vector<azgra::geometry::Point2D<f64>> &points)
{

    using namespace azgra::geometry;
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
    Plot::animate_lines("tmp", linesInTime, 0.1);

}

static std::vector<azgra::geometry::Point2D<f64>> random_cities(size_t cityCount, f64 maxX, f64 maxY)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<f64> randomXDistribution(0, maxX);
    std::uniform_real_distribution<f64> randomYDistribution(0, maxY);

    std::vector<azgra::geometry::Point2D<f64>> cities(cityCount);
    for (size_t i = 0; i < cityCount; ++i)
    {
        cities.emplace_back(randomXDistribution(generator), randomYDistribution(generator));
    }
    return cities;
}


int main(int argc, char **argv)
{
#if 1
    using namespace azgra;
    using namespace azgra::geometry;
//    auto cities =
//            {
//                    Point2D<f64>(0, 0), Point2D<f64>(5, 5), Point2D<f64>(6, 6), Point2D<f64>(4, 8), Point2D<f64>(12, 13),
//                    Point2D<f64>(36, 45), Point2D<f64>(12, 27), Point2D<f64>(35, 41), Point2D<f64>(0, 10), Point2D<f64>(10, 50),
//                    Point2D<f64>(74, 63), Point2D<f64>(12, 15), Point2D<f64>(11, 99), Point2D<f64>(105, 13), Point2D<f64>(13, 105),
//                    Point2D<f64>(52, 36), Point2D<f64>(97, 96), Point2D<f64>(1, 31), Point2D<f64>(51, 81), Point2D<f64>(17, 88),
//            };
    auto cities = random_cities(50, 200, 200);
    tsp::TspSolver solver(cities, 100);
    auto solution = solver.solve(1000, 0.2);

    //draw_cities(solution, cities);

//    azgra::geometry::Plot("TSP")
//            .add_line({Point2D<f64>(5, 5), Point2D<f64>(10, 5)})
//            .add_line({Point2D<f64>(10, 5), Point2D<f64>(10, 10)})
//            .display_window();

    // 20 mest (2D body X,Y)
    // vzdalenost v matici sousednosti
    // dvoubodove krizeni ve 2 bodech ;1. cast z prvniho,3. cast z prvniho,2.cast z druheho pokud tam jeste nejsou
    // mutace s pravdepodobnosti, mutace je prohozeni 2 mest v jedinci
    // pro kazdeho aktualniho jedince vybereme nahodneho partnera a porovnavame potomka s aktualnim jedincem
    return 0;
#else
    namespace cli = azgra::cli;
    cli::CliArguments args("Biologically inspired algorithms",
                           "Find global minimum on functions from https://www.sfu.ca/~ssurjano/optimization.html");
    args.print_help_on_parser_error();

    cli::CliFlag ackleyFlag("Ackley function", "Ackley optimalization function", '\0', "ackley");
    cli::CliFlag griewankFlag("Griewank function", "Griewank optimalization function", '\0', "griewank");
    cli::CliFlag sphereFlag("Sphere function", "Sphere optimalization function", '\0', "sphere");
    cli::CliFlag schwefelFlag("Schwefel function", "Schwefel optimalization function", '\0', "schwefel");
    cli::CliFlag rastriginFlag("Rastrigin function", "Rastrigin optimalization function", '\0', "rastrigin");
    cli::CliFlag rosenbrockFlag("Rosenbrock function", "Rosenbrock optimalization function", '\0', "rosenbrock");
    cli::CliFlag michalewiczFlag("Michalewicz function", "Michalewicz optimalization function", '\0', "michalewicz");
    cli::CliFlag zakharovFlag("Zakharov function", "Zakharov optimalization function", '\0', "zakharov");
    cli::CliFlag powerSumFlag("Power-sum function", "Power-sum optimalization function", '\0', "power-sum");
    cli::CliFlag stblinskiTangFlag("Styblinski-Tang function", "Styblinski-Tang optimalization function", '\0', "styblinski-tang");
    cli::CliFlag dixonPriceFlag("Dixon-Price function", "Dixon-Price optimalization function", '\0', "dixon-price");

    cli::CliFlag displayResultFlag("Interactive window", "Display result in interactive window.", 'w', "window");
    cli::CliValueFlag<const char *> saveResultFlag("Save result", "Save result in png file.", 's', "save", false,
            "optimalizationResult.png");
    cli::CliValueFlag<size_t> iterationCountFlag("Iteration", "Number of iterations.", 'i', "iter", false, 1000);

    cli::CliFlagGroup resultPresentation("Result presentation", {&displayResultFlag, &saveResultFlag},
                                         cli::CliGroupMatchPolicy::CliGroupMatchPolicy_AtLeastOne);

    cli::CliFlagGroup optimizationFunctionsGroup("Optimization functions", {&ackleyFlag, &griewankFlag, &sphereFlag, &schwefelFlag,
                                                                            &rastriginFlag, &rosenbrockFlag, &michalewiczFlag,
                                                                            &zakharovFlag, &powerSumFlag, &stblinskiTangFlag,
                                                                            &dixonPriceFlag});

    cli::CliMethod blindSearchMethod("blind-search", "Blind search optimization", {&optimizationFunctionsGroup}, {&iterationCountFlag});
    cli::CliMethod hillClimbingMethod("hill-climb", "Hill climbing optimization", {&optimizationFunctionsGroup}, {&iterationCountFlag});
    cli::CliMethod simulatedAnnealingMethod("simulated-annealing", "Simulated annealing optimization", {&optimizationFunctionsGroup}, {});

    args.flags = {&iterationCountFlag};
    args.methods = {&blindSearchMethod, &hillClimbingMethod, &simulatedAnnealingMethod};
    args.add_group(optimizationFunctionsGroup);
    args.add_group(resultPresentation);

    if (!args.parse(argc, argv))
    {
        fprintf(stderr, "%s\n", args.get_error().c_str());
        return 1;
    }

    show_solution(iter, "schwefel-HC", schwefelHccResult);
    show_solution(iter, "schwefel-SA", schwefelSAResult);
//    fprintf(stdout, "After %lu iterations found best solution for %s with cost: %f. Solution: ",
//            iter, "Schwefel", schwefelHccResult.bestSolutionValue);
//    optimalization::print_solution(schwefelHccResult.bestSolution);
//
//    azgra::geometry::Plot("Hill climbing - schwefel", 1280, 720)
//            //.add_line(schwefelHccResult.solutionValueHistoryFor2D, "Function cost")
//            .add_line(schwefelHccResult.bestSolutionValueHistoryFor2D, "Best function cost so far")
//            .display_window();

    return 0;
#endif
}

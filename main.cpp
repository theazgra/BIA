#include <iostream>
#include "optimalization_functions_vis.h"
#include "search_algorithms.h"
#include <azgra/cli/cli_arguments.h>

int main(int argc, char **argv)
{
#if 0
    //visualize_function(-31,31,0.5,-31,31,0.5,ackley_simple,"plots/ackley.png");
    //visualize_function(-500, 500, 5.5, -500, 500, 5.5, griewank, "plots/griewank.png");
    //visualize_function(-5, 5, 0.25, -5, 5, 0.25, sphere, "plots/sphere.png");
    //visualize_function(-500,500,5.5,-500,500,5.5,schwefel,"plots/schwefel.png");
    //visualize_function(-5, 5, 0.25, -5, 5, 0.25, rastrigin, "plots/rastrigin.png");
    //visualize_function(-5, 10, 0.25, -5, 10, 0.25, rosenbrock, "plots/rosenbrock.png");
    //visualize_function(0.0, M_PI+0.1, 0.2, 0.0, M_PI+0.1, 0.2, michalewicz_simple, "plots/michalewicz.png");
    //visualize_function(-5, 10, 0.2, -5, 10, 0.2, zakharov, "plots/zakharov.png");
    //visualize_function(0, 3, 0.05, 0, 3, 0.05, power_sum_simple, "plots/power_sum.png");
    //visualize_function(-5, 5, 0.25, -5, 5, 0.25, styblinski_tang, "plots/styblinski_tang.png");
    //visualize_function(-10, 10, 0.25, -10, 10, 0.25, dixon_price, "plots/dixon_price.png");


//    const size_t iterationCount = 10000;
//    const char *FnName = "Ackley Function";
//
//    optimalization::OptimalizationProblem ackleyProblem(ackley_simple, iterationCount, 2, {
//            optimalization::Limits(-33, 33),
//            optimalization::Limits(-33, 33)});
//
//    optimalization::HillClimbingConfig ackleyHcc = {};
//    ackleyHcc.set_probem(ackleyProblem);
//    ackleyHcc.neighborhoodSize = 100;
//    ackleyHcc.stdDev = 0.9;
//
//    optimalization::SimulatedAnnealingConfig ackleySAc = {};
//    ackleySAc.set_probem(ackleyProblem);
//    ackleySAc.stdDev = 0.9;
//    ackleySAc.neighborhoodSize = 100;
//    ackleySAc.temperatureReductionFactor = 0.99;
//    ackleySAc.initialTemperature = 200;
//    ackleySAc.finalTemperature = 0.1;
//    ackleySAc.repetitionOfMetropolisAlg = 10;
//
//    optimalization::OptimalizationProblem prob(ackley_simple, iterationCount, 2, {
//            optimalization::Limits(-33, 33),
//            optimalization::Limits(-33, 33)});
//
////    optimalization::OptimalizationProblem prob(griewank, iterationCount, 2, {
////            optimalization::Limits(-600, 600),
////            optimalization::Limits(-600, 600)});
//
//    auto result = optimalization::blind_search_with_history(prob);
//    //auto result = optimalization::hill_climbing_with_history(ackleyHcc);
//    //auto result = optimalization::simulated_annealing_with_history(ackleySAc);
//


    optimalization::OptimalizationProblem styblinskiTangProblem(styblinski_tang, iterationCount, 2, {
            optimalization::Limits(-5, 5),
            optimalization::Limits(-5, 5)});

    optimalization::HillClimbingConfig ackleyHcc = {};
    ackleyHcc.set_probem(styblinskiTangProblem);
    ackleyHcc.neighborhoodSize = 100;
    ackleyHcc.stdDev = 0.9;

    optimalization::SimulatedAnnealingConfig ackleySAc = {};
    ackleySAc.set_probem(styblinskiTangProblem);
    ackleySAc.stdDev = 0.9;
    ackleySAc.temperatureReductionFactor = 0.99;
    ackleySAc.initialTemperature = 2000;
    ackleySAc.finalTemperature = 0.05;
    ackleySAc.repetitionOfMetropolisAlg = 1;

    size_t iter = 1000;
    optimalization::OptimalizationProblem schwefelProblem(sphere, iter, 10,
                                                          {optimalization::Limits(-5.12, 5.12),
                                                           optimalization::Limits(-5.12, 5.12),
                                                           optimalization::Limits(-5.12, 5.12),
                                                           optimalization::Limits(-5.12, 5.12),
                                                           optimalization::Limits(-5.12, 5.12),
                                                           optimalization::Limits(-5.12, 5.12),
                                                           optimalization::Limits(-5.12, 5.12),
                                                           optimalization::Limits(-5.12, 5.12),
                                                           optimalization::Limits(-5.12, 5.12),
                                                           optimalization::Limits(-5.12, 5.12)});

    auto result = optimalization::hill_climbing_with_history(ackleyHcc);
    //auto result = optimalization::simulated_annealing_with_history(ackleySAc);

    optimalization::HillClimbingConfig schwefelHCC = {};
    schwefelHCC.set_probem(schwefelProblem);
    schwefelHCC.stdDev = 1.0;
    schwefelHCC.neighborhoodSize = 50;

    azgra::geometry::Plot("Simulated Annealing Solution History", 1280, 720)
            .add_line(result.solutionValueHistoryFor2D, "Function cost")
            .add_line(result.bestSolutionValueHistoryFor2D, "Best function cost so far")
            .display_window();//("AckleyFunction.png");


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

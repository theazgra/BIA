#include <iostream>
#include "optimalization_functions_vis.h"
#include "search_algorithms.h"

int main()
{
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


    const size_t iterationCount = 10000;
    const char *FnName = "Ackley Function";

    optimalization::OptimalizationProblem ackleyProblem(ackley_simple, iterationCount, 2, {
            optimalization::Limits(-33, 33),
            optimalization::Limits(-33, 33)});

    optimalization::HillClimbingConfig ackleyHcc = {};
    ackleyHcc.set_probem(ackleyProblem);
    ackleyHcc.neighborhoodSize = 100;
    ackleyHcc.stdDev = 0.9;

    optimalization::SimulatedAnnealingConfig ackleySAc = {};
    ackleySAc.set_probem(ackleyProblem);
    ackleySAc.stdDev = 0.9;
    ackleySAc.neighborhoodSize = 100;
    ackleySAc.temperatureReductionFactor = 0.99;
    ackleySAc.initialTemperature = 1000;
    ackleySAc.finalTemperature = 0.1;
    ackleySAc.repetitionOfMetropolisAlg = 10;


    //auto result = optimalization::hill_climbing_with_history(ackleyHcc);
    auto result = optimalization::simulated_annealing_with_history(ackleySAc);

    fprintf(stdout, "After %lu iterations found best solution for %s with cost: %f. Solution: ",
            iterationCount, FnName, result.bestSolutionValue);
    optimalization::print_solution(result.bestSolution);

    azgra::geometry::Plot("Simulated Annealing Solution History", 1280, 720)
            .add_line(result.solutionValueHistoryFor2D, "Function cost")
            .add_line(result.bestSolutionValueHistoryFor2D, "Best function cost so far")
            .save("AckleyFunction.png");


    return 0;
}
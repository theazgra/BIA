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
//    auto result = search_algorithms::blind_search_2d_with_history(ackley_simple, iterationCount, {
//            search_algorithms::Limits(-33, 33),
//            search_algorithms::Limits(-33, 33)});

    auto result = search_algorithms::hill_climbing_2d_with_history(ackley_simple, iterationCount, {
                                                                           search_algorithms::Limits(-33, 33),
                                                                           search_algorithms::Limits(-33, 33)},
                                                                   100,
                                                                   0.9);

    fprintf(stdout, "After %lu iterations found best solution for %s with cost: %f. Solution: ",
            iterationCount, FnName, result.bestSolutionValue);
    search_algorithms::print_solution(result.bestSolution);

    azgra::geometry::Plot("Hill Climbing Solution History", 1920, 1080)
            .add_line(result.solutionValueHistoryFor2D, "Function cost")
            .add_line(result.bestSolutionValueHistoryFor2D, "Best function cost so far")
            .save("AckleyFunction.png");


    return 0;
}
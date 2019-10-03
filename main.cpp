#include <iostream>
#include "optimalization_functions_vis.h"

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
    visualize_function(-10, 10, 0.25, -10, 10, 0.25, dixon_price, "plots/dixon_price.png");
    return 0;
}
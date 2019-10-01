#include <iostream>
#include "optimalization_functions.h"
#include <azgra/azgra.h>
#include <azgra/geometry/plot.h>

int main()
{
    azgra::print_colorized(azgra::ConsoleColor::ConsoleColor_Red, "huhu\n");

    // sphere
    using namespace azgra::geometry;

    std::vector<std::vector<Point3D<double>>> coords;
    for (double x = -500.0; x < 500.1; x += 5.5)
    {
        std::vector<Point3D<double>> line;
        for (double y = -500.0; y < 500.1; y += 5.5)
        {
            double z = schwefel({x, y});
            line.push_back(Point3D<double>(x, y, z));
        }
        coords.push_back(line);
    }

    Plot("Schwefel function").add_surface_plot(coords).display_window();

    return 0;
}
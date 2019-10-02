#include "optimalization_functions_viz.h"

using namespace azgra;
using namespace azgra::geometry;

void visualize_function(const f64 xMin, const f64 xMax, const f64 xStep, const f64 yMin, const f64 yMax, const f64 yStep,
                        const std::function<f64(std::vector<f64>)> &fn,
                        azgra::basic_string_view__<char> plotFileName)
{
    std::vector<std::vector<Point3D<double>>> surfacePoints;
    for (double x = xMin; x < xMax; x += xStep)
    {
        std::vector<Point3D<double>> line;
        for (double y = yMin; y < yMax; y += yStep)
        {
            double z = fn({x, y});
            line.push_back(Point3D<double>(x, y, z));
        }
        surfacePoints.push_back(line);
    }

    Plot(plotFileName).add_surface_plot(surfacePoints).save(plotFileName);
}
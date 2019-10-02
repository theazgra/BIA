#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-flp30-c"
#pragma once

#include "optimalization_functions.h"
#include <azgra/geometry/plot.h>
#include <functional>

using namespace azgra;

void visualize_function(const f64 xMin, const f64 xMax, const f64 xStep, const f64 yMin, const f64 yMax, const f64 yStep,
                        const std::function<f64(std::vector<f64>)> &fn,
                        azgra::basic_string_view__<char> plotFileName);

#pragma clang diagnostic pop
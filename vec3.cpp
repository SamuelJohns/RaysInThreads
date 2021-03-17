#include "vec3.h"
#include <random>
#include <functional>

thread_local std::uniform_real_distribution<> dice_distribution(0.0, 1.0);
thread_local std::mt19937 random_number_engine; // pseudorandom number generator
thread_local auto dice_roller = std::bind(dice_distribution, random_number_engine);

double drand() {
    return dice_roller();
}

#include <benchmark/benchmark.h>

#include "impl_force_calc.hpp"

using namespace ImplForceCalc;

template <class LennardJonesImpl>
static void BM_LennardJonesCommon(benchmark::State& state) 
{
    LennardJonesImpl fc;
    
    const size_t x_size = 10;
    const size_t y_size = 10;
    const size_t n_iters = 1000;

    size_t size = x_size * y_size;

    std::vector<scalar_t> masses;
    std::vector<scalar_t> charges; 
    std::vector<Vec2>     positions; 
    std::vector<Vec2>     velocities;

    masses.resize(size, 1.0);
    charges.resize(size, 0.0);
    positions.resize(size, {0,0});
    velocities.resize(size, {0,0});

    for (size_t x = 0; x < x_size; x++)
    {
        for (size_t y = 0; y < y_size; y++)
        {
            positions[x * y_size + y] = {static_cast<double>(x), static_cast<double>(y)};
        }
    }

    ParticlesState particles{masses, charges, positions, velocities};
    Vec2List res;

    for (auto _ : state) {
        for (size_t i = 0; i < n_iters; i++)
            benchmark::DoNotOptimize(res = fc.compute_forces(particles));
    }
}

BENCHMARK(BM_LennardJonesCommon<LennardJonesOMPForceCalc>)->UseRealTime();
BENCHMARK(BM_LennardJonesCommon<LennardJonesThreadPoolForceCalc>)->UseRealTime();
BENCHMARK(BM_LennardJonesCommon<LennardJonesForceCalc>);

BENCHMARK_MAIN();
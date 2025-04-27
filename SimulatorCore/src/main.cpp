#include <iostream>

#include "impl_force_calc.hpp"

using namespace Simulation;
using namespace ImplForceCalc;
using namespace Geom;

int main()
{
    LennardJonesForceCalc           fc_lj;
    LennardJonesOMPForceCalc        fc_lj_OMP;
    LennardJonesThreadPoolForceCalc fc_lj_TP;

    size_t x_size = 3;
    size_t y_size = 3; 
    size_t size = x_size * y_size;

    std::vector<scalar_t> masses;
    std::vector<scalar_t> charges; 
    std::vector<Vec2>     positions; 
    std::vector<Vec2>     velocities;

    masses.resize(size, 1.0);
    charges.resize(size, 0.0);
    positions.resize(size, {0,0});
    velocities.resize(size, {0,0});

    for (size_t y = 0; y < y_size; y++)
    {
        for (size_t x = 0; x < x_size; x++)
        {
            positions[y * x_size + x] = {static_cast<double>(x), static_cast<double>(y)};
        }
    }

    ParticlesState particles{masses, charges, positions, velocities};

    Vec2List res     = fc_lj.compute_forces(particles);
    Vec2List res_OMP = fc_lj_OMP.compute_forces(particles);
    Vec2List res_TP  = fc_lj_TP.compute_forces(particles);

    auto print = [x_size, y_size](const Vec2List &list){

        for (size_t y = 0; y < y_size; y++)
        {
            for (size_t x = 0; x < x_size; x++)
            {
                Vec2 vec = list[y * x_size + x];
                std::cout << "(" << vec.x() << ", " << vec.y() << ") ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    };

    print(res);
    print(res_OMP);
    print(res_TP);
}
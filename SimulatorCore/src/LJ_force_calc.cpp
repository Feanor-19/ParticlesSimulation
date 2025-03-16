#include "LJ_force_calc.hpp"

#include <cmath>

namespace Simulation
{

ForcesList LennardJonesForceCalc::computeForces(const ParticlesStateView &particles) const
{
    ForcesList forces;
    forces.assign(particles.size(), {0,0});

    for (size_t i = 0; i < particles.size(); ++i)
    {
        for (size_t j = i + 1; j < particles.size(); ++j)
        {
            Vec2 r_vec = particles.pos(j) - particles.pos(i);
            scalar_t r = r_vec.len();
            if (Geom::eq(r, 0)) continue;
            scalar_t sr6 = std::pow(sigma_ / r, 6);
            Vec2 force_on_j_from_i = (24 * epsilon_ * (2*sr6*sr6 - sr6) / r) * r_vec.norm_vec();

            forces[j] += force_on_j_from_i;
            forces[i] += -force_on_j_from_i;
        }
    }

    return forces;
}

ForceCalcPtr Simulation::LennardJonesForceCalc::clone() const
{
    return std::make_unique<LennardJonesForceCalc>(*this);
}
    
} // namespace Simulation



#pragma once

#include "simulator.hpp"

namespace Simulation
{

class LennardJonesForceCalc : public Simulation::ForceCalculator
{
private:
    scalar_t epsilon_;
    scalar_t sigma_;

    // TODO такое нужно будет в async версии, тут это не нужно 
    // void computeForcesRange(const ParticlesStateView& particles,
    //                         Vec2List &forces_out,
    //                         size_t start, size_t end) const;
                            
public:
    LennardJonesForceCalc(scalar_t epsilon, scalar_t sigma) : epsilon_(epsilon), sigma_(sigma) {}

    Vec2List compute_forces(const ParticlesStateView& particles) const override;
    
    ForceCalcPtr clone() const override;
};
} // namespace Simulation
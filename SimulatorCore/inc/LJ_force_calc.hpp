#pragma once

#include "simulator.hpp"

namespace Simulation
{

class LennardJonesForceCalc : public Simulation::ForceCalculator
{
private:
    scalar_t epsilon_;
    scalar_t sigma_;

    const std::string name_{"Lennard-Jones"};
    const std::vector<std::string> param_names_ =
    {
        "Epsilon",
        "Sigma"
    };

    // TODO такое нужно будет в async версии, тут это не нужно 
    // void computeForcesRange(const ParticlesStateView& particles,
    //                         Vec2List &forces_out,
    //                         size_t start, size_t end) const;
                            
public:
    LennardJonesForceCalc(scalar_t epsilon = 1.0, scalar_t sigma = 1.0) : epsilon_(epsilon), sigma_(sigma) {}

    Vec2List compute_forces(const ParticlesStateView& particles) const override;
    std::string get_name() const override { return name_; }
    std::vector<ImplParam> get_params() const override;
    void set_params(const std::vector<scalar_t> &params_values) override;
    ForceCalcPtr clone() const override;
};
} // namespace Simulation
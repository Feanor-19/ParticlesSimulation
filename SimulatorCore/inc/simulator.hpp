#pragma once

#include "particles_state.hpp"

#include <vector>
#include <memory>

namespace Simulation
{

class ForceCalculator;
class Integrator;

using Vec2List      = std::vector<Vec2>;
using ForceCalcPtr  = std::unique_ptr<ForceCalculator>;
using IntegratorPtr = std::unique_ptr<Integrator>;

class ForceCalculator 
{
public:
    virtual ~ForceCalculator() = default;
    
    virtual Vec2List compute_forces(const ParticlesStateView& particles) const = 0;
    
    virtual ForceCalcPtr clone() const = 0;
};

class Integrator 
{
public:
    virtual ~Integrator() = default;
    
    virtual void integrate(ParticlesStateView& particles, 
                           const ForceCalculator& force_calc, 
                           scalar_t dt) = 0;
    
    virtual IntegratorPtr clone() const = 0;
};

class Simulator
{
private:
    ParticlesState particles_;
    IntegratorPtr  integrator_;
    ForceCalcPtr   force_calc_;
public:
    Simulator(IntegratorPtr integrator, ForceCalcPtr force_calc)
        : integrator_(std::move(integrator)), force_calc_(std::move(force_calc)) {};

    void add_particle(Particle particle);
    void remove_particle(size_t index); // TODO подумать
    
    void step(scalar_t dt);
    
    const ParticlesStateView& particles() const {return particles_;};

    Simulator(const Simulator&) = delete;
    Simulator& operator=(const Simulator&) = delete;
};

} // namespace Simulation
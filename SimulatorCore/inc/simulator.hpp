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

struct ImplParam
{
    std::string name;
    scalar_t value;
};

class ForceCalculator 
{
public:
    virtual ~ForceCalculator() = default;
    
    virtual Vec2List compute_forces(const ParticlesStateView& particles) const = 0;
    virtual std::vector<ImplParam> get_params() const = 0;
    virtual void set_params(const std::vector<scalar_t> &params_values) = 0;
    virtual ForceCalcPtr clone() const = 0;
};

class Integrator 
{
public:
    virtual ~Integrator() = default;
    
    virtual void integrate(ParticlesStateView& particles, 
                           const ForceCalculator& force_calc, 
                           scalar_t dt) = 0;
    virtual std::vector<ImplParam> get_params() const = 0;
    virtual void set_params(const std::vector<scalar_t> &params_values) = 0;
    virtual IntegratorPtr clone() const = 0;
};

class MockForceCalculator : public ForceCalculator
{
public:
    
    Vec2List compute_forces(const ParticlesStateView& particles) const override
    {
        Vec2List list;
        list.assign(particles.size(), {0, 0});
        return list;
    }

    std::vector<ImplParam> get_params() const override { return {}; }
    void set_params(const std::vector<scalar_t> &params_values) override {}
    ForceCalcPtr clone() const override { return std::make_unique<MockForceCalculator>(*this); }
    
    static std::string get_name() { return "Mock Force Calculator"; }
};

class MockIntegrator : public Integrator
{
    void integrate(ParticlesStateView& particles, 
        const ForceCalculator& force_calc, 
        scalar_t dt) override {}
    
    std::vector<ImplParam> get_params() const override { return {}; }
    void set_params(const std::vector<scalar_t> &params_values) override {}
    IntegratorPtr clone() const override { return std::make_unique<MockIntegrator>(*this); };
    
    static std::string get_name() { return "Mock Integrator"; }
};

class Simulator
{
private:
    ParticlesState particles_;
    IntegratorPtr  integrator_;
    ForceCalcPtr   force_calc_;
public:
    Simulator()
        : integrator_(std::make_unique<MockIntegrator>()), 
          force_calc_(std::make_unique<MockForceCalculator>()) {}

    Simulator(IntegratorPtr integrator, ForceCalcPtr force_calc)
        : integrator_(std::move(integrator)), force_calc_(std::move(force_calc)) {}

    void push_back_particle(Particle particle);
    void remove_particle(size_t index);
    
    void step(scalar_t dt);
    
    const ParticlesStateView& particles() const {return particles_;}
    const IntegratorPtr& integrator() const { return integrator_; }
    const ForceCalcPtr&  force_calc() const { return force_calc_; }

    void change_integrator(IntegratorPtr new_integrator) { integrator_ = std::move(new_integrator); }
    void change_force_calc(ForceCalcPtr  new_force_calc) { force_calc_ = std::move(new_force_calc); }

    void load_particles(ParticlesState&& new_particles);

    Simulator(const Simulator&) = delete;
    Simulator& operator=(const Simulator&) = delete;
};

} // namespace Simulation
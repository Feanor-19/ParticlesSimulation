#pragma once

#include "simulator.hpp"

#include "thread_pool.hpp"

namespace ImplForceCalc
{

using namespace Simulation;

class LennardJonesForceCalc : public ForceCalculator
{
protected:
    scalar_t epsilon_;
    scalar_t sigma_;

    const std::vector<std::string> param_names_ =
    {
        "Epsilon",
        "Sigma"
    };
                            
public:
    LennardJonesForceCalc(scalar_t epsilon = 1.0, scalar_t sigma = 1.0) : epsilon_(epsilon), sigma_(sigma) {}

    Vec2List compute_forces(const ParticlesStateView& particles) override;
    std::vector<ImplParam> get_params() const override;
    void set_params(const std::vector<scalar_t> &params_values) override;
    ForceCalcPtr clone() const override;
    
    static std::string get_name() { return "Lennard-Jones"; }
};

class LennardJonesOMPForceCalc : public LennardJonesForceCalc
{                    
public:
    LennardJonesOMPForceCalc(scalar_t epsilon = 1.0, scalar_t sigma = 1.0);

    Vec2List compute_forces(const ParticlesStateView& particles) override;
    ForceCalcPtr clone() const override;
    
    static std::string get_name() { return "Lennard-Jones OpenMP"; }
};

class LennardJonesThreadPoolForceCalc : public LennardJonesForceCalc
{
private:
    ThreadPool pool_;

    Vec2 compute_force(const ParticlesStateView &particles, size_t i);
public:
    LennardJonesThreadPoolForceCalc(scalar_t epsilon = 1.0, scalar_t sigma = 1.0);

    Vec2List compute_forces(const ParticlesStateView& particles) override;
    ForceCalcPtr clone() const override;
    
    static std::string get_name() { return "Lennard-Jones Thread Pool"; }
};

class HookeCentralForceCalc : public ForceCalculator
{
private:
    scalar_t spring_const_;
    scalar_t center_x_;
    scalar_t center_y_;

    const std::vector<std::string> param_names_ = 
    {
        "Spring constant",
        "Center X",
        "Center Y"
    };

public:
    HookeCentralForceCalc(scalar_t spring_const = 1.0, 
                          scalar_t center_x = 0.0, 
                          scalar_t center_y = 0.0)
        : spring_const_(spring_const), center_x_(center_x), center_y_(center_y) {}

    Vec2List compute_forces(const ParticlesStateView& particles) override;
    std::vector<ImplParam> get_params() const override;
    void set_params(const std::vector<scalar_t> &params_values) override;
    ForceCalcPtr clone() const override;
    
    static std::string get_name() { return "Hooke Central"; }    
};

class HookeAmongForceCalc : public ForceCalculator
{
private:
    scalar_t spring_const_;
    scalar_t unstretched_len_;

    const std::vector<std::string> param_names_ = 
    {
        "Spring constant",
        "Unstretched Length"
    };

public:
    HookeAmongForceCalc(scalar_t spring_const = 1.0, scalar_t unstretched_len = 1.0)
        : spring_const_(spring_const), unstretched_len_(unstretched_len) {}

    Vec2List compute_forces(const ParticlesStateView& particles) override;
    std::vector<ImplParam> get_params() const override;
    void set_params(const std::vector<scalar_t> &params_values) override;
    ForceCalcPtr clone() const override;
    
    static std::string get_name() { return "Hooke Among"; }  
};

} // namespace ImplForceCalc
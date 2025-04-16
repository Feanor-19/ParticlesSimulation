#pragma once

#include "simulator.hpp"

namespace Simulation
{

class RungeKutta4Integrator : public Integrator
{
private:
    const std::string name_{"Runge-Kutta 4"};

    // -- helpers for "integrate"
    ParticlesState tmpFC;
    
    // kinda named std::pair
    struct pos_vel
    {
        std::vector<Vec2> pos;
        std::vector<Vec2> vel;
    };

    pos_vel old, tmp1, tmp2;

    // state += k * scalar
    void apply_state_change(ParticlesStateView &state, const pos_vel& k, scalar_t scalar);

    // state_res = a + b * scalar
    void compute_state_comb(ParticlesStateView &state_res, const pos_vel& a,
                            const pos_vel& b, scalar_t scalar);

    void make_step( ParticlesStateView &particles, const ForceCalculator &force_calc,
                    pos_vel& k_prev, pos_vel& k_next, scalar_t dt, scalar_t coeff);
public:
    void integrate(ParticlesStateView& particles, 
                   const ForceCalculator& force_calc, 
                   scalar_t dt) override;
    std::string get_name() const override { return name_; }
    std::vector<ImplParam> get_params() const override;
    void set_params(const std::vector<scalar_t> &params_values) override;
    IntegratorPtr clone() const override;
};
    
} // namespace Simulation

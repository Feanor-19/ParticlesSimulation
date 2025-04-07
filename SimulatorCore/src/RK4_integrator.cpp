#include "RK4_integrator.hpp"

namespace Simulation
{
// TODO
// void RungeKutta4Integrator::prepare_all_tmps(size_t new_size)
// {
//     tmpFC.resize(new_size);

//     old.pos.resize(new_size, {0,0});
//     old.vel.resize(new_size, {0,0});

//     tmp1.pos.resize(new_size, {0,0});
//     tmp1.vel.resize(new_size, {0,0});

//     tmp2.pos.resize(new_size, {0,0});
//     tmp2.vel.resize(new_size, {0,0});
// }

inline Vec2List compute_acc(const ForceCalculator &force_calc, const ParticlesStateView &state)
{
    auto forces = force_calc.compute_forces(state);
    
    Vec2List accelerations = forces;
    for (size_t ind = 0; ind < state.size(); ++ind)
        accelerations[ind] *= 1.0 / state.mass(ind);

    return accelerations;
}

void RungeKutta4Integrator::apply_state_change(ParticlesStateView &state, const pos_vel& k, scalar_t scalar)
{
    for (size_t ind = 0; ind < state.size(); ++ind)
    {
        state.pos(ind) += k.pos[ind] * scalar;
        state.vel(ind) += k.vel[ind] * scalar;
    }
}

void RungeKutta4Integrator::compute_state_comb( ParticlesStateView &state_res,
                                                const pos_vel& a,
                                                const pos_vel& b, 
                                                scalar_t scalar)
{
    for (size_t ind = 0; ind < state_res.size(); ++ind)
    {
        state_res.pos(ind) = a.pos[ind] + b.pos[ind] * scalar;
        state_res.vel(ind) = a.vel[ind] + b.vel[ind] * scalar;
    }
}

void RungeKutta4Integrator::make_step(ParticlesStateView &particles, const ForceCalculator &force_calc, 
                                      pos_vel &k_prev, pos_vel &k_next, scalar_t dt, scalar_t coeff)
{
    compute_state_comb(tmpFC, old, k_prev, dt / coeff);

    k_next.pos = tmpFC.velocities();
    k_next.vel = compute_acc(force_calc, tmpFC);

    apply_state_change(particles, k_next, dt * coeff / 6);
}

void RungeKutta4Integrator::integrate(ParticlesStateView &particles, 
                                      const ForceCalculator &force_calc, 
                                      scalar_t dt)
{
    //prepare_all_tmps(particles.size()); // TODO

    old.pos = particles.positions();
    old.vel = particles.velocities();

    tmpFC = particles;

    // --- step 1
    tmp1.pos = old.vel;
    tmp1.vel = compute_acc(force_calc, particles);

    apply_state_change(particles, tmp1, dt/6);

    // --- steps 2, 3, 4
    make_step(particles, force_calc, tmp1, tmp2, dt, 2);

    make_step(particles, force_calc, tmp2, tmp1, dt, 2);

    make_step(particles, force_calc, tmp1, tmp2, dt, 1);
}

IntegratorPtr RungeKutta4Integrator::clone() const
{
    return std::make_unique<RungeKutta4Integrator>(*this);
}

} // namespace Simulation

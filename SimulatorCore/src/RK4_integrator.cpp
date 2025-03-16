#include "RK4_integrator.hpp"

namespace Simulation
{
void RungeKutta4Integrator::reserve_all_tmps(size_t new_cap)
{
    tmpFC.reserve(new_cap);

    old.pos.reserve(new_cap);
    old.vel.reserve(new_cap);
    
    tmp1.pos.reserve(new_cap);
    tmp1.vel.reserve(new_cap);

    tmp2.pos.reserve(new_cap);
    tmp2.vel.reserve(new_cap);
}

inline std::vector<Vec2> compute_acc(const ForceCalculator &force_calc, const ParticlesStateView &state)
{
    auto forces = force_calc.computeForces(state);
    
    std::vector<Vec2> accelerations = forces;
    for (size_t ind = 0; ind < state.size(); ++ind)
        accelerations[ind] *= 1 / state.mass(ind);

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
    reserve_all_tmps(particles.size());

    old.pos = particles.positions();
    old.vel = particles.velocities();

    // --- step 1
    tmp1.pos = old.vel;
    tmp1.vel = compute_acc(force_calc, particles);

    apply_state_change(particles, tmp1, dt/6);

    // --- step 2 // TODO удалить лишнее после проверки
    // compute_state_comb(tmpFC, old, tmp1, dt/2);

    // tmp2.pos = tmpFC.velocities();
    // tmp2.vel = compute_acc(force_calc, tmpFC);

    // apply_state_change(particles, tmp2, dt/3);

    make_step(particles, force_calc, tmp1, tmp2, dt, 2);

    // --- step 3
    // compute_state_comb(tmpFC, old, tmp2, dt/2);

    // tmp1.pos = tmpFC.velocities();
    // tmp1.vel = compute_acc(force_calc, tmpFC);

    // apply_state_change(particles, tmp1, dt/3);

    make_step(particles, force_calc, tmp2, tmp1, dt, 2);

    //--- step 4
    // compute_state_comb(tmpFC, old, tmp1, dt);

    // tmp2.pos = tmpFC.velocities();
    // tmp2.vel = compute_acc(force_calc, tmpFC);

    // apply_state_change(particles, tmp2, dt/6);

    make_step(particles, force_calc, tmp1, tmp2, dt, 1);
}

IntegratorPtr RungeKutta4Integrator::clone() const
{
    return std::make_unique<RungeKutta4Integrator>(*this);
}

} // namespace Simulation

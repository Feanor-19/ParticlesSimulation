#include <iostream>

#include "RK4_integrator.hpp"

int main()
{
    using namespace Simulation;

    RungeKutta4Integrator integrator{};

    class ZeroForce : public ForceCalculator
    {
    public:
        ZeroForce() {}
        Vec2List compute_forces(const ParticlesStateView& particles) const override
        {
            return Vec2List{particles.size(), {0,0}};
        }

        ForceCalcPtr clone() const override {return std::make_unique<ZeroForce>(*this);}
    } force_calc;

    Vec2 init_pos = {0, 0};
    Vec2 init_vel = {1,0};
    ParticlesState state{{1}, Vec2List{init_pos}, Vec2List{init_vel}};

    const double dt = 1.0;
    integrator.integrate(state, force_calc, dt);

    std::cout << (state.pos(0) == init_pos + init_vel*dt) << std::endl;
    std::cout << (state.vel(0) == init_vel) << std::endl;
}
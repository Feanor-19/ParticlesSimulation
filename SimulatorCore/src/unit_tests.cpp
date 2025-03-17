#include <gtest/gtest.h>

#include "LJ_force_calc.hpp"
#include "RK4_integrator.hpp"

using namespace Simulation;
using namespace Geom;

TEST(Vec2, NormVec)
{
    Vec2 v0 = Vec2{0, 0}.norm_vec();
    EXPECT_DOUBLE_EQ(v0.x(), 0);
    EXPECT_DOUBLE_EQ(v0.y(), 0);

    Vec2 v1 = Vec2{42, 0}.norm_vec();
    EXPECT_DOUBLE_EQ(v1.x(), 1);
    EXPECT_DOUBLE_EQ(v1.y(), 0);

    Vec2 v2 = Vec2{0, 42}.norm_vec();
    EXPECT_DOUBLE_EQ(v2.x(), 0);
    EXPECT_DOUBLE_EQ(v2.y(), 1);

    Vec2 v3 = Vec2{42, 42}.norm_vec();
    EXPECT_DOUBLE_EQ(v3.x(), 1.0 / sqrt(2.0));
    EXPECT_DOUBLE_EQ(v3.y(), 1.0 / sqrt(2.0));
}

TEST(Vec2, BinPlus)
{
    Vec2 p{1, 2};
    Vec2 q{-1, -2};

    EXPECT_TRUE((p+q == Vec2{0,0}));

    Vec2 v0{19, -23};
    Vec2 v1{-42, -25};

    Vec2 v3 = v0;
    v3 += v1;
    EXPECT_TRUE((v0 + v1 == v3));
}

TEST(Vec2, MulScalar)
{
    EXPECT_TRUE((Vec2{1,2}*42 == 42*Vec2{1,2}));

    EXPECT_DOUBLE_EQ((Vec2{1,2}*0).len(), 0);

    EXPECT_TRUE((3*Vec2{1,2} == Vec2{3,6}));
}

TEST(Vec2, Len)
{
    double a = 42;
    Vec2 v1 = {a, 0};
    EXPECT_DOUBLE_EQ(v1.len(), a);

    Vec2 v2 = {0, a};
    EXPECT_DOUBLE_EQ(v2.len(), a);
    
    Vec2 v3 = {-a, a};
    EXPECT_DOUBLE_EQ(v3.len(), sqrt(2)*a);
}

TEST(Vec2, UnMinus)
{
    Vec2 v1{1, 2};
    Vec2 v2 = -v1;
    EXPECT_DOUBLE_EQ(v1.x(), -v2.x());
    EXPECT_DOUBLE_EQ(v1.y(), -v2.y());
}

TEST(LennardJonesForceCalc, ZeroForceAtEquilibrium)
{
    scalar_t epsilon = 1, sigma = 1;
    LennardJonesForceCalc force_calc{epsilon, sigma};

    Vec2List positions = {{0,0}, {pow(2, 1.0/6 * sigma), 0}};
    Vec2List velocities = {{0,0}, {0,0}};
    ParticlesState state{{1, 1}, positions, velocities};

    auto forces = force_calc.computeForces(state);

    EXPECT_NEAR(forces[0].x(), 0.0, 1e-9);
    EXPECT_NEAR(forces[1].x(), 0.0, 1e-9);

    EXPECT_DOUBLE_EQ(forces[0].y(), 0);
    EXPECT_DOUBLE_EQ(forces[1].y(), 0);
}

TEST(LennardJonesForceCalc, NewtonThirdLaw)
{
    scalar_t epsilon = 1, sigma = 1;
    LennardJonesForceCalc force_calc{epsilon, sigma};

    Vec2List positions = {{0,0}, {2 * sigma, -3*sigma}};
    Vec2List velocities = {{0,0}, {0,0}};
    ParticlesState state{{1, 1}, positions, velocities};

    auto forces = force_calc.computeForces(state);

    EXPECT_DOUBLE_EQ(forces[0].x(), -forces[1].x());
    EXPECT_DOUBLE_EQ(forces[0].y(), -forces[1].y());
}

TEST(LennardJonesForceCalc, EmptyParticlesState)
{
    scalar_t epsilon = 1, sigma = 1;
    LennardJonesForceCalc force_calc{epsilon, sigma};

    ParticlesState state{};

    auto forces = force_calc.computeForces(state);

    EXPECT_TRUE(forces.empty());
}

TEST(RungeKutta4Integrator, FreeMotion)
{
    RungeKutta4Integrator integrator{};

    class ZeroForce : public ForceCalculator
    {
    public:
        ZeroForce() {}
        Vec2List computeForces(const ParticlesStateView& particles) const override
        {
            return Vec2List{particles.size(), {0,0}};
        }

        ForceCalcPtr clone() const override {return std::make_unique<ZeroForce>(*this);}
    } force_calc;

    Vec2 init_pos = {-19, 4};
    Vec2 init_vel = {7,-5};
    ParticlesState state{{1}, Vec2List{init_pos}, Vec2List{init_vel}};

    const double dt = 1.0;
    integrator.integrate(state, force_calc, dt);

    EXPECT_TRUE((state.pos(0) == init_pos + init_vel*dt));
    EXPECT_TRUE((state.vel(0) == init_vel));
}

TEST(RungeKutta4Integrator, ConstantAcceleration)
{
    RungeKutta4Integrator integrator{};

    class ConstForce : public ForceCalculator
    {
    public:
        const Vec2 force = {2, 3};
        
        ConstForce() {}
        Vec2List computeForces(const ParticlesStateView& particles) const override
        {
            return Vec2List{particles.size(), force};
        }

        ForceCalcPtr clone() const override {return std::make_unique<ConstForce>(*this);}
    } force_calc;

    Vec2 init_pos = {-19, 4};
    Vec2 init_vel = {7,-5};
    scalar_t mass = 1;
    ParticlesState state{{mass}, Vec2List{init_pos}, Vec2List{init_vel}};

    const double dt = 1.0;
    integrator.integrate(state, force_calc, dt);

    Vec2 acc = force_calc.force * (1.0 / mass);
    EXPECT_TRUE((state.pos(0) == init_pos + init_vel*dt + 0.5*acc*dt*dt));
    EXPECT_TRUE((state.vel(0) == init_vel + acc*dt));
}

TEST(RungeKutta4Integrator, HarmonicOscillatorEnergyConservation)
{
    RungeKutta4Integrator integrator{};

    class HookeForce : public ForceCalculator
    {
    public:
        scalar_t k = 3.0;
        HookeForce() {}
        Vec2List computeForces(const ParticlesStateView& particles) const override
        {
            Vec2List forces = {particles.size(), {0,0}};
            for (size_t ind = 0; ind < particles.size(); ++ind)
                forces[ind] = -k * particles.pos(ind);

            return forces;
        }

        ForceCalcPtr clone() const override {return std::make_unique<HookeForce>(*this);}
    } force_calc;

    Vec2 init_pos = {-19, 4};
    Vec2 init_vel = {7,-5};
    scalar_t mass = 1;
    ParticlesState state{{mass}, Vec2List{init_pos}, Vec2List{init_vel}};

    const scalar_t dt = 1e-10;
    const size_t steps = 1000;
    integrator.integrate(state, force_calc, dt);

    const scalar_t init_energy = 0.5 * force_calc.k * init_pos.len() * init_pos.len() 
                               + 0.5 * mass * init_vel.len() * init_vel.len();

    const scalar_t final_energy = 0.5 * force_calc.k * state.pos(0).len() * state.pos(0).len()
                                + 0.5 * mass * state.vel(0).len() * state.vel(0).len();

    EXPECT_NEAR(init_energy, final_energy, 1e-8);
}
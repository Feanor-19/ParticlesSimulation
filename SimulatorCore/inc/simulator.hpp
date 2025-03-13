#pragma once

#include "particle.hpp"

#include <vector>
#include <memory>

namespace Simulation
{

class ForceCalculator 
{
public:
    virtual ~ForceCalculator() = default;
    
    virtual void computeForces(const std::vector<Particle>& particles, 
                               std::vector<Vec2>& forces) const = 0;
    
    virtual std::unique_ptr<ForceCalculator> clone() const = 0;
};

class Integrator 
{
public:
    virtual ~Integrator() = default;
    
    virtual void integrate(std::vector<Particle>& particles, 
                           const ForceCalculator& force,
                           scalar_t dt) = 0;
    
    virtual std::unique_ptr<Integrator> clone() const = 0;
};

class Simulator
{
private:
    std::vector<Particle> particles_;
    std::unique_ptr<Integrator> integrator_;
    std::unique_ptr<ForceCalculator> force_;
public:
    Simulator(std::unique_ptr<Integrator> integrator,
                std::unique_ptr<ForceCalculator> force);

    // void addParticle(const Particle& p);
    // void removeParticle(size_t index);
    
    void step(double dt);
    
    const std::vector<Particle>& particles() const;
};

} // namespace Simulation
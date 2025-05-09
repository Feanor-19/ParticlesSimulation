#pragma once

#include "utilities.hpp"

#include <vector>
#include <cassert>
#include <stdexcept>

namespace Simulation
{

using Geom::Vec2;
using Geom::scalar_t;

// doesn't allow adding and removing particles, doesn't own the state
class ParticlesStateView {
protected:
    const std::vector<scalar_t> &masses_;
    const std::vector<scalar_t> &charges_;
    std::vector<Vec2>           &positions_;
    std::vector<Vec2>           &velocities_;

    ParticlesStateView(std::vector<scalar_t> &masses,
                       std::vector<scalar_t> &charges, 
                       std::vector<Vec2>     &positions, 
                       std::vector<Vec2>     &velocities)
        : masses_(masses), charges_(charges), positions_(positions), velocities_(velocities) {}
public:
    size_t size() const noexcept { return masses_.size(); }
    bool empty() const noexcept { return masses_.empty(); }

    const scalar_t &mass(size_t index) const;
    
    const scalar_t &charge(size_t index) const; 

    const Vec2 &pos(size_t index) const;
    Vec2 &pos(size_t index);

    const Vec2 &vel(size_t index) const;
    Vec2 &vel(size_t index);

    const std::vector<scalar_t> &masses()     const noexcept { return masses_; }
    const std::vector<scalar_t> &charges()    const noexcept { return charges_; }
    const std::vector<Vec2>     &positions()  const noexcept { return positions_; }
    const std::vector<Vec2>     &velocities() const noexcept { return velocities_; }

    ParticlesStateView() = delete;
    ParticlesStateView(const ParticlesStateView&) = delete;
    ParticlesStateView(ParticlesStateView&&) = delete;
    ParticlesStateView& operator=(const ParticlesStateView&) = delete;
    ParticlesStateView& operator=(ParticlesStateView&&) = delete;
    ~ParticlesStateView() = default;
};

struct Particle
{
    scalar_t mass_;   // particle's mass
    scalar_t charge_; // in the most general sense
    Vec2     pos_;    // particle's initial position
    Vec2     vel_;    // particle's initial velocity
};

// cache-friendly, owns the state
class ParticlesState : public ParticlesStateView {
private:
    std::vector<scalar_t> masses_;
    std::vector<scalar_t> charges_;
    std::vector<Vec2>     positions_;
    std::vector<Vec2>     velocities_;

    void debug_check_invariants() const;
public:
    ParticlesState() : ParticlesStateView(masses_, charges_, positions_, velocities_) {}
    ParticlesState(std::vector<scalar_t> masses,
                   std::vector<scalar_t> charges, 
                   std::vector<Vec2>     positions, 
                   std::vector<Vec2>     velocities);
    
    ParticlesState(const ParticlesState& other);
    ParticlesState(ParticlesState&& other);
    ParticlesState& operator=(const ParticlesState& rhs);
    ParticlesState& operator=(ParticlesState&& rhs); 

    void operator=(const ParticlesStateView& view);
    
    void push_back_particle(Particle particle);
    void remove_particle(size_t index);

    void reserve(size_t new_capacity);
    void clear() noexcept;
};

} // namespace Simulation
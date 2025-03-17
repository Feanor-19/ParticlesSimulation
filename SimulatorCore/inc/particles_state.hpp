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

    std::vector<Vec2> &positions_;
    std::vector<Vec2> &velocities_;

    ParticlesStateView(std::vector<scalar_t> &masses, 
                       std::vector<Vec2>     &positions, 
                       std::vector<Vec2>     &velocities)
        : masses_(masses), positions_(positions), velocities_(velocities) {}
public:
    size_t size() const noexcept { return masses_.size(); }
    bool empty() const noexcept { return masses_.empty(); }

    const scalar_t &mass(size_t index) const;

    const Vec2 &pos(size_t index) const;
    Vec2 &pos(size_t index);

    const Vec2 &vel(size_t index) const;
    Vec2 &vel(size_t index);

    const std::vector<scalar_t> &masses()     const noexcept { return masses_; }
    const std::vector<Vec2>     &positions()  const noexcept { return positions_; }
    const std::vector<Vec2>     &velocities() const noexcept { return velocities_; }
};

// cache-friendly, owns the state
class ParticlesState : public ParticlesStateView {
private:
    std::vector<scalar_t> masses_;
    std::vector<Vec2>     positions_;
    std::vector<Vec2>     velocities_;

    void check_invariants() const;
public:
    ParticlesState() : ParticlesStateView(masses_, positions_, velocities_) {}
    ParticlesState(std::vector<scalar_t> masses, 
                   std::vector<Vec2>     positions, 
                   std::vector<Vec2>     velocities);
    
    void add_particle(scalar_t mass, Vec2 pos, Vec2 vel);
    void remove_particle(size_t index);

    void resize(size_t new_size);
    void reserve(size_t new_capacity);
    void clear() noexcept;

    void operator=(const ParticlesStateView& view);
};

} // namespace Simulation
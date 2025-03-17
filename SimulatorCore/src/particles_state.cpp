#include "particles_state.hpp"

namespace Simulation
{

namespace EXCEPTION_MSG
{
constexpr std::string_view OUT_OF_RANGE = "Particle index out of range";
} // namespace EXCEPTION_MSG

void ParticlesState::check_invariants() const
{
    assert(masses_.size() == positions_.size() 
        && positions_.size() == velocities_.size());
}

ParticlesState::ParticlesState(std::vector<scalar_t> masses, 
                               std::vector<Vec2>     positions, 
                               std::vector<Vec2>     velocities)
    : ParticlesStateView(masses_, positions_, velocities_),
      masses_    (std::move(masses)), 
      positions_ (std::move(positions)), 
      velocities_(std::move(velocities))
{
    if (!(masses_.size() == positions_.size() && positions_.size() == velocities_.size())) {
        throw std::invalid_argument("Initialization vectors size mismatch");
    }
    check_invariants();
}

void ParticlesState::add_particle(scalar_t mass, Vec2 pos, Vec2 vel)
{
    masses_.push_back(mass);
    positions_.push_back(pos);
    velocities_.push_back(vel);
    check_invariants();
}

void ParticlesState::remove_particle(size_t index)
{
    if (index >= size()) {
        throw std::out_of_range(EXCEPTION_MSG::OUT_OF_RANGE.data());
    }
    masses_.erase(masses_.begin() + index);
    positions_.erase(positions_.begin() + index);
    velocities_.erase(velocities_.begin() + index);
    check_invariants();
}

const scalar_t &ParticlesStateView::mass(size_t index) const
{
    if (index >= size()) throw std::out_of_range(EXCEPTION_MSG::OUT_OF_RANGE.data());
    return masses_[index];
}

const Vec2 &ParticlesStateView::pos(size_t index) const
{
    if (index >= size()) throw std::out_of_range(EXCEPTION_MSG::OUT_OF_RANGE.data());
    return positions_[index];
}

Vec2& ParticlesStateView::pos(size_t index)
{
    if (index >= size()) throw std::out_of_range(EXCEPTION_MSG::OUT_OF_RANGE.data());
    return positions_[index];
}

const Vec2& ParticlesStateView::vel(size_t index) const {
    if (index >= size()) throw std::out_of_range(EXCEPTION_MSG::OUT_OF_RANGE.data());
    return velocities_[index];
}

Vec2& ParticlesStateView::vel(size_t index) {
    if (index >= size()) throw std::out_of_range(EXCEPTION_MSG::OUT_OF_RANGE.data());
    return velocities_[index];
}

void ParticlesState::resize(size_t new_size)
{
    masses_.resize(new_size, 0.0);
    positions_.resize(new_size, {0,0});
    velocities_.resize(new_size, {0,0});
}

void ParticlesState::reserve(size_t new_capacity)
{
    masses_.reserve(new_capacity);
    positions_.reserve(new_capacity);
    velocities_.reserve(new_capacity);
}

void ParticlesState::clear() noexcept
{
    masses_.clear();
    positions_.clear();
    velocities_.clear();
}

void ParticlesState::operator=(const ParticlesStateView &view)
{
    masses_     = view.masses();
    positions_  = view.positions();
    velocities_ = view.velocities();
}

} // namespace Simulation
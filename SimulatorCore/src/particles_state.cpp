#include "particles_state.hpp"

namespace Simulation
{

namespace EXCEPTION_MSG
{
constexpr std::string_view OUT_OF_RANGE = "Particle index out of range";
} // namespace EXCEPTION_MSG

void ParticlesState::check_invariants() const
{
    assert((masses_.size()    == charges_.size()
         && charges_.size()   == positions_.size()
         && positions_.size() == velocities_.size()));
}

ParticlesState::ParticlesState(std::vector<scalar_t> masses, 
                               std::vector<scalar_t> charges, 
                               std::vector<Vec2>     positions, 
                               std::vector<Vec2>     velocities)
    : ParticlesStateView(masses_, charges_, positions_, velocities_),
      masses_    (std::move(masses)), 
      charges_   (std::move(charges)),
      positions_ (std::move(positions)), 
      velocities_(std::move(velocities))
{
    if (!(masses_.size()    == charges_.size()
       && charges_.size()   == positions_.size()
       && positions_.size() == velocities_.size())) {
        throw std::invalid_argument("Initialization vectors size mismatch");
    }
    check_invariants();
}

ParticlesState::ParticlesState(const ParticlesState &other) 
    : ParticlesStateView(masses_, charges_, positions_, velocities_),
      masses_(other.masses_), 
      charges_(other.charges_), 
      positions_(other.positions_),
      velocities_(other.velocities_)
{
    
}

ParticlesState::ParticlesState(ParticlesState &&other)
    : ParticlesStateView(masses_, charges_, positions_, velocities_),
      masses_    (std::move(other.masses_)), 
      charges_   (std::move(other.charges_)),
      positions_ (std::move(other.positions_)), 
      velocities_(std::move(other.velocities_))
{
}

ParticlesState &ParticlesState::operator=(const ParticlesState &rhs)
{
    if (this != &rhs)
    {
        masses_     = rhs.masses_;
        charges_    = rhs.charges_;
        positions_  = rhs.positions_;
        velocities_ = rhs.velocities_;
    }
    return *this;
}

ParticlesState &ParticlesState::operator=(ParticlesState &&rhs)
{
    if (this != &rhs)
    {
        masses_     = std::move(rhs.masses_);
        charges_    = std::move(rhs.charges_);
        positions_  = std::move(rhs.positions_);
        velocities_ = std::move(rhs.velocities_);
    }
    return *this;
}

void ParticlesState::push_back_particle(Particle particle)
{
    masses_.push_back(particle.mass_);
    charges_.push_back(particle.charge_);
    positions_.push_back(particle.pos_);
    velocities_.push_back(particle.vel_);
    check_invariants();
}

void ParticlesState::remove_particle(size_t index)
{
    if (index >= size()) {
        throw std::out_of_range(EXCEPTION_MSG::OUT_OF_RANGE.data());
    }
    masses_.erase(masses_.begin() + index);
    charges_.erase(charges_.begin() + index);
    positions_.erase(positions_.begin() + index);
    velocities_.erase(velocities_.begin() + index);
    check_invariants();
}

const scalar_t &ParticlesStateView::mass(size_t index) const
{
    if (index >= size()) throw std::out_of_range(EXCEPTION_MSG::OUT_OF_RANGE.data());
    return masses_[index];
}

const scalar_t &ParticlesStateView::charge(size_t index) const
{
    if (index >= size()) throw std::out_of_range(EXCEPTION_MSG::OUT_OF_RANGE.data());
    return charges_[index];
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
    charges_.resize(new_size, 0.0);
    positions_.resize(new_size, {0,0});
    velocities_.resize(new_size, {0,0});
}

void ParticlesState::reserve(size_t new_capacity)
{
    masses_.reserve(new_capacity);
    charges_.reserve(new_capacity);
    positions_.reserve(new_capacity);
    velocities_.reserve(new_capacity);
}

void ParticlesState::clear() noexcept
{
    masses_.clear();
    charges_.clear();
    positions_.clear();
    velocities_.clear();
}

void ParticlesState::operator=(const ParticlesStateView &view)
{
    masses_     = view.masses();
    charges_    = view.charges();
    positions_  = view.positions();
    velocities_ = view.velocities();
}

} // namespace Simulation
#include "impl_force_calc.hpp"

#include <cmath>
#include <omp.h>

namespace ImplForceCalc
{

Vec2List LennardJonesForceCalc::compute_forces(const ParticlesStateView &particles)
{
    Vec2List forces;
    forces.assign(particles.size(), {0,0});

    for (size_t i = 0; i < particles.size(); ++i)
    {
        for (size_t j = i + 1; j < particles.size(); ++j)
        {
            Vec2 r_vec = particles.pos(j) - particles.pos(i);
            scalar_t r = r_vec.len();
            if (Geom::eq(r, 0)) continue;
            scalar_t sr6 = std::pow(sigma_ / r, 6);
            Vec2 force_on_j_from_i = (24 * epsilon_ * (2*sr6*sr6 - sr6) / r) * r_vec.norm_vec();

            forces[j] += force_on_j_from_i;
            forces[i] += -force_on_j_from_i;
        }
    }

    return forces;
}

std::vector<ImplParam> LennardJonesForceCalc::get_params() const
{
    std::vector<ImplParam> res;

    for (size_t ind = 0; ind < param_names_.size(); ind++)
        res.push_back({param_names_[ind], 0});

    assert(res.size() == 2);

    res[0].value = epsilon_;
    res[1].value = sigma_;
    
    return res;
}

void LennardJonesForceCalc::set_params(const std::vector<scalar_t> &params_values)
{
    if (params_values.size() != param_names_.size())
        throw std::invalid_argument("Wrong number of params given");
    
    epsilon_ = params_values[0];
    sigma_   = params_values[1];
}

ForceCalcPtr LennardJonesForceCalc::clone() const
{
    return std::make_unique<LennardJonesForceCalc>(*this);
}

Vec2List HookeCentralForceCalc::compute_forces(const ParticlesStateView &particles)
{
    Vec2List forces;
    forces.assign(particles.size(), {0,0});

    for (size_t i = 0; i < particles.size(); ++i)
    {
        Vec2 r_vec = particles.pos(i);
        forces[i] = (-1) * spring_const_ * (r_vec - Vec2{center_x_, center_y_});
    }

    return forces;
}

std::vector<ImplParam> HookeCentralForceCalc::get_params() const
{
    std::vector<ImplParam> res;

    for (size_t ind = 0; ind < param_names_.size(); ind++)
        res.push_back({param_names_[ind], 0});
    
    assert(res.size() == 3);

    res[0].value = spring_const_;
    res[1].value = center_x_;
    res[2].value = center_y_;

    return res;
}

void HookeCentralForceCalc::set_params(const std::vector<scalar_t> &params_values)
{
    if (params_values.size() != param_names_.size())
        throw std::invalid_argument("Wrong number of params given");

    spring_const_ = params_values[0];
    center_x_     = params_values[1];
    center_y_     = params_values[2];
}

ForceCalcPtr HookeCentralForceCalc::clone() const
{
    return std::make_unique<HookeCentralForceCalc>(*this);
}

Vec2List HookeAmongForceCalc::compute_forces(const ParticlesStateView &particles)
{
    Vec2List forces;
    forces.assign(particles.size(), {0,0});

    for (size_t i = 0; i < particles.size(); ++i)
    {
        for (size_t j = i + 1; j < particles.size(); ++j)
        {
            Vec2 r_vec = particles.pos(j) - particles.pos(i);
            scalar_t r = r_vec.len();
            Vec2 force_on_j_from_i = (-spring_const_ * (r - unstretched_len_) / r) * r_vec; 

            forces[j] += force_on_j_from_i;
            forces[i] += -force_on_j_from_i;
        }
    }

    return forces;
}

std::vector<ImplParam> HookeAmongForceCalc::get_params() const
{
    std::vector<ImplParam> res;

    for (size_t ind = 0; ind < param_names_.size(); ind++)
        res.push_back({param_names_[ind], 0});
    
    assert(res.size() == 2);

    res[0].value = spring_const_;
    res[1].value = unstretched_len_;

    return res;
}

void HookeAmongForceCalc::set_params(const std::vector<scalar_t> &params_values)
{
    if (params_values.size() != param_names_.size())
        throw std::invalid_argument("Wrong number of params given");

    spring_const_    = params_values[0];
    unstretched_len_ = params_values[1];
}

ForceCalcPtr HookeAmongForceCalc::clone() const
{
    return std::make_unique<HookeAmongForceCalc>(*this);
}

LennardJonesOMPForceCalc::LennardJonesOMPForceCalc(scalar_t epsilon, scalar_t sigma)
    : LennardJonesForceCalc(epsilon, sigma)
{
}

Vec2List LennardJonesOMPForceCalc::compute_forces(const ParticlesStateView &particles)
{
    Vec2List forces;
    forces.assign(particles.size(), {0,0});

    #pragma omp parallel for
    for (size_t i = 0; i < particles.size(); ++i)
    {
        for (size_t j = 0; j < particles.size(); ++j)
        {
            Vec2 r_vec = particles.pos(j) - particles.pos(i);
            scalar_t r = r_vec.len();
            if (Geom::eq(r, 0)) continue;
            scalar_t sr6 = std::pow(sigma_ / r, 6);
            Vec2 force_on_i_from_j = -(24 * epsilon_ * (2*sr6*sr6 - sr6) / r) * r_vec.norm_vec();

            forces[i] += force_on_i_from_j;
        }
    }

    return forces;
}

ForceCalcPtr LennardJonesOMPForceCalc::clone() const
{
    return std::make_unique<LennardJonesOMPForceCalc>(*this);
}

Vec2 LennardJonesThreadPoolForceCalc::compute_force(const ParticlesStateView &particles, size_t i)
{
    Vec2 res_force_on_ind = {0, 0};
    Vec2 particle_ind_pos = particles.pos(i);

    for (size_t j = 0; j < particles.size(); ++j)
    {
        Vec2 r_vec = particles.pos(j) - particle_ind_pos;
        scalar_t r = r_vec.len();
        if (Geom::eq(r, 0)) 
            continue;
        scalar_t sr6 = std::pow(sigma_ / r, 6);
        Vec2 force_on_ind_from_j = (-1)*(24 * epsilon_ * (2*sr6*sr6 - sr6) / r) * r_vec.norm_vec();

        res_force_on_ind += force_on_ind_from_j;
    }

    return res_force_on_ind;
}

LennardJonesThreadPoolForceCalc::LennardJonesThreadPoolForceCalc(scalar_t epsilon, scalar_t sigma)
    : LennardJonesForceCalc(epsilon, sigma), pool_(std::thread::hardware_concurrency())
{
}

Vec2List LennardJonesThreadPoolForceCalc::compute_forces(const ParticlesStateView &particles)
{
    Vec2List forces;
    forces.assign(particles.size(), {0,0});

    const size_t num_chunks = std::thread::hardware_concurrency();
    std::vector<std::future<void>> futures;
    
    const size_t chunk_size = forces.size() / num_chunks;
    for (size_t t = 0; t < num_chunks; ++t) {
        size_t start = t * chunk_size;
        size_t end = (t == num_chunks - 1) ? forces.size() : start + chunk_size;
        futures.emplace_back(pool_.enqueue([start, end, &particles, &forces, this] {
            for (size_t i = start; i < end; ++i) {
                forces[i] = compute_force(particles, i);
            }
        }));
    }
    
    for (auto& fut : futures) 
        fut.get();

    return forces;
}
ForceCalcPtr LennardJonesThreadPoolForceCalc::clone() const
{
    return std::make_unique<LennardJonesThreadPoolForceCalc>(epsilon_, sigma_);
}

} // namespace ImplForceCalc

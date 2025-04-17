#include "impl_force_calc.hpp"

#include <cmath>

namespace ImplForceCalc
{

Vec2List LennardJonesForceCalc::compute_forces(const ParticlesStateView &particles) const
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

void LennardJonesForceCalc::set_params(const std::vector<scalar_t> &params)
{
    if (params.size() != param_names_.size())
        throw std::invalid_argument("Wrong number of params given");
    
    epsilon_ = params[0];
    sigma_   = params[1];
}

ForceCalcPtr LennardJonesForceCalc::clone() const
{
    return std::make_unique<LennardJonesForceCalc>(*this);
}
    
} // namespace ImplForceCalc



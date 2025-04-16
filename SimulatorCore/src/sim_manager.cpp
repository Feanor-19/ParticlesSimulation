#include "sim_manager.hpp"

void SimManager::SimManager::add_integrator(IIntFactoryPtr integrator_factory_ptr)
{
    factories_integrator_.push_back(std::move(integrator_factory_ptr));
}

void SimManager::SimManager::add_force_calc(IFCFactoryPtr force_calc_factory_ptr)
{
    factories_force_calc_.push_back(std::move(force_calc_factory_ptr));
}

void SimManager::SimManager::set_integrator(size_t ind)
{
    if (ind >= factories_integrator_.size())
        throw std::out_of_range("Invalid integrator index");

    simulator_ref_.change_integrator(factories_integrator_[ind]->create());
}

void SimManager::SimManager::set_force_calc(size_t ind)
{
    if (ind >= factories_force_calc_.size())
        throw std::out_of_range("Invalid force calculator index");

    simulator_ref_.change_force_calc(factories_force_calc_[ind]->create());
}

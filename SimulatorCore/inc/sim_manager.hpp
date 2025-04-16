#pragma once

#include "simulator.hpp"

namespace SimManager
{

using Simulation::Simulator;
using Simulation::Integrator;
using Simulation::ForceCalculator;

namespace Private
{
    template <typename TBase>
    class ISimImplFactory {
    public:
        virtual ~ISimImplFactory() = default;
        virtual std::unique_ptr<TBase> create() const = 0;
    };
    
    template <typename TDerived, typename TBase>
    class SimImplFactory : public ISimImplFactory<TBase> {
    public:
        std::unique_ptr<TBase> create() const override 
        {
            return std::make_unique<TDerived>();
        }
    };
} // namespace Private

using IIntFactoryPtr = std::unique_ptr< Private::ISimImplFactory<Integrator> >;
using IFCFactoryPtr  = std::unique_ptr< Private::ISimImplFactory<ForceCalculator> >;

template <typename T>
using IntegratorFactory = Private::SimImplFactory<T, Integrator>;

template <typename T>
using ForceCalcFactory = Private::SimImplFactory<T, ForceCalculator>;

class SimManager
{
private:
    std::vector<IIntFactoryPtr> factories_integrator_;
    std::vector<IFCFactoryPtr>  factories_force_calc_;

    Simulator &simulator_ref_;
public:
    SimManager(Simulator &sim_ref)
        : simulator_ref_(sim_ref) {};

    void add_integrator(IIntFactoryPtr integrator_factory_ptr);
    void add_force_calc(IFCFactoryPtr  force_calc_factory_ptr);

    template<typename T>
    void add_integrator()
    {
        factories_integrator_.push_back(std::make_unique<IntegratorFactory<T>>());
    }

    template<typename T>
    void add_force_calc()
    {
        factories_force_calc_.push_back(std::make_unique<ForceCalcFactory<T>>());
    }

    void set_integrator(size_t ind);
    void set_force_calc(size_t ind);
};

} // namespace SimManager 
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
        virtual std::string get_name() const = 0;
    };
    
    template <typename TDerived, typename TBase>
    class SimImplFactoryDefault : public ISimImplFactory<TBase> {
    public:
        std::unique_ptr<TBase> create() const override 
        {
            return std::make_unique<TDerived>();
        }

        std::string get_name() const override 
        {
            return TDerived::get_name();
        }
    };
} // namespace Private

using IIntegratorFactory = Private::ISimImplFactory<Integrator>;
using IForceCalcFactory  = Private::ISimImplFactory<ForceCalculator>;

using IIntFactoryPtr = std::unique_ptr<IIntegratorFactory>;
using IFCFactoryPtr  = std::unique_ptr<IForceCalcFactory>;

template <typename T>
using IntegratorFactoryDefault = Private::SimImplFactoryDefault<T, Integrator>;

template <typename T>
using ForceCalcFactoryDefault = Private::SimImplFactoryDefault<T, ForceCalculator>;

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

    // using IntegratorFactoryDefault
    template<typename T>
    void add_integrator()
    {
        factories_integrator_.push_back(std::make_unique<IntegratorFactoryDefault<T>>());
    }

    // using ForceCalcFactoryDefault
    template<typename T>
    void add_force_calc()
    {
        factories_force_calc_.push_back(std::make_unique<ForceCalcFactoryDefault<T>>());
    }

    void set_integrator(size_t ind);
    void set_force_calc(size_t ind);

    std::vector<std::string> get_all_integrator_names();
    std::vector<std::string> get_all_force_calc_names();
};

} // namespace SimManager 
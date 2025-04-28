# Физический симулятор частиц

Состоит из двух частей: **SimulatorCore**, выполняющий все вычисления, и **SimulatorGUI** (WIP). Для двумерных векторов используется своя [мини-реализация векторной арифметики](SimulatorCore/inc/utilities.hpp).

## SimulatorCore

Основные компоненты:

### [ParticlesState и ParticlesStateView](SimulatorCore/inc/particles_state.hpp)

`ParticlesState` хранит в себе **полное состояние симуляции** и владеет им как ресурсом. Полное состояние симуляции включает в себя **координаты, скорости и индвидуальные характеристики** _(масса и заряд в обобщённом смысле)_ всех частиц. 

Внутри хранение реализовано в виде набора из нескольких std::vector _(positions, velocities...)_, что более **cache-friendly**, чем хранение в одном векторе множества экземпляров класса, представляющего собой одну частицу. Класс обеспечивает консистентность всех внутренних std::vector и предоставляет публичный интерфейс для добавления/удаления частиц. 

`ParticlesStateView`, от которого наследуется `ParticlesState`, предоставляет интерфейс для доступа к данным, которыми владеет `ParticlesState`. Включает в себя методы для чтения/записи состояния уже существующих частиц, но не позволяет добавлять и удалять частицы. Все классы, работающие с состоянием симуляции, но не владеющие им, получают доступ к нему через `ParticlesStateView`.

### [ForceCalculator](SimulatorCore/inc/simulator.hpp)

Абстрактный класс, интерфейс для расчёта сил. Реализации задают силы взаимодействия между частицами. Есть методы для просмотра и изменения именованных параметров, зависящих от реализации.

*Ключевые методы*:  
- `Vec2List compute_forces(const ParticlesStateView& particles)` — вычисление сил для всех частиц.  

### [Integrator](SimulatorCore/inc/simulator.hpp)

Абстрактный класс, интерфейс для численного интегрирования уравнений движения. Реализации задают конкретные методы интегрирования, имея в распоряжении состояние симуляции и интерфейс для расчёта сил. Есть методы для просмотра и изменения именованных параметров, зависящих от реализации.

*Ключевые методы*:  
- `void integrate(ParticlesStateView& particles, const ForceCalculator& force_calc, scalar_t dt)` — вычисление нового состояния симуляции через время `dt`, используя `force_calc` для расчёта взаимодействия частиц.

### [Simulator](SimulatorCore/inc/simulator.hpp)

"Фасад", управляющий жизненным циклом симуляции. Хранит частицы, реализации стратегии расчёта сил и интегрирования. 

*Ключевые методы*:
- `void add_particle(Particle particle)` - добавление частицы
- `void remove_particle(size_t index)` - удаление частицы
- `void step(scalar_t dt)` - сделать шаг симуляции
- `const ParticlesStateView& particles()` - доступ только для чтения к состоянию симуляцииы
- `void change_integrator(IntegratorPtr new_integrator)` - смена интегратора
- `void change_force_calc(ForceCalcPtr  new_force_calc)` - смена калькулятора сил

### [SimImplManager](SimulatorCore/inc/sim_impl_manager.hpp)

Упрощает работу с несколькими `Integrator` и `ForceCalc` (вместе называемыми `SimImpl`), предоставляя функционал "регистрирования" и установки в подконтрольном `Simulator`. Если на момент компиляции всё известно про конкретную реализацю `SimImpl`, её регистрация производится максимально простым образом:

```cpp
sim_impl_manager.add_integrator<ImplIntegrator::RungeKutta4Integrator>();
sim_impl_manager.add_force_calc<ImplForceCalc::LennardJonesForceCalc>();
```

В более сложном случае можно реализовать свою реализацию интерфейса фабрики `IIntegratorFactory` или `IForceCalcFactory`, и с её помощью регистрировать динамически создаваемые реализации `Integrator` и `ForceCalc` _(на данный момент такое не реализовано, задел на будушее)_.

### Конкретные реализации:

#### [ForceCalculator](SimulatorCore/inc/impl_force_calc.hpp):

- `LennardJonesForceCalc` - расчёт сил исходя из потенциала Леннарда-Джонса с заданными параметрами.
- `LennardJonesOMPForceCalc` - потенциал Леннрард-Джонса, для ускорения используется параллелизация с помощью OpenMP.
- `LennardJonesThreadPoolForceCalc` - потенциал Леннрард-Джонса, для ускорения используется параллелизация с помощью самописного пула потоков.
- `HookeCentralForceCalc` - каждая частица как бы прикреплена пружиной к заданной общей для всех точке, между собой частицы не взаимодействуют.
- `HookeAmongForceCalc`- все частицы попарно скреплены пружинами заданной жесткости и нерастянутой длины.

#### [Integrator](SimulatorCore/inc/impl_integrator.hpp):

- `RungeKutta4Integrator` - метод численного интегрирования Рунге-Кутты четвёртого порядка (классический).

## SimulatorGUI

### Общая информация

Основной функционал интерфейса:

1. **Отображение частиц** на поле [цветными кружочками](#некоторые-детали-реализации)
2. Переключение режима паузы симуляции.
3. Два режима **добавления** новых частиц:
    1. **Ручной**: на месте указываются все характеристики частицы _(масса, заряд, размер, цвет)_.
    2. **По шаблону**: характеристики частицы берутся из выбранного шаблона.
4. В обоих режимах добавления частиц указываются **начальные координаты и скорости**.
5. **Управление шаблонами частиц** (создание и удаление).
6. **Выделение** отображаемой на поле частиц, с отображением характеристик выбранной частицы. **Удаление** выбранной частицы.
7. **Переключение между реализациями** `Integrator` и `ForceCalc`, изменение их **параметров**.
8. **Сохранение и загрузка состояния частиц** в базу данных.

### Некоторые детали реализации

- Пользовательский интерфейс реализован с помощью библиотеки `wxWidgets`.

- В целях визуализации добавлены для каждой частицы две новые _визуальные_ характеристики: `размер (в пикселях)` и `цвет`. 

- Класс [SimGUIWrapper](SimulatorGUI/inc/sim_gui_wrapper.hpp) осуществляет управление состоянием симуляции, учитывая визуальную составляющую.

- Ко всем полям ввода установлены валидаторы.

## Установка

Все последующие шаги, включая установку conan, можно делать в [виртуальном окружении](https://docs.python.org/3/library/venv.html):

```
python3 -m venv .venv
source .venv/bin/activate
pip3 install conan
```

### Необходимые сторонние инструменты

- cmake >= 3.28
- conan >= 2.14

Если `conan` используется первый раз, требуется также исполнить: 
```
conan profile detect
```

### Зависимости, не управляемые conan

По некоторым причинам в `conanfile` не включен wxWidgets, поэтому он требует отдельной установки:

- wxWidgets >= 3.2.7

### Клонирование репозитория

```
git clone https://github.com/Feanor-19/ParticlesSimulation
cd ParticlesSimulation
```

### Подготовка зависимостей

Release configure preset:
```
conan install . --output-folder=build/Release --build=missing --settings=build_type=Release
```

Debug configure preset:
```
conan install . --output-folder=build/Debug --build=missing --settings=build_type=Debug
```

### Сборка с cmake

Release:
```
cd build/Release
cmake ../.. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

Debug:
```
cd build/Debug
cmake ../.. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```
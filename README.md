# Физический симулятор частиц (WIP)

Состоит из двух частей: **SimulatorCore**, выполняющий все вычисления, и **SimulatorGUI** (WIP). Для двумерных векторов используется своя [мини-реализация векторной арифметики](SimulatorCore/inc/utilities.hpp).

## SimulatorCore

Основные компоненты:

### [ParticlesState и ParticlesStateView](SimulatorCore/inc/particles_state.hpp)

`ParticlesState` хранит в себе **полное состояние симуляции** и владеет им как ресурсом. Полное состояние симуляции включает в себя **координаты, скорости и индвидуальные характеристики** _(например, массы)_ всех частиц. 

Внутри хранение реализовано в виде набора из нескольких std::vector _(positions, velocities...)_, что более **cache-friendly**, чем хранение в одном векторе множества экземпляров класса, представляющего собой одну частицу. Класс обеспечивает консистентность всех внутренних std::vector и предоставляет публичный интерфейс для добавления/удаления частиц. 

`ParticlesStateView`, от которого наследуется `ParticlesState`, предоставляет интерфейс для доступа к данным, которыми владеет `ParticlesState`. Предоставляет методы для чтения/записи состояния уже существующих частиц, но не позволяет добавлять и удалять частицы. Все классы, работающие с состоянием симуляции, но не владеющие им, получают доступ к нему через `ParticlesStateView`.

### [ForceCalculator](SimulatorCore/inc/simulator.hpp)

Абстрактный класс, интерфейс для расчёта сил. Реализации задают силы взаимодействия между частицами. 

*Ключевые методы*:  
- `Vec2List compute_forces(const ParticlesStateView& particles)` — вычисление сил для всех частиц.  
- `clone()` — полиморфное копирование.

### [Integrator](SimulatorCore/inc/simulator.hpp)

Абстрактный класс, интерфейс для численного интегрирования уравнений движения. Реализации задают конкретные методы интегрирования, имея в распоряжении состояние симуляции и интерфейс для расчёта сил.

*Ключевые методы*:  
- `void integrate(ParticlesStateView& particles, const ForceCalculator& force_calc, scalar_t dt)` — вычисление нового состояния симуляции через время `dt`, используя `force_calc` для расчёта взаимодействия частиц.
- `clone()` — полиморфное копирование.

### [Simulator](SimulatorCore/inc/simulator.hpp)

"Фасад", управляющий жизненным циклом симуляции. Хранит частицы, реализации стратегии расчёта сил и интегрирования. 

*Ключевые методы*:
- `void add_particle(Particle particle)` - добавление частицы
- `void remove_particle(size_t index)` - удаление частицы
- `void step(scalar_t dt)` - сделать шаг симуляции
- `const ParticlesStateView& particles()` - доступ только для чтения к состоянию симуляцииы

### Конкретные реализации:

- `LennardJonesForceCalc` - расчёт сил исходя из потенциала Леннарда-Джонса с заданными параметрами.
- `RungeKutta4Integrator` - метод численного интегрирования Рунге-Кутты четвёртого порядка (классический).

## SimulatorGUI (WIP)

В разработке. Будет реализовывать графический интерфейс, визуализирующий симуляцию частиц. 

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
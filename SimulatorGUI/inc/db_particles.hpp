#pragma once

#include "common.hpp"
#include "simulator.hpp"

#include <SQLiteCpp/SQLiteCpp.h>
#include <stdexcept>
#include <string>
#include <vector>

class DBParticles 
{
private:
    SQLite::Database db_;

    void create_tables();

    int find_save_id(const std::string &save_name);
public:
    DBParticles(const std::string& db_path);

    void save_state(const std::string& save_name, 
                    const Simulation::ParticlesStateView &sim_particles,
                    const std::vector<SimGUI::ParticleVisual> &vis_particles);

    void load_state(const std::string& save_name,
                    Simulation::ParticlesState &sim_particles_res,
                    std::vector<SimGUI::ParticleVisual> &vis_particles_res);

    void delete_save(const std::string& save_name);

    std::vector<std::string> get_saves();
};
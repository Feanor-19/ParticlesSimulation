#include "db_particles.hpp"

DBParticles::DBParticles(const std::string &db_path)
    : db_(db_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
{
    create_tables();
}

void DBParticles::save_state(const std::string &save_name, 
                                  const Simulation::ParticlesStateView &sim_particles, 
                                  const std::vector<SimGUI::ParticleVisual> &vis_particles)
{
    {
        SQLite::Transaction tx(db_);
        SQLite::Statement insertSave(db_,
            "INSERT INTO saves (name) VALUES (?)");
        insertSave.bind(1, save_name);
        insertSave.exec();
        tx.commit();
    }

    const int save_id = static_cast<int>(db_.getLastInsertRowid());

    SQLite::Transaction tx(db_);
    SQLite::Statement insertParticle(db_,
        "INSERT INTO particles "
        "(save_id, pos_x, pos_y, vel_x, vel_y, mass, charge, size, color_r, color_g, color_b) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    const int count = sim_particles.size();
    for (int i = 0; i < count; ++i) {
        insertParticle.bind(1, save_id);
        insertParticle.bind(2, sim_particles.pos(i).x());
        insertParticle.bind(3, sim_particles.pos(i).y());
        insertParticle.bind(4, sim_particles.vel(i).x());
        insertParticle.bind(5, sim_particles.vel(i).x());
        insertParticle.bind(6, sim_particles.mass(i));
        insertParticle.bind(7, sim_particles.charge(i));

        insertParticle.bind(8,  vis_particles[i].size);
        insertParticle.bind(9,  vis_particles[i].colour.GetRed());
        insertParticle.bind(10, vis_particles[i].colour.GetGreen());
        insertParticle.bind(11, vis_particles[i].colour.GetBlue());

        insertParticle.exec();
        insertParticle.reset();
    }

    tx.commit();
}

void DBParticles::load_state(const std::string &save_name, 
                                  Simulation::ParticlesState &sim_particles_res, 
                                  std::vector<SimGUI::ParticleVisual> &vis_particles_res)
{
    int save_id = find_save_id(save_name);

    sim_particles_res.clear();
    vis_particles_res.clear();

    SQLite::Statement loadParticles(db_,
        "SELECT pos_x, pos_y, vel_x, vel_y, mass, charge, size, color_r, color_g, color_b "
        "FROM particles WHERE save_id = ?");
    loadParticles.bind(1, save_id);

    while (loadParticles.executeStep()) {

        double pos_x = loadParticles.getColumn(0).getDouble();
        double pos_y = loadParticles.getColumn(1).getDouble();
        double vel_x = loadParticles.getColumn(2).getDouble();
        double vel_y = loadParticles.getColumn(3).getDouble();
        
        double mass   = loadParticles.getColumn(4).getDouble();
        double charge = loadParticles.getColumn(5).getDouble();
        
        int size    = loadParticles.getColumn(6).getInt();
        uint8_t color_r = static_cast<uint8_t>(loadParticles.getColumn(7).getInt());
        uint8_t color_g = static_cast<uint8_t>(loadParticles.getColumn(8).getInt());
        uint8_t color_b = static_cast<uint8_t>(loadParticles.getColumn(9).getInt());

        sim_particles_res.push_back_particle({mass, charge, {pos_x, pos_y}, {vel_x, vel_y}});
        vis_particles_res.push_back({wxColor{color_r, color_g, color_b}, size});
    }
}

std::vector<std::string> DBParticles::get_saves()
{
    std::vector<std::string> names;
    SQLite::Statement query(db_,
        "SELECT name FROM saves ORDER BY save_id");
    while (query.executeStep()) {
        names.push_back(query.getColumn(0).getString());
    }
    return names;
}

void DBParticles::delete_save(const std::string &save_name)
{
    SQLite::Transaction transaction(db_);

    int save_id = find_save_id(save_name);

    {
        SQLite::Statement query(db_, "DELETE FROM particles WHERE save_id = ?");
        query.bind(1, save_id);
        query.exec();
    }

    {
        SQLite::Statement query(db_, "DELETE FROM saves WHERE save_id = ?");
        query.bind(1, save_id);
        query.exec();
    }

    transaction.commit();
}

void DBParticles::create_tables()
{
    db_.exec(R"cpp(
        CREATE TABLE IF NOT EXISTS saves (
            save_id    INTEGER PRIMARY KEY AUTOINCREMENT,
            name       TEXT    NOT NULL UNIQUE,
            created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
        );
    )cpp");

    db_.exec(R"cpp(
        CREATE TABLE IF NOT EXISTS particles (
            particle_id INTEGER PRIMARY KEY AUTOINCREMENT,
            save_id     INTEGER NOT NULL,
            pos_x       REAL    NOT NULL,
            pos_y       REAL    NOT NULL,
            vel_x       REAL    NOT NULL,
            vel_y       REAL    NOT NULL,
            mass        REAL    NOT NULL,
            charge      REAL    NOT NULL,
            size        INTEGER NOT NULL,
            color_r     INTEGER NOT NULL CHECK(color_r BETWEEN 0 AND 255),
            color_g     INTEGER NOT NULL CHECK(color_g BETWEEN 0 AND 255),
            color_b     INTEGER NOT NULL CHECK(color_b BETWEEN 0 AND 255),
            FOREIGN KEY(save_id) REFERENCES saves(save_id) ON DELETE CASCADE
        );
    )cpp");

    db_.exec(R"cpp(
        CREATE INDEX IF NOT EXISTS idx_particles_save
        ON particles(save_id);
    )cpp");
}

int DBParticles::find_save_id(const std::string &save_name)
{
    int save_id = -1;
    {
        SQLite::Statement query(db_, "SELECT save_id FROM saves WHERE name = ?");
        query.bind(1, save_name);
        if (!query.executeStep()) {
            throw std::runtime_error("Save not found");
        }
        save_id = query.getColumn(0);
    }

    return save_id;
}

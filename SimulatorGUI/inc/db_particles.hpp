#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>
#include <string>

// class ParticleDatabase {
// public:
//     ParticleDatabase(const std::string& db_path) 
//         : db(db_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) 
//     {
//         create_tables();
//     }

//     void saveState(const std::string& save_name) 
//     {
//         {
//             SQLite::Transaction transaction(db);
//             SQLite::Statement query(db, "INSERT INTO save_points (name) VALUES (?)");
//             query.bind(1, save_name);
//             query.exec();
//             transaction.commit();
//         }

//         // Get last inserted save_id
//         const int save_id = db.getLastInsertRowid();

//         // Insert particles
//         SQLite::Transaction transaction(db);
//         SQLite::Statement query(db, 
//             "INSERT INTO particles (save_id, pos_x, pos_y, vel_x, vel_y, mass, charge, size, color_r, color_g, color_b) "
//             "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

//         const int count = particles_count();
//         for (int i = 0; i < count; ++i) {
//             query.bind(1, save_id);
//             query.bind(2, get_pos_x(i));
//             query.bind(3, get_pos_y(i));
//             query.bind(4, get_vel_x(i));
//             query.bind(5, get_vel_y(i));
//             query.bind(6, get_mass(i));
//             query.bind(7, get_charge(i));
//             query.bind(8, get_size(i));
            
//             int r, g, b;
//             get_color(i, r, g, b);
//             query.bind(9, r);
//             query.bind(10, g);
//             query.bind(11, b);

//             query.exec();
//             query.reset();
//         }
//         transaction.commit();
//     }

//     void loadState(const std::string& save_name) {
//         // Get save_id by name
//         int save_id = -1;
//         {
//             SQLite::Statement query(db, "SELECT id FROM save_points WHERE name = ?");
//             query.bind(1, save_name);
//             if (!query.executeStep()) {
//                 throw std::runtime_error("Save point not found");
//             }
//             save_id = query.getColumn(0);
//         }

//         // Load particles
//         particles_clear();

//         SQLite::Statement query(db, 
//             "SELECT pos_x, pos_y, vel_x, vel_y, mass, charge, size, color_r, color_g, color_b "
//             "FROM particles WHERE save_id = ?");
//         query.bind(1, save_id);

//         while (query.executeStep()) {
//             AddParticle(
//                 query.getColumn(0).getDouble(),  // pos_x
//                 query.getColumn(1).getDouble(),  // pos_y
//                 query.getColumn(2).getDouble(),  // vel_x
//                 query.getColumn(3).getDouble(),  // vel_y
//                 query.getColumn(4).getDouble(),  // mass
//                 query.getColumn(5).getDouble(),  // charge
//                 query.getColumn(6).getInt(),     // size
//                 query.getColumn(7).getInt(),     // color_r
//                 query.getColumn(8).getInt(),     // color_g
//                 query.getColumn(9).getInt()      // color_b
//             );
//         }
//     }

//     std::vector<std::string> getSavePoints() {
//         std::vector<std::string> saves;
//         SQLite::Statement query(db, "SELECT name FROM save_points ORDER BY id");
//         while (query.executeStep()) {
//             saves.push_back(query.getColumn(0).getString());
//         }
//         return saves;
//     }

// private:
//     SQLite::Database db;

//     void create_tables() {
//         db.exec(
//             "CREATE TABLE IF NOT EXISTS save_points ("
//             "id INTEGER PRIMARY KEY,"
//             "name TEXT NOT NULL UNIQUE)"
//         );

//         db.exec(
//             "CREATE TABLE IF NOT EXISTS particles ("
//             "save_id INTEGER NOT NULL,"
//             "pos_x REAL NOT NULL,"
//             "pos_y REAL NOT NULL,"
//             "vel_x REAL NOT NULL,"
//             "vel_y REAL NOT NULL,"
//             "mass REAL NOT NULL,"
//             "charge REAL NOT NULL,"
//             "size INTEGER NOT NULL,"
//             "color_r INTEGER NOT NULL,"
//             "color_g INTEGER NOT NULL,"
//             "color_b INTEGER NOT NULL,"
//             "FOREIGN KEY(save_id) REFERENCES save_points(id))"
//         );
//     }

//     // Заглушки для доступа к данным частиц
//     int particles_count() { /* реализация */ }
//     double get_pos_x(int index) { /* реализация */ }
//     double get_pos_y(int index) { /* реализация */ }
//     double get_vel_x(int index) { /* реализация */ }
//     double get_vel_y(int index) { /* реализация */ }
//     double get_mass(int index) { /* реализация */ }
//     double get_charge(int index) { /* реализация */ }
//     int get_size(int index) { /* реализация */ }
//     void get_color(int index, int& r, int& g, int& b) { /* реализация */ }

//     void particles_clear() { /* реализация */ }
//     void AddParticle(double pos_x, double pos_y, double vel_x, double vel_y,
//                     double mass, double charge, int size, int r, int g, int b) { /* реализация */ }
// };
#include "UsersDatabase.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include "../models/Admin.h"
#include "../core/SystemSettings.h"

UsersDatabase::UsersDatabase(const char* dbName): Database(dbName) {}

void UsersDatabase::writeUserToTextFile(unsigned int id, const String& username, const String& password, const char* role) const {
    std::ofstream ofs((dbName + ".txt").getElements(), std::ios::app);

    if (!ofs.is_open()) {
        throw std::runtime_error("Error: could not open database file");
    }

    ofs << id << '|'
        << username << '|'
        << password << '|'
        << role << '\n';

    ofs.close();
}

void UsersDatabase::writeUserToBinaryFile(unsigned int id, const String& username, const String& password, const char* role) const {
    std::ofstream ofs((dbName + ".dat").getElements(), std::ios::binary | std::ios::app);

    if (!ofs.is_open()) {
        throw std::runtime_error("Error: could not open database file");
    }

    ofs.write((const char*)&id, sizeof(id));

    int len;

    len = username.getSize();
    ofs.write((const char*)&len, sizeof(len));
    ofs.write(username.getElements(), len);

    len = password.getSize();
    ofs.write((const char*)&len, sizeof(len));
    ofs.write(password.getElements(), len);

    len = std::strlen(role);
    ofs.write((const char*)&len, sizeof(len));
    ofs.write(role, len);

    ofs.close();
}

void UsersDatabase::addNewUser(const String& username, const String& password, const char* role) const {
    if (isUsernameTaken(username)) {
        throw std::logic_error("Username already taken.");
    }

    unsigned int nextId = autoIncrement();
    writeUserToTextFile(nextId, username, password, role);
    writeUserToBinaryFile(nextId, username, password, role);
}

User* UsersDatabase::getUser(const String& username) const {
    SystemSettings systemSettings = SystemSettings::getInstance();
    FileMode mode = systemSettings.getFileMode();

    String fileName = systemSettings.getDbFileName(dbName);
    std::ifstream DBFile;

    if (mode == FileMode::TEXT) {
        DBFile.open(fileName.getElements());

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        String line;
        while (true) {
            getline(DBFile, line);

            if (DBFile.eof()) {
                break;
            }

            std::stringstream ss(line.getElements());
            String idStr, usernameStr, passwordStr, roleStr;

            getline(ss, idStr, '|');
            getline(ss, usernameStr, '|');
            getline(ss, passwordStr, '|');
            getline(ss, roleStr);

            if (usernameStr == username) {
                DBFile.close();
                unsigned int currId = std::atoi(idStr.getElements());

                if (roleStr == "Admin") {
                    return new Admin(currId, username, passwordStr);
                } else {
                    return new User(currId, usernameStr, passwordStr);
                }
            }
        }
    } else {
        DBFile.open(fileName.getElements(), std::ios::binary);

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        while (!DBFile.eof()) {
            int currId;
            if (!DBFile.read((char*)&currId, sizeof(currId))) break;

            int len;

            // Username
            if (!DBFile.read((char*)&len, sizeof(len))) break;
            char* currUsername = new char[len + 1];
            if (!DBFile.read(currUsername, len)) {
              delete[] currUsername;
              break;
            }
            currUsername[len] = '\0';

            // Password
            if (!DBFile.read((char*)&len, sizeof(len))) {
                delete[] currUsername;
                break;
            }
            char* currPassword = new char[len + 1];
            if (!DBFile.read(currPassword, len)) {
                delete[] currUsername;
                delete[] currPassword;
                break;
            }
            currPassword[len] = '\0';

            // Role
            if (!DBFile.read((char*)&len, sizeof(len))) {
                delete[] currUsername;
                delete[] currPassword;
                break;
            }
            char* currRole = new char[len + 1];
            if (!DBFile.read(currRole, len)) {
                delete[] currUsername;
                delete[] currPassword;
                delete[] currRole;
                break;
            }
            currRole[len] = '\0';

            if (currUsername == username) {
                User* user = nullptr;

                if (std::strcmp(currRole, "Admin") == 0) {
                    user = new Admin(currId, currUsername, currPassword);
                } else {
                    user = new User(currId, currUsername, currPassword);
                }

                delete[] currUsername; delete[] currPassword; delete[] currRole;
                DBFile.close();
                return user;
            }

            delete[] currUsername; delete[] currPassword; delete[] currRole;
        }
    }

    DBFile.close();
    return nullptr;
}

bool UsersDatabase::isUsernameTaken(const String& username) const {
    SystemSettings systemSettings = SystemSettings::getInstance();
    FileMode mode = systemSettings.getFileMode();

    String fileName = systemSettings.getDbFileName(dbName);

    if (mode == FileMode::TEXT) {
        std::ifstream DBFile(fileName.getElements());

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        String line;
        while (true) {
            getline(DBFile, line);

            if (DBFile.eof()) {
                break;
            }

            std::stringstream ss(line.getElements());
            String idStr, usernameStr;

            getline(ss, idStr, '|');
            getline(ss, usernameStr, '|');

            if (usernameStr == username) {
                DBFile.close();
                return true;
            }
        }

        DBFile.close();
    } else {
        std::ifstream DBFile(fileName.getElements(), std::ios::binary);

        if (!DBFile.is_open()) {
            throw std::runtime_error("Error: could not open database file");
        }

        while (!DBFile.eof()) {
              int currId, len;

              if (!DBFile.read((char*)&currId, sizeof(currId))) break;

              if (!DBFile.read((char*)&len, sizeof(len))) break;
              char* currUsername = new char[len + 1];

              if (!DBFile.read(currUsername, len)) {
                  delete[] currUsername;
                  break;
              }
              currUsername[len] = '\0';

              if (currUsername == username) {
                  delete[] currUsername;
                  return true;
              }

              delete[] currUsername;

              DBFile.read((char*)&len, sizeof(len));
              DBFile.seekg(len, std::ios::cur); // skip password
              DBFile.read((char*)&len, sizeof(len));
              DBFile.seekg(len, std::ios::cur); // skip role
        }

        DBFile.close();
    }

    return false;
}
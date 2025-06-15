#include "UsersDatabase.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include "../models/Admin.h"
#include "../core/SystemSettings.h"
#include "../core/Constants.h"

UsersDatabase::UsersDatabase(const char* dbName): Database(dbName) {}

void UsersDatabase::writeUserToTextFile(unsigned int id, const String& username, const String& password, const char* role) const {
    std::ofstream ofs((dbName + ".txt").getElements(), std::ios::app);

    if (!ofs.is_open()) {
        throw std::runtime_error("Error: could not open database file");
    }

    ofs << id << FIELD_DELIMITER
        << username << FIELD_DELIMITER
        << password << FIELD_DELIMITER
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

            getline(ss, idStr, FIELD_DELIMITER);
            getline(ss, usernameStr, FIELD_DELIMITER);
            getline(ss, passwordStr, FIELD_DELIMITER);
            getline(ss, roleStr);

            if (usernameStr == username) {
                DBFile.close();
                unsigned int currId = std::atoi(idStr.getElements());

                if (roleStr == USER_ADMIN) {
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

                if (std::strcmp(currRole, USER_ADMIN) == 0) {
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

User* UsersDatabase::getById(unsigned int id) const {
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

            getline(ss, idStr, FIELD_DELIMITER);
            getline(ss, usernameStr, FIELD_DELIMITER);
            getline(ss, passwordStr, FIELD_DELIMITER);
            getline(ss, roleStr);

            unsigned int currId = std::atoi(idStr.getElements());

            if (currId == id) {
                DBFile.close();
                if (roleStr == USER_ADMIN) {
                    return new Admin(currId, usernameStr, passwordStr);
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

            if (currId == id) {
                User* user = nullptr;

                if (std::strcmp(currRole, USER_ADMIN) == 0) {
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

            getline(ss, idStr, FIELD_DELIMITER);
            getline(ss, usernameStr, FIELD_DELIMITER);

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

void UsersDatabase::removeUserById(unsigned int userId) const {
    std::ifstream input((dbName + ".txt").getElements());
    std::ofstream tempText("temp_users.txt");

    if (!input.is_open() || !tempText.is_open()) {
        throw std::runtime_error("Error: could not open user database files.");
    }

    String line;
    while (true) {
        getline(input, line);

        if (input.eof()) {
            break;
        }

        std::stringstream ss(line.getElements());
        String idStr, usernameStr, passwordStr, roleStr;

        getline(ss, idStr, FIELD_DELIMITER);
        getline(ss, usernameStr, FIELD_DELIMITER);
        getline(ss, passwordStr, FIELD_DELIMITER);
        getline(ss, roleStr);

        unsigned int currId = std::atoi(idStr.getElements());

        if (currId == userId) continue;

        tempText << currId << FIELD_DELIMITER << usernameStr << FIELD_DELIMITER << passwordStr << FIELD_DELIMITER << roleStr << '\n';
    }

    input.close();
    tempText.close();

    std::remove((dbName + ".txt").getElements());
    std::rename("temp_users.txt", (dbName + ".txt").getElements());

    std::ifstream binIn((dbName + ".dat").getElements(), std::ios::binary);
    std::ofstream binOut("temp_users.dat", std::ios::binary);

    if (!binIn.is_open() || !binOut.is_open()) {
        throw std::runtime_error("Error: could not open user binary database files.");
    }

    while (true) {
        if (binIn.eof()) break;

        unsigned int id;
        binIn.read((char*)&id, sizeof(id));
        if (binIn.eof()) break;

        unsigned int usernameLen;
        binIn.read((char*)&usernameLen, sizeof(usernameLen));
        if (binIn.eof()) break;

        char* usernameBuf = new char[usernameLen + 1];
        binIn.read(usernameBuf, usernameLen);
        usernameBuf[usernameLen] = '\0';

        unsigned int passwordLen;
        binIn.read((char*)&passwordLen, sizeof(passwordLen));
        char* passwordBuf = new char[passwordLen + 1];
        binIn.read(passwordBuf, passwordLen);
        passwordBuf[passwordLen] = '\0';

        int roleVal;
        binIn.read((char*)&roleVal, sizeof(roleVal));

        if (id != userId) {
            binOut.write((char*)&id, sizeof(id));
            binOut.write((char*)&usernameLen, sizeof(usernameLen));
            binOut.write(usernameBuf, usernameLen);
            binOut.write((char*)&passwordLen, sizeof(passwordLen));
            binOut.write(passwordBuf, passwordLen);
            binOut.write((char*)&roleVal, sizeof(roleVal));
        }

        delete[] usernameBuf;
        delete[] passwordBuf;
    }

    binIn.close();
    binOut.close();

    std::remove((dbName + ".dat").getElements());
    std::rename("temp_users.dat", (dbName + ".dat").getElements());
}

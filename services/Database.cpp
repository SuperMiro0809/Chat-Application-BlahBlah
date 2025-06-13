#include "Database.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include "../core/Constants.h"

Database::Database(const char* dbName): dbName(std::strlen(dbName), dbName) {}

Database::Database(const String& dbName): dbName(dbName) {}

unsigned int Database::autoIncrement() const {
    std::ifstream DBFile((dbName + ".txt").getElements());

    if (!DBFile.is_open()) {
        throw std::runtime_error("Error: could not open database file");
    }

    String line;
    unsigned int currentId = 0;

    while (true) {
        getline(DBFile, line);

        if (DBFile.eof()) {
            break;
        }

        std::stringstream ss(line.getElements());
        String idStr;
        getline(ss, idStr, FIELD_DELIMITER);

        unsigned int currId = std::atoi(idStr.getElements());

        if (currId > 0) {
            currentId = currId;
        }
    }

    DBFile.close();

    return currentId + 1;
}
#include "SystemSettings.h"

#include <fstream>
#include <stdexcept>
#include "Constants.h"

SystemSettings SystemSettings::instance;

SystemSettings& SystemSettings::getInstance() {
    return instance;
}

SystemSettings::SystemSettings() {
    FileMode currMode = getFromFile();
    setFileMode(currMode);
}

void SystemSettings::saveToFile() const {
    std::ofstream ofs(SETTINGS_FILE_PATH);
    if (!ofs.is_open()) {
        throw std::runtime_error("Error: could not open settings file");
    }

    switch (mode) {
        case TEXT:
            ofs << "text";
            break;
        case BINARY:
            ofs << "binary";
            break;
    }

    ofs.close();
}

FileMode SystemSettings::getFromFile() const {
    std::ifstream ifs(SETTINGS_FILE_PATH);

    if (!ifs.is_open()) {
        throw std::runtime_error("Error: could not open settings file");
    }

    String token;
    ifs >> token;

    if (token == "text") {
        return TEXT;
    } else if (token == "binary") {
        return BINARY;
    } else {
        return DEFAULT;
    }
}

void SystemSettings::setFileMode(FileMode newMode) {
    mode = newMode;
    saveToFile();
}

FileMode SystemSettings::getFileMode() const {
    return mode;
}

String SystemSettings::getDbFileName(const String& dbName) const {
    const char* ext = (mode == TEXT ? ".txt" : ".dat");

    return dbName + ext;
}
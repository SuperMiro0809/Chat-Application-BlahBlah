#include "SystemSettings.h"

SystemSettings SystemSettings::instance;

SystemSettings& SystemSettings::getInstance() {
    return instance;
}

SystemSettings::SystemSettings() {
    setFileMode(BINARY);
}

void SystemSettings::setFileMode(FileMode newMode) {
    mode = newMode;
}

FileMode SystemSettings::getFileMode() const {
    return mode;
}

String SystemSettings::getDbFileName(const String& dbName) const {
    const char* ext = (mode == TEXT ? ".txt" : ".dat");

    return dbName + ext;
}
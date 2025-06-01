#include "SystemSettings.h"

SystemSettings SystemSettings::instance;

SystemSettings& SystemSettings::getInstance() {
    return instance;
}

SystemSettings::SystemSettings() {
    setFileMode(TEXT);
}

void SystemSettings::reloadPaths() {
    const char* ext = (mode == TEXT ? ".txt" : ".dat");

    usersFile = "users" + String(ext);
}

void SystemSettings::setFileMode(FileMode newMode) {
    mode = newMode;
    reloadPaths();
}

FileMode SystemSettings::getFileMode() const {
    return mode;
}

const String& SystemSettings::getUsersFile() const {
    return usersFile;
}
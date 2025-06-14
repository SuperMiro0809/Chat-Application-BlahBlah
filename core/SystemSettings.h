#pragma once

#include "../utils/String.h"

enum FileMode {
    TEXT,
    BINARY,
    DEFAULT = TEXT
};

class SystemSettings {
    FileMode mode;

    static SystemSettings instance;

    SystemSettings();

    void saveToFile() const;

    FileMode getFromFile() const;

    public:
        static SystemSettings& getInstance();

        void setFileMode(FileMode mode);

        FileMode getFileMode() const;

        String getDbFileName(const String& dbName) const;
};
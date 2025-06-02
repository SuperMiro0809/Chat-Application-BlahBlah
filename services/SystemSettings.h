#pragma once

#include "../utils/String.h"

enum FileMode {
    TEXT,
    BINARY
};

class SystemSettings {
    FileMode mode;

    static SystemSettings instance;

    SystemSettings();

    public:
        static SystemSettings& getInstance();

        void setFileMode(FileMode mode);

        FileMode getFileMode() const;

        String getDbFileName(const String& dbName) const;
};
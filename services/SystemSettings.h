#pragma once

#include "../utils/String.h"

enum FileMode {
    TEXT,
    BINARY
};

class SystemSettings {
    FileMode mode;

    String usersFile;

    static SystemSettings instance;

    SystemSettings();

    void reloadPaths();

    public:
        static SystemSettings& getInstance();

        void setFileMode(FileMode mode);

        FileMode getFileMode() const;

        const String& getUsersFile() const;
};
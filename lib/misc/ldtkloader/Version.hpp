// Created by Modar on 11/02/2023.

#pragma once

namespace ldtk {
    struct ApiVersion {
        static constexpr unsigned Major = 1;
        static constexpr unsigned Minor = 5;
        static constexpr unsigned Patch = 3;
        static constexpr unsigned Tweak = 1;
    };

    constexpr const char* const API_VERSION_STRING = "1.5.3.1";
}

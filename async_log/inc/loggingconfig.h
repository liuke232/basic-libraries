#pragma once
#include "logging.h"
#include <string>

#ifndef CFG_JSON_FILE
#define CFG_JSON_FILE "cfg/cfg.json"
#endif

namespace loggingconfig {
    extern LogLevel log_level;
    extern std::string log_output_file;
    extern std::int64_t log_writer_cycle;

    extern bool loadConfigFileOk;
}

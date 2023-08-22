#include "loggingconfig.h"
#include "JsonObject.h"
#include "JsonParser.h"
#include "logging.h"
#include <cstdint>
#include <iostream>

namespace loggingconfig {
    LogLevel log_level = LOG_LEVEL_INFO;
    std::string log_output_file = "asynclog.log";
    std::int64_t log_writer_cycle = 20;
}

namespace {
    bool loadFromCfgJson() {
        JsonObject* root = jsonparser::parse(CFG_JSON_FILE);
        if (!root) {
            std::cerr << "--- [WARNING] Parse failed or Cannot find file [" << CFG_JSON_FILE << "]." << std::endl;
            return false;
        }
        JsonDict* dict;
        if (!root->expectJsonDict(&dict)) {
            std::cerr << "--- [WARNING] Json file [" << CFG_JSON_FILE << "] is not a dict." << std::endl;
            return false;
        }
        
        for (const auto& p : dict->items()) {
            // 解析日志等级
            if (p.first == "log_level") {
                std::string s;
                if (!p.second->expectString(&s)) {
                    std::cerr << "--- [WARNING] Expect a string literal at [log_level] in line [" 
                              << p.second->linenr << "]." << std::endl;
                    continue;
                }
                for (int i = 0; i < 5; ++i) {
                    if (s == logging::levels[i]) {
                        loggingconfig::log_level = static_cast<LogLevel>(i);
                        break;
                    }
                }
            }

            // 解析日志输出文件
            if (p.first == "log_output_file") {
                if (!p.second->expectString(&loggingconfig::log_output_file)) {
                    std::cerr << "--- [WARNING] Expect a string literal at [log_output_file] in line [" 
                              << p.second->linenr << "]." << std::endl;
                    continue;
                }
            }

            // 解析日志打印线程的调用周期
            if (p.first == "log_writer_cycle") {
                if (!p.second->expectInteger(&loggingconfig::log_writer_cycle)) {
                    std::cerr << "--- [WARNING] Expect a integer literal at [log_writer_cycle] in line [" 
                              << p.second->linenr << "]." << std::endl;
                    continue;
                }
                // 线程调用周期在10~1000ms之间
                loggingconfig::log_writer_cycle = std::min(loggingconfig::log_writer_cycle, 1000L);
                loggingconfig::log_writer_cycle = std::max(loggingconfig::log_writer_cycle, 10L);
            }
        }

        delete root;
        return true;
    }
}

namespace loggingconfig {
    bool loadConfigFileOk = loadFromCfgJson();
}

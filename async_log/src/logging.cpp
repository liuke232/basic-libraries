#include "logging.h"
#include "loggingconfig.h"
#include "logwriter.h"
#include <ctime>
#include <sstream>

namespace logging {

const char* levels[] = { "DEBUG", "INFO", "WARNING", "ERROR", "FATAL" };

void write_log(std::time_t timestamp, const char* log) {
    LogWriter::instance().enqueue(MsgItem{timestamp, log});
}

void write_log(int level, const char* time, const char* file, int line, const char* content) {
    if (level < loggingconfig::log_level)
        return;
    struct tm t;
    strptime(time, "%Y-%m-%d %H:%M:%S",  &t);
    std::stringstream ss;
    ss << time << " [" << levels[level] << "] " << content << " (" << file << ":" << line << ")";
    write_log(std::mktime(&t), ss.str().c_str());
}

void write_log(int level, const char* file, int line, const char* content) {
    if (level < loggingconfig::log_level)
        return;
    char time_str[32] = {0};
    std::time_t tm;
    std::time(&tm);
    std::strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", std::localtime(&tm));
    std::stringstream ss;
    ss << time_str << " [" << levels[level] << "] " << content << " (" << file << ":" << line << ")";
    write_log(tm, ss.str().c_str());
}

}
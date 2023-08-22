#pragma once

enum LogLevel {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
};

namespace logging {

extern const char* levels[];
void write_log(int level, const char* file, int line, const char* content);

}

#define WRITE_LOG(level, content) \
	do { \
		logging::write_log(level, __FILE__, __LINE__, content); \
    } while (false) 


#define LOG_DEBUG(content) WRITE_LOG(LOG_LEVEL_DEBUG, content)

#define LOG_INFO(content) WRITE_LOG(LOG_LEVEL_INFO, content)

#define LOG_WARNING(content) WRITE_LOG(LOG_LEVEL_WARNING, content)

#define LOG_ERROR(content) WRITE_LOG(LOG_LEVEL_ERROR, content)

#define LOG_FATAL(content) WRITE_LOG(LOG_LEVEL_FATAL, content)




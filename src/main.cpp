#include "data/database.hpp"
#include "data/log/log.hpp"
#include <fmt/core.h>

#define LOG_GRADE LOG_LEV_DEBUG

int main()
{
    LOG::Log g_log("appDatabase", "app", "LoveDang12.");
    Database db("appDatabase", "app", "LoveDang12.");
    
    g_log.switchWriteToSTDIO();

    g_log.debug("This is a debug message.");
    g_log.info("Hello, World!");
    g_log.warn("This is a warning message.");
    g_log.error("This is an error message.");

    g_log.switchWriteToDatabase();

    g_log.debug("This is a debug message.");
    g_log.info("Hello, World!");
    g_log.warn("This is a warning message.");
    g_log.error("This is an error message.");

    return 0;
}
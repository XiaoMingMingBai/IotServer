#include "data/database.hpp"
#include "data/log/log.hpp"
#include <fmt/core.h>

#define LOG_GRADE LOG_LEV_DEBUG
const std::string dataBaseName = "appDatabase";
const std::string dataBaseUser = "app";
const std::string dataBasePassword = "LoveDang12.";

int main()
{
    LOG::Log g_log;
    Database db(dataBaseName, dataBaseUser, dataBasePassword);

    g_log.debug("This is a debug message.");
    g_log.info("Hello, World!");
    g_log.warn("This is a warning message.");
    g_log.error("This is an error message.");

    return 0;
}
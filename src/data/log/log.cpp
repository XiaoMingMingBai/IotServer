#include "log.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>

#define NO_OP void(0)

#if LOG_GRADE <= LOG_LEV_DEBUG
#define LOG_DEBUG_WRITE(mes) currentWriterLog(mes, LOG_LEV_DEBUG)
#else
#define LOG_DEBUG_WRITE(mes) NO_OP
#endif

#if LOG_GRADE <= LOG_LEV_INFO
#define LOG_INFO_WRITE(mes) currentWriterLog(mes, LOG_LEV_INFO)
#else
#define LOG_INFO_WRITE(mes) NO_OP
#endif

#if LOG_GRADE <= LOG_LEV_WARN
#define LOG_WARN_WRITE(mes) currentWriterLog(mes, LOG_LEV_WARN)
#else
#define LOG_WARN_WRITE(mes) NO_OP
#endif

#if LOG_GRADE <= LOG_LEV_ERROR
#define LOG_ERROR_WRITE(mes) currentWriterLog(mes, LOG_LEV_ERROR)
#else
#define LOG_ERROR_WRITE(mes) NO_OP
#endif

static std::string getCurrentTimeWithMilliseconds()
{
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S")
        << '.' << std::setfill('0') << std::setw(3) << now_ms.count();

    return oss.str();
}

static short outToSTDIO(const char *message, short lv)
{

    if (lv == LOG_LEV_DEBUG)
    {
        std::cout << "\033[1;34m[DEBUG] ";
    }

    else if (lv == LOG_LEV_INFO)
    {
        std::cout << "\033[1;32m[INFO]  ";
    }
    else if (lv == LOG_LEV_WARN)
    {
        std::cout << "\033[1;33m[WARN]  ";
    }
    else if (lv == LOG_LEV_ERROR)
    {
        std::cout << "\033[1;31m[ERROR] ";
    }
    std::cout << getCurrentTimeWithMilliseconds() << " ";
    std::cout << message;
    std::cout << "\033[0m" << std::endl;
    return 0;
}

static short outToFile(const char *message, short lv, std::ofstream &outFile)
{
    if (lv == LOG_LEV_DEBUG)
    {
        outFile << "[DEBUG] ";
    }
    else if (lv == LOG_LEV_INFO)
    {
        outFile << "[INFO]  ";
    }
    else if (lv == LOG_LEV_WARN)
    {
        outFile << "[WARN]  ";
    }
    else if (lv == LOG_LEV_ERROR)
    {
        outFile << "[ERROR] ";
    }
    outFile << getCurrentTimeWithMilliseconds() << " ";
    outFile << message << std::endl;
    return 0;
}

static short outToDatabase(const char *message, short lv, soci::session &database)
{
    try
    {
        std::string messageStr(message);
        std::string timestamp = getCurrentTimeWithMilliseconds();
        database << "INSERT INTO log (level, timestamp, message) VALUES (:level, :timestamp, :message)",
            soci::use(std::string(lv == LOG_LEV_DEBUG ? "DEBUG" : lv == LOG_LEV_INFO ? "INFO"
                                                              : lv == LOG_LEV_WARN   ? "WARN"
                                                                                     : "ERROR")),
            soci::use(timestamp),
            soci::use(messageStr);
    }
    catch (const soci::soci_error &e)
    {
        std::cerr << "Database error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}

short LOG::Log::debug(const char *message)
{
    LOG_DEBUG_WRITE(message);
    return 0;
}

short LOG::Log::info(const char *message)
{
    LOG_INFO_WRITE(message);
    return 0;
}

short LOG::Log::warn(const char *message)
{
    LOG_WARN_WRITE(message);
    return 0;
}

short LOG::Log::error(const char *message)
{
    LOG_ERROR_WRITE(message);
    return 0;
}

short LOG::Log::switchWriteToSTDIO()
{
    if (outFile.is_open())
    {
        outFile.close();
    }
    if (database.is_connected())
    {
        database.close();
    }
    currentWriterLog = [](const char *message, short lv) -> short
    {
        return outToSTDIO(message, lv);
    };
    return 0;
}

short LOG::Log::switchWriteToFile()
{
    if (database.is_connected())
    {
        database.close();
    }
    outFile.open(fileName, std::ios::app);
    if (!outFile.is_open())
    {
        currentWriterLog = [](const char *message, short lv) -> short
        {
            return outToSTDIO(message, lv);
        };
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return -1;
    }
    currentWriterLog = [this](const char *message, short lv) -> short
    {
        return outToFile(message, lv, this->outFile);
    };
    return 0;
}

short LOG::Log::switchWriteToDatabase(void)
{
    if (outFile.is_open())
    {
        outFile.close();
    }

    try
    {
        this->database.open(soci::mysql, "db=" + std::string(databaseName) + " user=" + databaseUser + " password=" + databasePassword + " unix_socket=/var/run/mysqld/mysqld.sock");
    }
    catch (const soci::soci_error &e)
    {
        std::cerr << "SOCI error: " << e.what() << std::endl;
    }

    currentWriterLog = [this](const char *message, short lv) -> short
    {
        return outToDatabase(message, lv, this->database);
    };
    return 0;
}

LOG::Log::Log(void) : fileName("log/log.txt")
{
    switchWriteToSTDIO();
}

LOG::Log::Log(const char *file = nullptr) : fileName(file ? file : "log/log.txt")
{
    switchWriteToFile();
}

LOG::Log::Log(const std::string &database, const std::string &user, const std::string &password) : fileName("log/log.txt"), databaseName(database.c_str()), databaseUser(user.c_str()), databasePassword(password.c_str())
{
    try
    {
        this->database.open(soci::mysql, "db=" + database + " user=" + user + " password=" + password + " unix_socket=/var/run/mysqld/mysqld.sock");
        int tableCount = 0;
        this->database << "SELECT COUNT(*) FROM information_schema.tables WHERE table_name = 'log'", soci::into(tableCount);

        if (tableCount == 0)
        {
            this->database << "CREATE TABLE log ("
                              "id INT AUTO_INCREMENT PRIMARY KEY, "
                              "level VARCHAR(10), "
                              "timestamp VARCHAR(30), "
                              "message TEXT)";
        }
        else
        {
            std::cout << "Table 'log' already exists." << std::endl;
        }
        this->database.close();
    }
    catch (const soci::soci_error &e)
    {
        std::cerr << "Database error: " << e.what() << std::endl;
        switchWriteToSTDIO();
    }
    switchWriteToDatabase();
}

LOG::Log::~Log()
{
    if (outFile.is_open())
    {
        outFile.close();
    }
}
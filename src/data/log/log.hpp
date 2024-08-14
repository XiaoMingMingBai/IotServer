#if !defined(__LOG_HPP__)
#define __LOG_HPP__

#include <fstream>
#include <functional>
#include <soci/soci.h>

namespace LOG
{
#define LOG_LEV_DEBUG 0
#define LOG_LEV_INFO 1
#define LOG_LEV_WARN 2
#define LOG_LEV_ERROR 3


    class Log
    {
    private:
        const char *fileName;
        std::ofstream outFile;
        soci::session database;
        const char *databaseName;
        const char *databaseUser;
        const char *databasePassword;
        std::function<short(const char *, short)> currentWriterLog;

    public:
        short debug(const char *message);
        short info(const char *message);
        short warn(const char *message);
        short error(const char *message);
        short switchWriteToSTDIO(void);
        short switchWriteToFile(void);
        short switchWriteToDatabase(void);
        Log(void);
        Log(const char *fileName);
        Log(const std::string &database, const std::string &user, const std::string &password);
        ~Log(void);
    };

} // namespace LOG

#endif // __LOG_HPP__

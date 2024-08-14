#if !defined(__DATABASE_HPP__)
#define __DATABASE_HPP__

#include <string>

class Database
{
private:
    /* data */
public:
    Database(const std::string &database, const std::string &user, const std::string &password);
    ~Database();
};

#endif // __DATABASE_HPP__

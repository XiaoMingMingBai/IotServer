#if !defined(__DATABASE_HPP__)
#define __DATABASE_HPP__

#include <string>

class Database
{
private:
    /* data */
public:
    Database(std::string database, std::string user, std::string password);
    ~Database();
};

#endif // __DATABASE_HPP__

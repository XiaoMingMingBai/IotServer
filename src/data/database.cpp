#include "database.hpp"
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <iostream>

Database::Database(std::string database, std::string user, std::string password)
{
    try
    {
        soci::session sql(soci::mysql, "db=" + database + " user=" + user + " password=" + password + " unix_socket=/var/run/mysqld/mysqld.sock");

        // 检查并创建 users 表
        soci::statement st = (sql.prepare << "CREATE TABLE IF NOT EXISTS users ("
                                             "id INT AUTO_INCREMENT PRIMARY KEY, "
                                             "username VARCHAR(255) NOT NULL, "
                                             "password VARCHAR(255) NOT NULL, "
                                             "email VARCHAR(255) NOT NULL, "
                                             "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                                             ")");
        st.execute(true); // true 表示要获取受影响的行数

        if (st.get_affected_rows() > 0)
        {
            std::cout << "Table 'users' created successfully." << std::endl;
        }
        else
        {
            std::cout << "Table 'users' already exists." << std::endl;
        }
    }
    catch (const soci::soci_error &e)
    {
        std::cerr << "SOCI error: " << e.what() << std::endl;
    }
}

Database::~Database()
{
}
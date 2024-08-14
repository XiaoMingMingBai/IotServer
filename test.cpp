#include <iostream>
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>

int main(int argc, char const *argv[])
{
    try {
        soci::session sql(soci::mysql, "db=" + "appDatabase" + " user=" + "app" + " password=" + "LoveDang12." + " unix_socket=/var/run/mysqld/mysqld.sock");
    }
    return 0;
}

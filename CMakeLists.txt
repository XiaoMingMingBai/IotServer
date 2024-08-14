cmake_minimum_required(VERSION 3.10)

project(App)

set(CMAKE_BUILD_TYPE Debug)

find_package(fmt CONFIG REQUIRED)
find_package(SOCI CONFIG REQUIRED)

    
    
# 查找 src/ 文件夹下所有文件夹里的所有源文件
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
    
add_executable(App ${SOURCES})
    
target_link_libraries(App PRIVATE fmt::fmt)
 target_link_libraries(App PRIVATE $<IF:$<TARGET_EXISTS:SOCI::soci_mysql>,SOCI::soci_mysql,SOCI::soci_mysql_static>)
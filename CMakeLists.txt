cmake_minimum_required(VERSION 2.4)
project(LobShooter)
cmake_policy(SET CMP0003 NEW)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(Oracle REQUIRED OCCI CLNTSH)

project(oraload)
add_subdirectory(lib)
add_subdirectory(oraload)
MESSAGE("----------------------------------------------------------------------------------------------------------------------------------------------------------")
MESSAGE("Your LD_LIBRARY_PATH is set to $ENV{LD_LIBRARY_PATH}")
MESSAGE("LD_LIBRARY_PATH is needed at runtime and should be added to your environment like this:")
MESSAGE(" ")
MESSAGE("LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ENV{ORACLE_HOME}/lib:$ENV{ORACLE_HOME}/ctx/lib")
MESSAGE("LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:/usr/lib")
MESSAGE("export LD_LIBRARY_PATH")
MESSAGE("----------------------------------------------------------------------------------------------------------------------------------------------------------")

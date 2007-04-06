set(LibSrcs BasicDML.cpp BinLob.cpp Connection.cpp CharLob.cpp Lob.cpp)
include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${ORACLE_HOME}/rdbms/public)
include_directories(${ORACLE_HOME}/sdk/include)
include_directories(${ORACLE_HOME}/rdbms/public)
add_library(loadutil SHARED ${LibSrcs})

install(TARGETS loadutil DESTINATION lib)
set(LibSrcs BasicDML.cpp BinLob.cpp Connection.cpp CharLob.cpp Lob.cpp Log.cpp)
include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${ORACLE_HOME}/rdbms/public)
include_directories(${ORACLE_HOME}/sdk/include)
include_directories(${ORACLE_HOME}/rdbms/public)
#add_library(loadutil SHARED ${LibSrcs})
add_library(loadutil  ${LibSrcs})

target_link_libraries(loadutil ${libnnz} ${libocci} ${libclntsh})

install(TARGETS loadutil DESTINATION lib)
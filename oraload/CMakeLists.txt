include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${ORACLE_HOME}/rdbms/public)
set(libnnz ${ORACLE_HOME}/lib/libnnz10.so)
set(libocci ${ORACLE_HOME}/lib/libocci.so.10.1)
set(libclntsh ${ORACLE_HOME}/lib/libclntsh.so.10.1)
set(AppSrcs main.cpp)
add_executable(oraload ${AppSrcs})

target_link_libraries(oraload loadutil ${libnnz} ${libocci} ${libclntsh})

install(TARGETS oraload DESTINATION bin) 

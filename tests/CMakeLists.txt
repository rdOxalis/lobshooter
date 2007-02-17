set(ORACLE_HOME /usr/lib/oracle/xe/app/oracle/product/10.2.0/server/)
include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${ORACLE_HOME}/rdbms/public)
set(loadutil /usr/local/lib/libloadutil.so)
set(libnnz ${ORACLE_HOME}/lib/libnnz10.so)
set(libocci ${ORACLE_HOME}/lib/libocci.so.10.1)
set(libclntsh ${ORACLE_HOME}/libclntsh.so.10.1)
set(libcppunit /usr/lib/libcppunit.so)
set(AppSrcs TestSuite.cpp test.cpp)
add_executable(loadutiltests ${AppSrcs})

target_link_libraries(loadutiltests ${loadutil} ${libnnz} ${libocci} ${libclntsh} ${libcppunit} )

install(TARGETS loadutiltests DESTINATION bin) 

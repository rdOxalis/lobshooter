include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(/usr/lib/oracle/xe/app/oracle/product/10.2.0/server/rdbms/public)
set(loadutil /usr/local/lib/libloadutil.so)
set(libnnz /usr/lib/oracle/xe/app/oracle/product/10.2.0/server/lib/libnnz10.so)
set(libocci /usr/lib/oracle/xe/app/oracle/product/10.2.0/server/lib/libocci.so.10.1)
set(libclntsh /usr/lib/oracle/xe/app/oracle/product/10.2.0/server/lib/libclntsh.so.10.1)
set(libcppunit /usr/lib/libcppunit.so)
set(AppSrcs TestSuite.cpp test.cpp)
add_executable(loadutiltests ${AppSrcs})

target_link_libraries(loadutiltests ${loadutil} ${libnnz} ${libocci} ${libclntsh} ${libcppunit} )

install(TARGETS loadutiltests DESTINATION bin) 

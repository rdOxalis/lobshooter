include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(/usr/lib/oracle/xe/app/oracle/product/10.2.0/server/rdbms/public)
set(libnnz /usr/lib/oracle/xe/app/oracle/product/10.2.0/server/lib/libnnz10.so)
set(libocci /usr/lib/oracle/xe/app/oracle/product/10.2.0/server/lib/libocci.so.10.1)
set(libclntsh /usr/lib/oracle/xe/app/oracle/product/10.2.0/server/lib/libclntsh.so.10.1)
set(AppSrcs main.cpp)
add_executable(oraload ${AppSrcs})

target_link_libraries(oraload loadutil ${libnnz} ${libocci} ${libclntsh})

install(TARGETS oraload DESTINATION bin) 

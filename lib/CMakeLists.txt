set(LibSrcs BasicDML.cpp BinLob.cpp Connection.cpp CharLob.cpp Lob.cpp)
include_directories(/usr/lib/oracle/xe/app/oracle/product/10.2.0/server/rdbms/public)
add_library(loadutil SHARED ${LibSrcs})

install(TARGETS loadutil DESTINATION lib)
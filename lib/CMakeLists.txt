set(LibSrcs BasicDML.cpp BinLob.cpp Connection.cpp CharLob.cpp Lob.cpp Log.cpp)

include_directories( ${ORACLE_INCLUDES} )

add_library(loadutil  ${LibSrcs})

target_link_libraries(loadutil ${ORACLE_LIBRARIES} )

install(TARGETS loadutil DESTINATION lib)

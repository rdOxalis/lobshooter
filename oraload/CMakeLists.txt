include_directories( ${ORACLE_INCLUDES} )
include_directories( ${CMAKE_SOURCE_DIR}/lib )

set(AppSrcs main.cpp)

link_directories(${CMAKE_BINARY_DIR}/lib)
add_executable(oraload ${AppSrcs})
target_link_libraries(oraload loadutil ${ORACLE_LIBRARIES})

install(TARGETS oraload DESTINATION bin)

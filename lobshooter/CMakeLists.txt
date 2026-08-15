include_directories( ${ORACLE_INCLUDES} )
include_directories( ${CMAKE_SOURCE_DIR}/lib )

set(AppSrcs main.cpp)

link_directories(${CMAKE_BINARY_DIR}/lib)
add_executable(lobshooter ${AppSrcs})
target_link_libraries(lobshooter loadutil ${ORACLE_LIBRARIES})

install(TARGETS lobshooter DESTINATION bin)

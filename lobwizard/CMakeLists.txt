# Qt 4.5 is required, make sure that at least 4.5 is installed
# via "qmake -v".
# find_package(Qt4) looks for qmake in the system path
find_package(Qt4 REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${ORACLE_HOME}/rdbms/public)
include_directories(${ORACLE_HOME}/sdk/include)
include_directories(${ORACLE_HOME}/rdbms/public)
include(${QT_USE_FILE})
set(LobSrcs LobWizard.cpp)
set(LobMocSrcs LobWizard.h)
set(LobUis LobWizard.ui)
set(LobUis_H ui_LobWizard.h)

qt4_wrap_cpp(LobMoc ${LobMocSrcs})
qt4_wrap_ui(LobUi ${LobUis})
qt4_wrap_cpp(LobMocUi ${LobUis_H})

add_definitions(-DQT_NO_DEBUG -FLAT_NAMESPACE)
cmake_policy(SET CMP0003 OLD)
add_library(lobwizard ${LobSrcs} ${LobMoc} ${LobMocUi})
target_link_libraries(lobwizard ${QT_LIBRARIES} pthread)

install(TARGETS lobwizard DESTINATION lobwizard)

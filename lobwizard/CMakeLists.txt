find_package(Qt4 REQUIRED)
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

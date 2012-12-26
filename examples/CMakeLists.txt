project(exampleQtNetworkManager)
cmake_minimum_required(VERSION 2.6)

include(FindPkgConfig)

pkg_check_modules(QTNETWORKMANAGER REQUIRED QtNetworkManager)
if (NOT QTNETWORKMANAGER_FOUND)
    message(FATAL_ERROR "ERROR: QtNetworkManager not found. http://projects.kde.org/search?q=networkmanager&scope=all&all_words=1&projects=1&submit=Submit")
endif (NOT QTNETWORKMANAGER_FOUND)

pkg_check_modules(QTNETWORK REQUIRED QtNetwork)
pkg_check_modules(NETWORKMANAGER REQUIRED NetworkManager)

include_directories(${QTNETWORK_INCLUDE_DIRS} ${QTNETWORKMANAGER_INCLUDE_DIRS})
link_directories(${QTNETWORK_LIBRARY_DIRS} ${QTNETWORKMANAGER_LIBRARY_DIRS})

set(exampleQtNetworkManager_SRCS
   main.cpp
)

add_executable(exampleQtNetworkManager ${exampleQtNetworkManager_SRCS})
target_link_libraries(exampleQtNetworkManager QtNetworkManager QtCore QtNetwork)

add_subdirectory(createconnection)

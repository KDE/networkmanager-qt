project(createConnectionExample)
cmake_minimum_required(VERSION 2.8)

find_package(ECM 0.0.12 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

include(ECMPackageConfigHelpers)
include(ECMOptionalAddSubdirectory)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Network
)

find_package(KF5NetworkManagerQt)
set_package_properties(KF5NetworkManagerQt PROPERTIES
                       TYPE REQUIRED)

find_package(NetworkManager)
set_package_properties(NetworkManager PROPERTIES
                       TYPE REQUIRED)

include_directories(${NETWORKMANAGER_INCLUDE_DIRS})

set(createConnectionExample_SRCS
   main.cpp
)

add_executable(createConnectionExample ${createConnectionExample_SRCS})
target_link_libraries(createConnectionExample
    KF5::NetworkManagerQt
    Qt5::Core
    Qt5::Network)

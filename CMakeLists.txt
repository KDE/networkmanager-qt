cmake_minimum_required(VERSION 2.8.12)

project(NetworkManagerQt)

find_package(ECM 1.7.0 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(NetworkManager 0.9.8.4 REQUIRED)

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings)
include(KDECMakeSettings)

set(REQUIRED_QT_VERSION 5.2.0)
find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED Core DBus Network)

add_definitions(${QT_DEFINITIONS})

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)

remove_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_FROM_BYTEARRAY)

include(FeatureSummary)
include(GenerateExportHeader)

include(ECMSetupVersion)
include(ECMGenerateHeaders)

set(KF5_VERSION "5.8.0") # handled by release scripts

ecm_setup_version(${KF5_VERSION} VARIABLE_PREFIX NETWORKMANAGERQT
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/networkmanagerqt_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5NetworkManagerQtConfigVersion.cmake"
                        SOVERSION 6)

# Enable unit testing
if (BUILD_TESTING)
    add_subdirectory(src/fakenetwork)
    add_subdirectory(autotests)
endif ()

add_subdirectory(src)
##################  examples  ################################
option(BUILD_EXAMPLES "Build example applications" FALSE)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5NetworkManagerQt")

include(ECMPackageConfigHelpers)

ecm_configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF5NetworkManagerQtConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5NetworkManagerQtConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KF5NetworkManagerQtConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5NetworkManagerQtConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KF5NetworkManagerQtTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5NetworkManagerQtTargets.cmake NAMESPACE KF5:: )

install(FILES
   ${CMAKE_CURRENT_BINARY_DIR}/networkmanagerqt_version.h
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel
)

if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
endif()

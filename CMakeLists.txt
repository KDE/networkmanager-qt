cmake_minimum_required(VERSION 3.0)

set(KF5_VERSION "5.33.0") # handled by release scripts
project(NetworkManagerQt VERSION ${KF5_VERSION})

include(FeatureSummary)
find_package(ECM 5.32.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(NetworkManager 0.9.10.0 REQUIRED)

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

set(REQUIRED_QT_VERSION 5.6.0)
find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED Core DBus Network)

add_definitions(${QT_DEFINITIONS})

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)

remove_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_FROM_BYTEARRAY)


include(GenerateExportHeader)

include(ECMSetupVersion)
include(ECMGenerateHeaders)

ecm_setup_version(PROJECT VARIABLE_PREFIX NETWORKMANAGERQT
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

include(CMakePackageConfigHelpers)

configure_package_config_file(
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

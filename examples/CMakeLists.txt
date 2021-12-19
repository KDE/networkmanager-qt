set(exampleNetworkManagerQt_SRCS
   main.cpp
)

add_executable(exampleNetworkManagerQt ${exampleNetworkManagerQt_SRCS})
target_link_libraries(exampleNetworkManagerQt
    KF5::NetworkManagerQt
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Network)

add_subdirectory(createconnection)

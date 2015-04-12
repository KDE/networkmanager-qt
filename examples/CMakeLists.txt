set(exampleNetworkManagerQt_SRCS
   main.cpp
)

add_executable(exampleNetworkManagerQt ${exampleNetworkManagerQt_SRCS})
target_link_libraries(exampleNetworkManagerQt
    KF5::NetworkManagerQt
    Qt5::Core
    Qt5::Network)

add_subdirectory(createconnection)

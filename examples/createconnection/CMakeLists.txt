
set(createConnectionExample_SRCS
   main.cpp
)

add_executable(createConnectionExample ${createConnectionExample_SRCS})
target_link_libraries(createConnectionExample
    KF5::NetworkManagerQt
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Network)

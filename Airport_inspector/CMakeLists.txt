cmake_minimum_required(VERSION 3.5)

project(Airport_inspector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql PrintSupport )


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        database.h
        database.cpp
        workload_statistics.h
        workload_statistics.cpp
        workload_statistics.ui
        qcustomplot.h
        qcustomplot.cpp
)

add_executable(Airport_inspector
            ${PROJECT_SOURCES}
)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PG_Libs)

target_link_libraries(Airport_inspector PRIVATE Qt6::Widgets
                                                Qt6::Sql
                                                Qt6::PrintSupport
                                                libcrypto-3-x64
                                                libiconv-2
                                                libintl-9
                                                libpq
                                                libssl-3-x64)


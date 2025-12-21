cmake_minimum_required(VERSION 3.16)

project(paywall VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)
find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/ui/mainwindow.ui
    src/successwidget.cpp
    src/successwidget.h
    src/ui/successwidget.ui
    src/networkmanager.h
    src/networkmanager.cpp
    src/rsaverifier.h
    src/rsaverifier.cpp
    src/storagemanager.h
    src/storagemanager.cpp
    src/fileutility.cpp
    src/fileutility.h
    src/jsonutility.cpp
    src/jsonutility.h
    src/winutility.cpp
    src/winutility.h
    resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(paywall
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/favicon.rc
    )
else()
    add_executable(paywall
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(paywall PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    OpenSSL::SSL
    OpenSSL::Crypto
)

set_target_properties(paywall PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS paywall
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(paywall)
endif()

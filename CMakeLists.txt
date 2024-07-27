cmake_minimum_required(VERSION 3.16...3.29.2)
project(Lappbill VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

add_executable(Lappbill
    database.cpp database.hpp
    invoicetemplate.hpp
    main.cpp
    mainwindow.cpp mainwindow.hpp mainwindow.ui
    settingsdialog.cpp settingsdialog.hpp settingsdialog.ui
)

target_link_libraries(Lappbill PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
  )

install(TARGETS Lappbill
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

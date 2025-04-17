cmake_minimum_required(VERSION 3.14) # Qt requires a slightly newer CMake

project(FootballHighlightWizard LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Find Qt ---
# Automatically find Qt components (Core, GUI, Widgets)
# Adjust CMAKE_PREFIX_PATH if Qt isn't found automatically
# Example: set(CMAKE_PREFIX_PATH "C:/Qt/6.5.0/msvc2019_64")
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
# For Qt5 use: find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

# --- Enable Qt Features ---
set(CMAKE_AUTOMOC ON)  # Automatically handle Meta-Object Compiler (moc)
set(CMAKE_AUTORCC ON)  # Automatically handle Resource Compiler (rcc)
set(CMAKE_AUTOUIC ON)  # Automatically handle UI Compiler (uic)

# --- Add Source Files ---
add_executable(FootballHighlightWizard
    main.cpp
    highlightWizard.cpp
    highlightWizard.h
    mainwindow.cpp
    mainwindow.h
    # Add mainwindow.ui here if you create one with Qt Designer
    # mainwindow.ui
)

# --- Link Qt Libraries ---
target_link_libraries(FootballHighlightWizard PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

# --- Compiler Warnings (Keep previous settings) ---
if(MSVC)
    target_compile_options(FootballHighlightWizard PRIVATE /W4 /WX)
else()
    target_compile_options(FootballHighlightWizard PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# --- Installation (Optional) ---
# install(TARGETS FootballHighlightWizard DESTINATION bin)
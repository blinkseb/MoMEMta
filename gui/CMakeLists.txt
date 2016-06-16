
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

include_directories("ui")

# Find the QtWidgets library
find_package(Qt5Widgets)


# UI files
qt5_wrap_ui(momemta_gui_ui_headers ui/main.ui)

add_executable(momemta_gui ${momemta_gui_ui_headers}
    ui/GraphicsConnectionItem.cc
    ui/GraphicsHandleItem.cc
    ui/GraphicsModuleItem.cc
    ui/GraphicsView.cc
    ui/MainWindow.cc
    src/main.cc
)

# Use the Widgets module from Qt 5.
target_link_libraries(momemta_gui Qt5::Widgets)
target_link_libraries(momemta_gui momemta)

# Test executables

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")

add_subdirectory(integration_tests)
add_subdirectory(unit_tests)
add_subdirectory(phaseSpaceGeneration)

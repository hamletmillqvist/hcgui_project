# Configuration for example application: app1

# Source files do not need to write "../headers/" in includes
include_directories(${CMAKE_CURRENT_BINARY_DIR}/headers)

# Add project important files
file(GLOB_RECURSE HEADERS "headers/*.h")
file(GLOB_RECURSE SOURCES "source/*.cpp")

# Set executable to use important files and then set linking to DLL-files
add_executable(app1 ${HEADERS} ${SOURCES})
target_link_libraries(app1 LINK_PUBLIC hcgui)

# -----------------------------------------------------
# src/module_comments/CMakeLists.txt
# CMakeLists.txt for module_comments
#
# This module is responsible for removing comments from
# the source code during preprocessing.
# It is compiled as a static library.
# -----------------------------------------------------

# Create the static library from the module_comments source file
add_library(module_comments module_comments.c)

# Include the current source directory for header file access
target_include_directories(module_comments PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link against utils if needed by the preprocessor infrastructure
target_link_libraries(module_comments PRIVATE utils)

# Status message
message(STATUS "(${PROJECT_NAME}) Module_comments configured: Added as static library")

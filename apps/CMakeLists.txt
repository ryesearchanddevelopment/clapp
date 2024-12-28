add_subdirectory(test)

add_library(clapp_apps STATIC
    apps.h
    apps.c
    )

target_include_directories(clapp_apps PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(clapp_apps PUBLIC clapp_app_test)
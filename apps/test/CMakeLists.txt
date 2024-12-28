add_library(clapp_app_test STATIC
    test.h
    test.c
    )

target_include_directories(clapp_app_test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
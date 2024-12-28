# Clapp

Clapp is an app framework using the Clay layout engine.
The main goal is to easily develop and run apps/widgets across multiple targets.

## To use

### Linux

Run mkdir build && cd build && cmake .. && make && ./targets/raylib/clapp_raylib

## Working with apps

### Add an app

1. Copy the test folder in /apps and change "test" in the file names and CMakeLists.txt to the name of your app
2. In /apps/CMakeLists.txt, add ```add_subdirectory(<your app name>)``` at the top and add ```"clapp_app_<your app name>"``` to the target_link_libraries
3. In /apps/apps.c, add ```#include "<your app name>.h"``` at the top and ```app<your app name>``` to the list of apps

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)
target_sources(app PRIVATE src/main.c)
target_sources_ifdef(CONFIG_MYFUNCTION app PRIVATE src/exfunction.c)
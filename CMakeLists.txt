cmake_minimum_required(VERSION 3.21)

project(wifiSample)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=gold")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-fuse-ld=gold")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-fuse-ld=gold")

set(TOOLCHAIN_PATH /opt/aarch64-buildroot-linux-gnu_sdk-buildroot_kbg7z)
set(BUILD_USERINTERFACE 1)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_SYSROOT ${TOOLCHAIN_PATH}/aarch64-buildroot-linux-gnu/sysroot)
set(CMAKE_FIND_ROOT_PATH  ${CMAKE_SYSROOT})
set(CMAKE_PROGRAM_PATH  ${TOOLCHAIN_PATH}/bin ${TOOLCHAIN_PATH}/usr/bin)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)


set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/bin/aarch64-buildroot-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/bin/aarch64-buildroot-linux-gnu-g++)

set(CMAKE_C_FLAGS "-O2 -march=armv8-a -mtune=cortex-a53 -mtls-dialect=desc -pipe")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

set(CMAKE_C_FLAGS_RELEASE "-s")
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})


find_package(PkgConfig)
pkg_check_modules(DBUS REQUIRED "dbus-cxx-2.0")

if (CMAKE_CROSSCOMPILING)
    SET(BUILD_TESTS OFF CACHE BOOL "Configure CMake to not build tests" FORCE)
endif ()

include_directories(${DBUS_INCLUDE_DIRS})


add_executable(${PROJECT_NAME}
        main.cpp
        WpaSupplicant/WpaSupplicantDBusClient.cpp
        WifiScan.cpp)

if (CMAKE_CROSSCOMPILING)
    target_link_libraries(${PROJECT_NAME} PUBLIC
            ${DBUS_LIBRARIES}
            systemd
            )
else ()
    target_link_libraries(${PROJECT_NAME} PUBLIC
            ${DBUS_LIBRARIES}
            )
endif (CMAKE_CROSSCOMPILING)

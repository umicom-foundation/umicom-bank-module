#-----------------------------------------------------------------------------
# Umicom Bank Module
# File: CMakeLists.txt
#
# PURPOSE:
#   Build a thin C23 product composition over Framework-owned application
#   experience, runtime and workbench contracts.
#
# Created by: Sammy Hegab
# Organisation: Umicom Foundation
# Licence: MIT
#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.24)

project(
    BankModule
    VERSION 0.1.0
    DESCRIPTION "Thin Umicom Bank composition over Umicom Framework"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(UMICOM_BANK_BUILD_CONSOLE
       "Build the native Umicom Bank verification console"
       ON)

include(CTest)
include(GNUInstallDirs)

if(NOT TARGET Umicom::Framework)
    find_package(UmicomFramework CONFIG REQUIRED)
endif()

if(NOT TARGET Umicom::application)
    message(FATAL_ERROR
        "Umicom Bank requires Framework target Umicom::application")
endif()

if(NOT COMMAND umicom_apply_warnings)
    function(umicom_apply_warnings target)
        if(MSVC)
            target_compile_options(${target} PRIVATE /W4)
        else()
            target_compile_options(${target} PRIVATE
                -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
        endif()
    endfunction()
endif()

if(NOT COMMAND umicom_apply_sanitizers)
    function(umicom_apply_sanitizers target)
    endfunction()
endif()

add_library(umicom_bank_module STATIC
    src/application.c
    src/workspace.c
)
add_library(Umicom::bank_module ALIAS umicom_bank_module)

target_include_directories(umicom_bank_module PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(umicom_bank_module PUBLIC
    Umicom::Framework
    Umicom::application
)
umicom_apply_warnings(umicom_bank_module)
umicom_apply_sanitizers(umicom_bank_module)

if(UMICOM_BANK_BUILD_CONSOLE)
    add_executable(umicom-bank-console
        src/console/main.c)
    target_link_libraries(umicom-bank-console PRIVATE Umicom::bank_module)
    umicom_apply_warnings(umicom-bank-console)
    umicom_apply_sanitizers(umicom-bank-console)
endif()

if(BUILD_TESTING)
    add_executable(bank-application-test
        tests/test_application.c)
    target_link_libraries(bank-application-test PRIVATE Umicom::bank_module)
    umicom_apply_warnings(bank-application-test)
    umicom_apply_sanitizers(bank-application-test)
    add_test(NAME bank.module.application
             COMMAND bank-application-test)

    add_executable(bank-workspace-test
        tests/test_workspace.c)
    target_link_libraries(bank-workspace-test PRIVATE Umicom::bank_module)
    umicom_apply_warnings(bank-workspace-test)
    umicom_apply_sanitizers(bank-workspace-test)
    add_test(NAME bank.module.workspace
             COMMAND bank-workspace-test)
endif()

install(TARGETS umicom_bank_module
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
if(TARGET umicom-bank-console)
    install(TARGETS umicom-bank-console
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES application.umicom.yaml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/applications/bank)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    install(FILES LICENSE
        DESTINATION ${CMAKE_INSTALL_DATADIR}/licenses/bank)
endif()

message(STATUS "Umicom Bank Module version: ${PROJECT_VERSION}")
message(STATUS "Umicom Bank uses Framework-owned application experience")

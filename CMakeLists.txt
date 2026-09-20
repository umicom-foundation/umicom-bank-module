#-----------------------------------------------------------------------------
# Umicom Bank Module
# File: CMakeLists.txt
#
# PURPOSE:
#   Build a thin C23 product composition over Framework-owned application
#   experience, runtime and workbench contracts.
#
# AUTHOR AND ORGANISATION:
#   Sammy Hegab
#   Umicom Foundation
#
# LICENCE:
#   MIT
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
option(UMICOM_BANK_BUILD_GTK4
       "Build the Framework-rendered native Umicom Bank workstation"
       ON)

include(CTest)
include(GNUInstallDirs)

# Load the dependency only when the parent build has not already provided its target.
if(NOT TARGET Umicom::Framework)
    find_package(UmicomFramework CONFIG REQUIRED)
endif()

# Load the dependency only when the parent build has not already provided its target.
if(NOT TARGET Umicom::application)
    message(FATAL_ERROR
        "Umicom Bank requires Framework target Umicom::application")
endif()
# Load the dependency only when the parent build has not already provided its target.
if(NOT TARGET Umicom::finance_ui)
    message(FATAL_ERROR
        "Umicom Bank requires Framework target Umicom::finance_ui")
endif()

# Define the local fallback only when the parent build did not provide the shared helper.
if(NOT COMMAND umicom_apply_warnings)
    # Define the apply warnings build helper so parent and application projects apply one
    # consistent rule.
    function(umicom_apply_warnings target)
        # Select the warning options understood by the active compiler.
        if(MSVC)
            target_compile_options(${target} PRIVATE /W4)
        # Use this fallback path when the earlier condition does not apply.
        else()
            target_compile_options(${target} PRIVATE
                -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
        endif()
    endfunction()
endif()

# Define the local fallback only when the parent build did not provide the shared helper.
if(NOT COMMAND umicom_apply_sanitizers)
    # Define the apply sanitizers build helper so parent and application projects apply one
    # consistent rule.
    function(umicom_apply_sanitizers target)
    endfunction()
endif()

add_library(umicom_bank_module STATIC
    src/application_surface.c
    src/application_surface_controllers.c
    src/application.c
    src/workspace.c
    src/runtime.c
    src/readiness.c
    src/workspace_commands.c
)
add_library(Umicom::bank_module ALIAS umicom_bank_module)

target_include_directories(umicom_bank_module PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(umicom_bank_module PUBLIC
    Umicom::Framework
    Umicom::application
    Umicom::finance_ui
)
umicom_apply_warnings(umicom_bank_module)
umicom_apply_sanitizers(umicom_bank_module)

# Create this optional product surface only when its build option is enabled.
if(UMICOM_BANK_BUILD_CONSOLE)
    add_executable(umicom-bank-console
        src/console/main.c)
    target_link_libraries(umicom-bank-console PRIVATE Umicom::bank_module)
    umicom_apply_warnings(umicom-bank-console)
    umicom_apply_sanitizers(umicom-bank-console)
endif()

# Bank supplies only identity and controllers. Framework owns the complete
# native shell, reusable panels, layout editing and appearance system.
if(UMICOM_BANK_BUILD_GTK4 AND TARGET Umicom::ui_gtk4)
    add_library(umicom_bank_gtk STATIC
        src/gtk/workstation.c)
    add_library(Umicom::bank_gtk ALIAS umicom_bank_gtk)
    target_include_directories(umicom_bank_gtk PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
    target_link_libraries(umicom_bank_gtk PUBLIC
        Umicom::bank_module
        Umicom::ui_gtk4)
    umicom_apply_warnings(umicom_bank_gtk)
    umicom_apply_sanitizers(umicom_bank_gtk)

    add_executable(umicom-bank src/gtk/main.c)
    target_link_libraries(umicom-bank PRIVATE Umicom::bank_gtk)
    umicom_apply_warnings(umicom-bank)
    umicom_apply_sanitizers(umicom-bank)
    # Define the local fallback only when the parent build did not provide the shared helper.
    if(NOT COMMAND umicom_apply_application_branding)
        include(
            "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomApplicationBranding.cmake")
    endif()
    umicom_apply_application_branding(
        TARGET umicom-bank
        PRODUCT_NAME "Umicom Bank"
        INTERNAL_NAME "umicom-bank"
        APPLICATION_ID "org.umicom.bank"
        VERSION_MAJOR ${PROJECT_VERSION_MAJOR}
        VERSION_MINOR ${PROJECT_VERSION_MINOR}
        VERSION_PATCH ${PROJECT_VERSION_PATCH}
        RESOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/resources"
        DESKTOP_ENTRY
        WINDOWS_GUI)
# Apply this branch only when its contract condition is satisfied.
elseif(UMICOM_BANK_BUILD_GTK4)
    message(STATUS
        "Umicom Bank native workstation disabled because Umicom::ui_gtk4 is unavailable")
endif()

# Register verification targets only when the developer has enabled testing.
if(BUILD_TESTING)
    add_executable(bank-application-surface-test
        tests/test_application_surface.c)
    target_link_libraries(bank-application-surface-test PRIVATE
        Umicom::bank_module)
    umicom_apply_warnings(bank-application-surface-test)
    umicom_apply_sanitizers(bank-application-surface-test)
    add_test(NAME bank.module.application_surface
             COMMAND bank-application-surface-test)

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

    add_executable(bank-runtime-test
        tests/test_runtime.c
    )
    target_link_libraries(bank-runtime-test PRIVATE
        umicom_bank_module
    )
    umicom_apply_warnings(bank-runtime-test)
    umicom_apply_sanitizers(bank-runtime-test)
    add_test(
        NAME bank.module.runtime
        COMMAND bank-runtime-test
    )

    add_executable(bank-readiness-runtime-test
        tests/test_readiness_runtime.c
    )
    target_link_libraries(bank-readiness-runtime-test PRIVATE
        umicom_bank_module
    )
    umicom_apply_warnings(bank-readiness-runtime-test)
    umicom_apply_sanitizers(bank-readiness-runtime-test)
    add_test(
        NAME bank.module.readiness_runtime
        COMMAND bank-readiness-runtime-test
    )

    add_executable(bank-framework-views-test
        tests/test_framework_views.c
    )
    target_link_libraries(bank-framework-views-test PRIVATE
        Umicom::bank_module
    )
    umicom_apply_warnings(bank-framework-views-test)
    umicom_apply_sanitizers(bank-framework-views-test)
    add_test(
        NAME bank.module.framework_views
        COMMAND bank-framework-views-test
    )

endif()

# Commands without a provider must not become fictional approval requests.
if(BUILD_TESTING)
    add_executable(bank-command-feedback-test tests/test_command_feedback.c)
    target_link_libraries(bank-command-feedback-test PRIVATE Umicom::bank_module)
    umicom_apply_warnings(bank-command-feedback-test)
    umicom_apply_sanitizers(bank-command-feedback-test)
    foreach(case IN ITEMS command dirty refresh)
        add_test(NAME bank.command_feedback.${case} COMMAND bank-command-feedback-test ${case})
        set_tests_properties(bank.command_feedback.${case} PROPERTIES TIMEOUT 30)
    endforeach()
endif()

install(TARGETS umicom_bank_module
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
# Configure the optional target only when its feature has created it.
if(TARGET umicom-bank-console)
    install(TARGETS umicom-bank-console
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
# Configure the optional target only when its feature has created it.
if(TARGET umicom-bank)
    install(TARGETS umicom-bank
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES application.umicom.yaml README.md
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/applications/bank)
# Use the optional file only when it is present in this checkout.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    install(FILES LICENSE
        DESTINATION ${CMAKE_INSTALL_DATADIR}/licenses/bank)
endif()

message(STATUS "Umicom Bank Module version: ${PROJECT_VERSION}")
message(STATUS "Umicom Bank uses Framework-owned application experience")

include(
    "${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomApplicationProductisationContribution.cmake")
umicom_add_application_productisation_contribution(
    NAME "bank"
    TARGET "umicom_bank_productisation"
    SOURCE "src/productisation_contribution.c"
    TEST_SOURCE "tests/test_productisation_contribution.c")

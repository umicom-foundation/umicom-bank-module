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

# The product entry point delegates to the same Framework review used by GTK.
if(BUILD_TESTING)
    add_executable(bank-workflow-tools-test tests/test_workflow_tools.c)
    target_link_libraries(bank-workflow-tools-test PRIVATE Umicom::bank_module)
    if(COMMAND umicom_apply_warnings)
        umicom_apply_warnings(bank-workflow-tools-test)
        umicom_apply_sanitizers(bank-workflow-tools-test)
    endif()
    foreach(case IN ITEMS valid currency overflow no-side-effects)
        add_test(NAME bank.workflow_tools.${case} COMMAND bank-workflow-tools-test ${case})
        set_tests_properties(bank.workflow_tools.${case} PROPERTIES TIMEOUT 30 LABELS "bank;workflow;regression")
    endforeach()
endif()

# Install the public workflow lesson beside this product's other guides.
install(FILES docs/WORKFLOW_TOOLS.html
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/bank/docs)

# Banking operations are selected here, but their services, adapter, tests and
# installation policy live in Framework. Existing manual review, workspaces,
# commands and unavailable-provider behaviour remain unchanged.
if(NOT TARGET Umicom::bank_operations)
    include("${CMAKE_CURRENT_SOURCE_DIR}/../../framework/cmake/UmicomBankOperations.cmake")
endif()
target_sources(umicom_bank_module PRIVATE src/operations.c)
target_link_libraries(umicom_bank_module PUBLIC Umicom::bank_operations)
if(TARGET umicom_bank_gtk)
    if(NOT TARGET Umicom::bank_operations_gtk4)
        message(FATAL_ERROR "Bank's operational launcher requires the Framework banking GTK4 adapter")
    endif()
    target_link_libraries(umicom_bank_gtk PUBLIC Umicom::bank_operations_gtk4)
endif()
if(UMICOM_BANK_BUILD_CONSOLE OR BUILD_TESTING)
    add_executable(umicom-bank-operations-example src/console/operations_example.c)
    target_link_libraries(umicom-bank-operations-example PRIVATE Umicom::bank_module)
    umicom_apply_warnings(umicom-bank-operations-example)
    umicom_apply_sanitizers(umicom-bank-operations-example)
    if(BUILD_TESTING)
        add_test(NAME bank.operations.composition COMMAND umicom-bank-operations-example)
        set_tests_properties(bank.operations.composition PROPERTIES TIMEOUT 30 LABELS "bank;bank-operations;regression")
    endif()
    if(UMICOM_BANK_BUILD_CONSOLE)
        install(TARGETS umicom-bank-operations-example RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()

# Exercise Bank's actual thin layout composition without opening the banking database.
if(BUILD_TESTING AND TARGET umicom_bank_gtk)
    add_executable(bank-layout-library-test tests/test_layout_library.c)
    target_link_libraries(bank-layout-library-test PRIVATE Umicom::bank_gtk)
    umicom_apply_warnings(bank-layout-library-test)
    umicom_apply_sanitizers(bank-layout-library-test)
    add_test(NAME bank.module.layout_library COMMAND bank-layout-library-test)
    set_tests_properties(bank.module.layout_library PROPERTIES TIMEOUT 60 SKIP_RETURN_CODE 77 LABELS "bank;layout;gtk4")
endif()

# This complete lesson composes product entry points with shared reviewed commands.
if(UMICOM_BANK_BUILD_CONSOLE OR BUILD_TESTING)
    add_executable(umicom-bank-interest-example src/console/interest_example.c)
    target_link_libraries(umicom-bank-interest-example PRIVATE Umicom::bank_module)
    umicom_apply_warnings(umicom-bank-interest-example)
    umicom_apply_sanitizers(umicom-bank-interest-example)
    if(BUILD_TESTING)
        add_test(NAME bank.operations.interest_lesson COMMAND umicom-bank-interest-example)
        set_tests_properties(bank.operations.interest_lesson PROPERTIES TIMEOUT 30 LABELS "bank;interest;lesson")
    endif()
    if(UMICOM_BANK_BUILD_CONSOLE)
        install(TARGETS umicom-bank-interest-example RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()
install(FILES docs/PRACTICE_INTEREST_AND_STATEMENTS.md
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/bank/docs)

# Keep the beginner report workflow with the installed documentation.
install(FILES docs/COPYING_STATEMENT_REPORTS.md
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/bank/docs)

# Product code selects the shared reviewed-charge service; it adds no ledger rules.
if(UMICOM_BANK_BUILD_CONSOLE OR BUILD_TESTING)
    add_executable(umicom-bank-charges-example src/console/charges_example.c)
    target_link_libraries(umicom-bank-charges-example PRIVATE Umicom::bank_module)
    umicom_apply_warnings(umicom-bank-charges-example)
    umicom_apply_sanitizers(umicom-bank-charges-example)
    if(BUILD_TESTING)
        add_test(NAME bank.operations.charges_lesson COMMAND umicom-bank-charges-example)
        set_tests_properties(bank.operations.charges_lesson PROPERTIES TIMEOUT 30 LABELS "bank;charges;lesson")
    endif()
    if(UMICOM_BANK_BUILD_CONSOLE)
        install(TARGETS umicom-bank-charges-example RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()
install(FILES docs/PRACTICE_CHARGES.md
    DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/bank/docs)


# A thin product lesson exercises shared quote arithmetic without ledger mutation.
if(UMICOM_BANK_BUILD_CONSOLE OR BUILD_TESTING)
    add_executable(umicom-bank-payment-quote-example src/console/payment_quote_example.c)
    target_link_libraries(umicom-bank-payment-quote-example PRIVATE Umicom::bank_module)
    umicom_apply_warnings(umicom-bank-payment-quote-example)
    umicom_apply_sanitizers(umicom-bank-payment-quote-example)
    if(BUILD_TESTING)
        add_test(NAME bank.operations.payment_quote_lesson COMMAND umicom-bank-payment-quote-example)
        set_tests_properties(bank.operations.payment_quote_lesson PROPERTIES TIMEOUT 30 LABELS "bank;payments;quote;lesson")
    endif()
    if(UMICOM_BANK_BUILD_CONSOLE)
        install(TARGETS umicom-bank-payment-quote-example RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()
install(FILES docs/REVIEWING_PAYMENT_FEES.md DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/bank/docs)

# The queue lesson calls the actual thin product entry points and shared review.
if(UMICOM_BANK_BUILD_CONSOLE OR BUILD_TESTING)
    add_executable(umicom-bank-review-queue-example src/console/review_queue_example.c)
    target_link_libraries(umicom-bank-review-queue-example PRIVATE Umicom::bank_module)
    umicom_apply_warnings(umicom-bank-review-queue-example)
    umicom_apply_sanitizers(umicom-bank-review-queue-example)
    if(BUILD_TESTING)
        if(COMMAND umicom_register_validation_target)
            umicom_register_validation_target(umicom-bank-review-queue-example)
        endif()
        add_test(NAME bank.operations.review_queue_lesson COMMAND umicom-bank-review-queue-example)
        set_tests_properties(bank.operations.review_queue_lesson PROPERTIES TIMEOUT 30 LABELS "bank;queue;lesson;regression")
    endif()
    if(UMICOM_BANK_BUILD_CONSOLE)
        install(TARGETS umicom-bank-review-queue-example RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()
install(FILES docs/REVIEWING_PENDING_REQUESTS.md DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/bank/docs)

# The product lesson uses Framework-owned evidence, reviewed execution and replay.
if(UMICOM_BANK_BUILD_CONSOLE OR BUILD_TESTING)
    add_executable(umicom-bank-reconciliation-example src/console/reconciliation_example.c)
    target_link_libraries(umicom-bank-reconciliation-example PRIVATE Umicom::bank_module)
    umicom_apply_warnings(umicom-bank-reconciliation-example)
    umicom_apply_sanitizers(umicom-bank-reconciliation-example)
    if(BUILD_TESTING)
        if(COMMAND umicom_register_validation_target)
            umicom_register_validation_target(umicom-bank-reconciliation-example)
        endif()
        add_test(NAME bank.operations.reconciliation_lesson COMMAND umicom-bank-reconciliation-example)
        set_tests_properties(bank.operations.reconciliation_lesson PROPERTIES TIMEOUT 30 LABELS "bank;reconciliation;lesson;regression")
    endif()
    if(UMICOM_BANK_BUILD_CONSOLE)
        install(TARGETS umicom-bank-reconciliation-example RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()
install(FILES docs/INVESTIGATING_RECONCILIATION_BREAKS.md DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/bank/docs)


# The product lesson calls the actual facade and retains immutable report data.
if(UMICOM_BANK_BUILD_CONSOLE OR BUILD_TESTING)
    add_executable(umicom-bank-activity-example src/console/activity_example.c)
    target_link_libraries(umicom-bank-activity-example PRIVATE Umicom::bank_module)
    umicom_apply_warnings(umicom-bank-activity-example)
    umicom_apply_sanitizers(umicom-bank-activity-example)
    if(BUILD_TESTING)
        add_test(NAME bank.operations.activity_lesson COMMAND umicom-bank-activity-example)
        set_tests_properties(bank.operations.activity_lesson PROPERTIES TIMEOUT 30 LABELS "bank;activity;lesson;regression")
        if(COMMAND umicom_register_validation_target)
            umicom_register_validation_target(umicom-bank-activity-example)
        endif()
    endif()
    if(UMICOM_BANK_BUILD_CONSOLE)
        install(TARGETS umicom-bank-activity-example RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()
install(FILES docs/REVIEWING_ACCOUNT_ACTIVITY.md DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/bank/docs)

# A complete volatile lesson exercises the actual product facade and shared review.
if(UMICOM_BANK_BUILD_CONSOLE OR BUILD_TESTING)
    add_executable(umicom-bank-reservations-example src/console/reservations_example.c)
    target_link_libraries(umicom-bank-reservations-example PRIVATE Umicom::bank_module)
    umicom_apply_warnings(umicom-bank-reservations-example)
    umicom_apply_sanitizers(umicom-bank-reservations-example)
    if(BUILD_TESTING)
        add_test(NAME bank.operations.reservations_lesson COMMAND umicom-bank-reservations-example)
        set_tests_properties(bank.operations.reservations_lesson PROPERTIES TIMEOUT 60 LABELS "bank;reservations;lesson;regression")
        if(COMMAND umicom_register_validation_target)
            umicom_register_validation_target(umicom-bank-reservations-example)
        endif()
    endif()
    if(UMICOM_BANK_BUILD_CONSOLE)
        install(TARGETS umicom-bank-reservations-example RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()
install(FILES docs/UNDERSTANDING_RESERVED_FUNDS.md DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/bank/docs)

# The audit lesson demonstrates copied evidence after service destruction.
if(UMICOM_BANK_BUILD_CONSOLE OR BUILD_TESTING)
    add_executable(umicom-bank-audit-example src/console/audit_example.c)
    target_link_libraries(umicom-bank-audit-example PRIVATE Umicom::bank_module)
    umicom_apply_warnings(umicom-bank-audit-example)
    umicom_apply_sanitizers(umicom-bank-audit-example)
    if(BUILD_TESTING)
        add_test(NAME bank.operations.audit_lesson COMMAND umicom-bank-audit-example)
        set_tests_properties(bank.operations.audit_lesson PROPERTIES TIMEOUT 60 LABELS "bank;audit;lesson;regression")
        if(COMMAND umicom_register_validation_target)
            umicom_register_validation_target(umicom-bank-audit-example)
        endif()
    endif()
    if(UMICOM_BANK_BUILD_CONSOLE)
        install(TARGETS umicom-bank-audit-example RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()
install(FILES docs/INVESTIGATING_ACCEPTED_COMMANDS.md DESTINATION ${CMAKE_INSTALL_DATADIR}/umicom/bank/docs)

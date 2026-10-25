/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: src/console/main.c
 *
 * PURPOSE:
 *   Provide a native verification frontend for the thin product composition.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#ifdef UMICOM_HAS_UFMP_FOUNDATION
#include "umicom/financial_messaging/simulation.h"
#endif
#ifdef UMICOM_HAS_UFMP_CLEARING
#include "umicom/financial_messaging/clearing_preview.h"
#endif

#include "umicom/bank/application.h"
#include "umicom/bank/workspace.h"

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
/* Preserve the original entry signature for review. RunDefault keeps the
 * complete existing console workflow; main now dispatches an optional shared
 * Framework simulation before invoking that unchanged workflow. */
#if 0
int main(void)
#endif
static int RunDefault(void)
{
    UmiApplicationExperienceStatus status;
    const UmiApplicationExperienceDefinition *definition =
        umi_bank_application_experience();
    const UmiExperienceLayoutDefinition *layout =
        umi_bank_workspace_default();
    const UmiExperienceFeatureDefinition *next =
        umi_bank_workspace_next_feature();

    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (definition == NULL ||
        umi_bank_application_status(&status) != UMI_STATUS_OK ||
        layout == NULL) {
        fputs("Umicom Bank: Framework experience unavailable\n", stderr);
        return 1;
    }

    printf("%s\n", definition->display_name);
    printf("application: %s\n", definition->application_id);
    printf("default layout: %s\n", layout->title);
    printf("panels: %zu\n", status.panel_count);
    printf("layouts: %zu\n", status.layout_count);
    printf("features: %zu\n", status.feature_count);
    printf("readiness: %u%%\n", status.readiness_percent);
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (next != NULL)
        printf("next: %s [%s]\n", next->title,
               umi_experience_priority_text(next->priority));
    return 0;
}

/* Financial rules remain in Framework; this host owns only command routing
 * and writing the synthetic report to its console. */
int main(int argc, char **argv)
{
    if (argc == 1) return RunDefault();
    if (argc == 2 && strcmp(argv[1], "--ufmp-simulate") == 0) {
#ifdef UMICOM_HAS_UFMP_FOUNDATION
        char report[1024];
        size_t required = 0U;
        UmiStatus status = UmiUfmpSimulationFormat(report, sizeof report, &required);
        if (status != UMI_STATUS_OK) {
            fprintf(stderr, "UFMP simulation failed: %d\n", (int)status);
            return 1;
        }
        return fputs(report, stdout) == EOF ? 1 : 0;
#else
        fputs("This Framework SDK does not include UFMP simulation support.\n", stderr);
        return 2;
#endif
    }
    if (argc == 2 && strcmp(argv[1], "--clearing-preview") == 0) {
#ifdef UMICOM_HAS_UFMP_CLEARING
        char report[1024];
        size_t required = 0U;
        UmiStatus status = UmiUfmpClearingExampleFormat(report, sizeof report, &required);
        if (status != UMI_STATUS_OK) {
            fprintf(stderr, "Clearing preview failed: %d\n", (int)status);
            return 1;
        }
        return fputs(report, stdout) == EOF ? 1 : 0;
#else
        fputs("This Framework SDK does not include clearing previews.\n", stderr);
        return 2;
#endif
    }
    /* Retain the earlier help text for review; the expanded line documents both optional commands. */
#if 0
    fputs("Usage: umicom-bank-console [--ufmp-simulate]\n", stderr);
#endif
    fputs("Usage: umicom-bank-console [--ufmp-simulate | --clearing-preview]\n", stderr);
    return 2;
}

/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: tests/test_readiness_runtime.c
 *
 * PURPOSE:
 *   Verify readiness remains Framework-owned while the product exposes the status to its frontend.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include <assert.h>

#include "umicom/bank/readiness.h"

/*
 * Start this command or application, report setup failures, and return a process exit code
 * to the operating system.
 */
int main(void)
{
    UmiApplicationReadinessReport report;
    assert(umi_bank_readiness_report(&report) == UMI_STATUS_OK);
    assert(report.feature_count > 0U);
    assert(report.readiness_percent <= 100U);
    return 0;
}

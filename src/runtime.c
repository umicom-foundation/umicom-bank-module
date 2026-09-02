/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: src/runtime.c
 *
 * PURPOSE:
 *   Initialise the product through canonical Framework application runtime contracts.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include "umicom/bank/runtime.h"

#include "umicom/application/experience_catalogue.h"

/*
 * Provide the bank runtime experience operation used by this module and its client
 * applications.
 */
const UmiApplicationExperienceDefinition *umi_bank_runtime_experience(void)
{
    return umi_application_experience_catalogue_find("org.umicom.bank");
}

/*
 * Initialise bank runtime from caller-provided values so later operations receive a known
 * state.
 */
UmiStatus umi_bank_runtime_init(UmiApplicationWorkspaceRuntime *out_runtime)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_bank_runtime_experience();
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (experience == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_workspace_runtime_init(experience, out_runtime);
}

/*
 * Provide the bank runtime health operation used by this module and its client
 * applications.
 */
UmiStatus umi_bank_runtime_health(
    UmiApplicationCapabilityProbe probe,
    void *user_data,
    UmiApplicationRuntimeHealth *out_health)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_bank_runtime_experience();
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (experience == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_runtime_health_evaluate(
        experience, probe, user_data, out_health);
}

/* Compose the Bank overview from the shared Framework finance projection. */
UmiStatus umi_bank_runtime_create_overview_view(
    const UmiBankingBankingSnapshot *snapshot,
    UmiUiViewModel **out_view)
{
    /* Delegate the visible projection to Framework finance_ui so Bank does not
     * introduce a second interpretation of customer or balance state. */
    return umi_finance_ui_banking_summary_view_create(
        "overview", snapshot, out_view);
}

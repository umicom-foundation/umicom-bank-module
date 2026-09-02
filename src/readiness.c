/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: src/readiness.c
 *
 * PURPOSE:
 *   Project the canonical Framework feature backlog without product-local roadmap duplication.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include "umicom/bank/readiness.h"

#include "umicom/bank/runtime.h"
#include "umicom/application/experience_plan.h"

/*
 * Provide the bank readiness report operation used by this module and its client
 * applications.
 */
UmiStatus umi_bank_readiness_report(UmiApplicationReadinessReport *out_report)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_bank_runtime_experience();
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (experience == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_readiness_report(experience, out_report);
}

/*
 * Provide the bank readiness next feature operation used by this module and its client
 * applications.
 */
const UmiExperienceFeatureDefinition *umi_bank_readiness_next_feature(void)
{
    return umi_application_experience_next_feature(
        umi_bank_runtime_experience());
}

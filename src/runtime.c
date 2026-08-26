/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: src/runtime.c
 *
 * PURPOSE:
 *   Initialise the product through canonical Framework application runtime contracts.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/bank/runtime.h"

#include "umicom/application/experience_catalogue.h"

const UmiApplicationExperienceDefinition *umi_bank_runtime_experience(void)
{
    return umi_application_experience_catalogue_find("org.umicom.bank");
}

UmiStatus umi_bank_runtime_init(UmiApplicationWorkspaceRuntime *out_runtime)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_bank_runtime_experience();
    if (experience == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_workspace_runtime_init(experience, out_runtime);
}

UmiStatus umi_bank_runtime_health(
    UmiApplicationCapabilityProbe probe,
    void *user_data,
    UmiApplicationRuntimeHealth *out_health)
{
    const UmiApplicationExperienceDefinition *experience =
        umi_bank_runtime_experience();
    if (experience == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_runtime_health_evaluate(
        experience, probe, user_data, out_health);
}

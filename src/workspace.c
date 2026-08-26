/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: src/workspace.c
 *
 * PURPOSE:
 *   Project canonical Framework layouts and next-feature planning through the thin application boundary.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/bank/workspace.h"

#include "umicom/bank/application.h"
#include "umicom/application/experience_plan.h"

size_t umi_bank_workspace_layout_count(void)
{
    const UmiApplicationExperienceDefinition *definition =
        umi_bank_application_experience();
    return definition != NULL ? definition->layout_count : 0U;
}

const UmiExperienceLayoutDefinition *umi_bank_workspace_layout_at(
    size_t index)
{
    const UmiApplicationExperienceDefinition *definition =
        umi_bank_application_experience();
    return definition != NULL && index < definition->layout_count
        ? &definition->layouts[index] : NULL;
}

const UmiExperienceLayoutDefinition *umi_bank_workspace_default(void)
{
    const UmiApplicationExperienceDefinition *definition =
        umi_bank_application_experience();
    return definition != NULL
        ? umi_application_experience_layout_find(
            definition, definition->default_layout_id)
        : NULL;
}

const UmiExperienceFeatureDefinition *umi_bank_workspace_next_feature(
    void)
{
    return umi_application_experience_next_feature(
        umi_bank_application_experience());
}

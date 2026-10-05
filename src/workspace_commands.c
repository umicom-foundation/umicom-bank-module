/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: src/workspace_commands.c
 *
 * PURPOSE:
 *   Forward product workspace actions into Framework-owned session and context orchestration.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#include "umicom/bank/workspace_commands.h"

/*
 * Provide the bank workspace select layout operation used by this module and its client
 * applications.
 */
UmiStatus umi_bank_workspace_select_layout(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *layout_id)
{
    return umi_application_workspace_runtime_select_layout(runtime, layout_id);
}

/*
 * Provide the bank workspace activate panel operation used by this module and its client
 * applications.
 */
UmiStatus umi_bank_workspace_activate_panel(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *panel_id)
{
    return umi_application_workspace_runtime_activate_panel(runtime, panel_id);
}

/*
 * Provide the bank workspace set context operation used by this module and its client
 * applications.
 */
UmiStatus umi_bank_workspace_set_context(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *group_id,
    const char *value)
{
    return umi_application_workspace_runtime_set_context(runtime, group_id, value);
}

/*
 * Provide the bank workspace commands operation used by this module and its client
 * applications.
 */
const UmiApplicationCommandSurface *umi_bank_workspace_commands(
    const UmiApplicationWorkspaceRuntime *runtime)
{
    return runtime != NULL ? &runtime->commands : NULL;
}

/* Product adapters contribute identity, not a second context store. Reuse the
 * Framework review so all applications have the same stale-state guarantees. */
UmiStatus umi_bank_workspace_context_review(
    UmiApplicationWorkspaceRuntime *runtime,
    const UmiApplicationContextChange *changes, size_t count,
    UmiApplicationContextReview **out_review)
{
    const UmiApplicationExperienceDefinition *experience = umi_bank_runtime_experience();
    if (runtime == NULL || experience == NULL || runtime->session.experience != experience)
        return UMI_STATUS_INVALID_ARGUMENT;
    return umi_application_context_review_prepare(runtime, changes, count, out_review);
}

UmiStatus umi_bank_workspace_context_apply(
    UmiApplicationWorkspaceRuntime *runtime, UmiApplicationContextReview *review)
{
    const UmiApplicationExperienceDefinition *experience = umi_bank_runtime_experience();
    if (runtime == NULL || experience == NULL || runtime->session.experience != experience)
        return UMI_STATUS_INVALID_ARGUMENT;
    return umi_application_context_review_apply(runtime, review);
}

UmiStatus umi_bank_workspace_clear_context(
    UmiApplicationWorkspaceRuntime *runtime, const char *group_id)
{
    const UmiApplicationExperienceDefinition *experience = umi_bank_runtime_experience();
    if (runtime == NULL || experience == NULL || runtime->session.experience != experience)
        return UMI_STATUS_INVALID_ARGUMENT;
    return umi_application_workspace_runtime_clear_context(runtime, group_id);
}

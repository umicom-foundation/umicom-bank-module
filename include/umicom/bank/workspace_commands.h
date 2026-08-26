/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: include/umicom/bank/workspace_commands.h
 *
 * PURPOSE:
 *   Expose product-facing layout, panel and context commands implemented by the Framework runtime.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_BANK_WORKSPACE_COMMANDS_H
#define UMICOM_BANK_WORKSPACE_COMMANDS_H

#include "umicom/bank/runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

UmiStatus umi_bank_workspace_select_layout(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *layout_id);
UmiStatus umi_bank_workspace_activate_panel(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *panel_id);
UmiStatus umi_bank_workspace_set_context(
    UmiApplicationWorkspaceRuntime *runtime,
    const char *group_id,
    const char *value);
const UmiApplicationCommandSurface *umi_bank_workspace_commands(
    const UmiApplicationWorkspaceRuntime *runtime);

#ifdef __cplusplus
}
#endif

#endif

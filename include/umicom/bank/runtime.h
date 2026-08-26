/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: include/umicom/bank/runtime.h
 *
 * PURPOSE:
 *   Bind the thin product to the Framework-owned application workspace runtime without duplicating services.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_BANK_RUNTIME_H
#define UMICOM_BANK_RUNTIME_H

#include "umicom/application/runtime/runtime.h"
#include "umicom/finance_ui/finance_ui.h"

#ifdef __cplusplus
extern "C" {
#endif

UmiStatus umi_bank_runtime_init(
    UmiApplicationWorkspaceRuntime *out_runtime);
UmiStatus umi_bank_runtime_health(
    UmiApplicationCapabilityProbe probe,
    void *user_data,
    UmiApplicationRuntimeHealth *out_health);
const UmiApplicationExperienceDefinition *umi_bank_runtime_experience(void);
/* Build the Bank overview from a Framework banking snapshot; no balance or
 * accounting logic is owned by the application module. */
UmiStatus umi_bank_runtime_create_overview_view(
    const UmiBankingBankingSnapshot *snapshot,
    UmiUiViewModel **out_view);

#ifdef __cplusplus
}
#endif

#endif

/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: include/umicom/bank/runtime.h
 *
 * PURPOSE:
 *   Bind the thin product to the Framework-owned application workspace runtime without duplicating services.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_BANK_RUNTIME_H
#define UMICOM_BANK_RUNTIME_H

#include "umicom/application/runtime/runtime.h"
#include "umicom/finance_ui/funds_review.h"
#include "umicom/finance_ui/finance_ui.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialise bank runtime from caller-provided values so later operations receive a known
 * state.
 */
UmiStatus umi_bank_runtime_init(
    UmiApplicationWorkspaceRuntime *out_runtime);
/**
 * Provide the bank runtime health operation used by this module and its client
 * applications.
 */
UmiStatus umi_bank_runtime_health(
    UmiApplicationCapabilityProbe probe,
    void *user_data,
    UmiApplicationRuntimeHealth *out_health);
/**
 * Provide the bank runtime experience operation used by this module and its client
 * applications.
 */
const UmiApplicationExperienceDefinition *umi_bank_runtime_experience(void);
/* Build the Bank overview from a Framework banking snapshot; no balance or
 * accounting logic is owned by the application module. */
UmiStatus umi_bank_runtime_create_overview_view(
    const UmiBankingBankingSnapshot *snapshot,
    UmiUiViewModel **out_view);


/** Review caller-supplied balances; creates no payment or account mutation.
 * Delegates to Framework funds_review.h. The example and regression check
 * both the available balance and the amount remaining after a proposed debit. */
UmiStatus UmiBankRuntimeReviewFunds(const UmiMoney *ledger,
    const UmiMoney *reserved, const UmiMoney *proposedPayment,
    UmiFinanceAccountReview *out);
#ifdef __cplusplus
}
#endif

#endif

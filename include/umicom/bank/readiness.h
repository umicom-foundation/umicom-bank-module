/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: include/umicom/bank/readiness.h
 *
 * PURPOSE:
 *   Expose Framework-owned readiness and ownership evidence through the thin product boundary.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_BANK_READINESS_H
#define UMICOM_BANK_READINESS_H

#include "umicom/application/runtime/readiness.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Provide the bank readiness report operation used by this module and its client
 * applications.
 */
UmiStatus umi_bank_readiness_report(
    UmiApplicationReadinessReport *out_report);
/**
 * Provide the bank readiness next feature operation used by this module and its client
 * applications.
 */
const UmiExperienceFeatureDefinition *umi_bank_readiness_next_feature(void);

#ifdef __cplusplus
}
#endif

#endif

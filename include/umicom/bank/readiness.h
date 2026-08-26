/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: include/umicom/bank/readiness.h
 *
 * PURPOSE:
 *   Expose Framework-owned readiness and ownership evidence through the thin product boundary.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_BANK_READINESS_H
#define UMICOM_BANK_READINESS_H

#include "umicom/application/runtime/readiness.h"

#ifdef __cplusplus
extern "C" {
#endif

UmiStatus umi_bank_readiness_report(
    UmiApplicationReadinessReport *out_report);
const UmiExperienceFeatureDefinition *umi_bank_readiness_next_feature(void);

#ifdef __cplusplus
}
#endif

#endif

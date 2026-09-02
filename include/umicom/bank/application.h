/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: include/umicom/bank/application.h
 *
 * PURPOSE:
 *   Expose the thin application composition over Framework-owned experience metadata and services.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_BANK_APPLICATION_H
#define UMICOM_BANK_APPLICATION_H

#include "umicom/application/experience.h"
#include "umicom/application/experience_status.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UMI_BANK_MODULE_API_VERSION 1U

/**
 * Provide the bank application id operation used by this module and its client
 * applications.
 */
const char *umi_bank_application_id(void);

/**
 * Provide the bank application experience operation used by this module and its client
 * applications.
 */
const UmiApplicationExperienceDefinition *
umi_bank_application_experience(void);

/**
 * Provide the bank application status operation used by this module and its client
 * applications.
 */
UmiStatus umi_bank_application_status(
    UmiApplicationExperienceStatus *out_status);

#ifdef __cplusplus
}
#endif

#endif

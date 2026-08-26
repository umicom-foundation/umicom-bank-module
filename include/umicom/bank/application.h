/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: include/umicom/bank/application.h
 *
 * PURPOSE:
 *   Expose the thin application composition over Framework-owned experience metadata and services.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/

#ifndef UMICOM_BANK_APPLICATION_H
#define UMICOM_BANK_APPLICATION_H

#include "umicom/application/experience.h"
#include "umicom/application/experience_status.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UMI_BANK_MODULE_API_VERSION 1U

const char *umi_bank_application_id(void);

const UmiApplicationExperienceDefinition *
umi_bank_application_experience(void);

UmiStatus umi_bank_application_status(
    UmiApplicationExperienceStatus *out_status);

#ifdef __cplusplus
}
#endif

#endif

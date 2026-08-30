/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: include/umicom/bank/application_surface_controllers.h
 *
 * PURPOSE:
 *   Register Bank guidance and safe command-staging behavior for every panel
 *   selected by a Framework-owned Bank workspace recipe.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_BANK_APPLICATION_SURFACE_CONTROLLERS_H
#define UMICOM_BANK_APPLICATION_SURFACE_CONTROLLERS_H

#include "umicom/application/presentation/surface_runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

UmiStatus umi_bank_application_surface_controllers_register(
    UmiApplicationPresentationSurfaceRuntime *runtime, void *context);

#ifdef __cplusplus
}
#endif

#endif

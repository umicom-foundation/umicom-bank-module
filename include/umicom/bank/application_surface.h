/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: include/umicom/bank/application_surface.h
 *
 * PURPOSE:
 *   Expose Bank's standard, focus and learning workspaces through the shared
 *   Framework product-surface lifecycle and safe product controllers.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_BANK_APPLICATION_SURFACE_H
#define UMICOM_BANK_APPLICATION_SURFACE_H

#include "umicom/application/presentation/presentation.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UMI_BANK_APPLICATION_ID "org.umicom.bank"
#define UMI_BANK_STANDARD_RECIPE_ID "org.umicom.workspace.bank.standard"

/**
 * Represent the bank application surface data shared with callers of this public contract.
 */
typedef struct UmiBankApplicationSurface UmiBankApplicationSurface;

/**
 * Initialise bank application surface from caller-provided values so later operations
 * receive a known state.
 */
UmiStatus umi_bank_application_surface_create(
    UmiBankApplicationSurface **out_surface);
/**
 * Provide the bank application surface create for audience operation used by this module
 * and its client applications.
 */
UmiStatus umi_bank_application_surface_create_for_audience(
    UmiApplicationComponentRecipeAudience audience,
    UmiBankApplicationSurface **out_surface);
/**
 * Release or reset state held by bank application surface so the same storage can be
 * reused safely.
 */
void umi_bank_application_surface_destroy(UmiBankApplicationSurface *surface);
/**
 * Provide the bank application surface activate operation used by this module and its
 * client applications.
 */
UmiStatus umi_bank_application_surface_activate(
    UmiBankApplicationSurface *surface, const char *component_id);
/**
 * Provide the bank application surface deactivate operation used by this module and its
 * client applications.
 */
UmiStatus umi_bank_application_surface_deactivate(
    UmiBankApplicationSurface *surface, const char *component_id);
/**
 * Provide the bank application surface focus operation used by this module and its client
 * applications.
 */
UmiStatus umi_bank_application_surface_focus(
    UmiBankApplicationSurface *surface, const char *component_id);
/**
 * Provide the bank application surface refresh operation used by this module and its
 * client applications.
 */
UmiStatus umi_bank_application_surface_refresh(
    UmiBankApplicationSurface *surface);
/**
 * Provide the bank application surface command operation used by this module and its
 * client applications.
 */
UmiStatus umi_bank_application_surface_command(
    UmiBankApplicationSurface *surface,
    const char *component_id,
    const char *command_id);
/**
 * Provide the bank application surface context changed operation used by this module and
 * its client applications.
 */
UmiStatus umi_bank_application_surface_context_changed(
    UmiBankApplicationSurface *surface,
    const char *component_id,
    const char *context_value);
/**
 * Provide the bank application surface advance operation used by this module and its
 * client applications.
 */
UmiStatus umi_bank_application_surface_advance(
    UmiBankApplicationSurface *surface, uint32_t elapsed_seconds);
/**
 * Provide the bank application surface set background operation used by this module and
 * its client applications.
 */
UmiStatus umi_bank_application_surface_set_background(
    UmiBankApplicationSurface *surface, int background);
/**
 * Provide the bank application surface snapshot operation used by this module and its
 * client applications.
 */
UmiStatus umi_bank_application_surface_snapshot(
    const UmiBankApplicationSurface *surface,
    UmiApplicationPresentationSurfaceSnapshot *out_snapshot);
/**
 * Provide the bank application surface runtime operation used by this module and its
 * client applications.
 */
UmiApplicationPresentationSurfaceRuntime *
umi_bank_application_surface_runtime(UmiBankApplicationSurface *surface);

#ifdef __cplusplus
}
#endif

#endif

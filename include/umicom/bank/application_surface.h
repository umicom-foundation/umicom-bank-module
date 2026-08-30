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

typedef struct UmiBankApplicationSurface UmiBankApplicationSurface;

UmiStatus umi_bank_application_surface_create(
    UmiBankApplicationSurface **out_surface);
UmiStatus umi_bank_application_surface_create_for_audience(
    UmiApplicationComponentRecipeAudience audience,
    UmiBankApplicationSurface **out_surface);
void umi_bank_application_surface_destroy(UmiBankApplicationSurface *surface);
UmiStatus umi_bank_application_surface_activate(
    UmiBankApplicationSurface *surface, const char *component_id);
UmiStatus umi_bank_application_surface_deactivate(
    UmiBankApplicationSurface *surface, const char *component_id);
UmiStatus umi_bank_application_surface_focus(
    UmiBankApplicationSurface *surface, const char *component_id);
UmiStatus umi_bank_application_surface_refresh(
    UmiBankApplicationSurface *surface);
UmiStatus umi_bank_application_surface_command(
    UmiBankApplicationSurface *surface,
    const char *component_id,
    const char *command_id);
UmiStatus umi_bank_application_surface_context_changed(
    UmiBankApplicationSurface *surface,
    const char *component_id,
    const char *context_value);
UmiStatus umi_bank_application_surface_advance(
    UmiBankApplicationSurface *surface, uint32_t elapsed_seconds);
UmiStatus umi_bank_application_surface_set_background(
    UmiBankApplicationSurface *surface, int background);
UmiStatus umi_bank_application_surface_snapshot(
    const UmiBankApplicationSurface *surface,
    UmiApplicationPresentationSurfaceSnapshot *out_snapshot);
UmiApplicationPresentationSurfaceRuntime *
umi_bank_application_surface_runtime(UmiBankApplicationSurface *surface);

#ifdef __cplusplus
}
#endif

#endif

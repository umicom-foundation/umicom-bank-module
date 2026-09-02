/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: src/application_surface.c
 *
 * PURPOSE:
 *   Compose Bank over the Framework product surface while preserving a small
 *   product-owned API for frontends, consoles and automated checks.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/bank/application_surface.h"

#include <stdlib.h>

#include "umicom/bank/application_surface_controllers.h"

struct UmiBankApplicationSurface {
    UmiApplicationPresentationProductSurface product;
};

/*
 * Initialise bank application surface from caller-provided values so later operations
 * receive a known state.
 */
UmiStatus umi_bank_application_surface_create(
    UmiBankApplicationSurface **out_surface)
{
    return umi_bank_application_surface_create_for_audience(
        UMI_APPLICATION_COMPONENT_RECIPE_AUDIENCE_STANDARD, out_surface);
}

/*
 * Provide the bank application surface create for audience operation used by this module
 * and its client applications.
 */
UmiStatus umi_bank_application_surface_create_for_audience(
    UmiApplicationComponentRecipeAudience audience,
    UmiBankApplicationSurface **out_surface)
{
    UmiBankApplicationSurface *surface;
    UmiStatus status;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (out_surface == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    *out_surface = NULL;
    surface = calloc(1U, sizeof(*surface));
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (surface == NULL) return UMI_STATUS_OUT_OF_MEMORY;
    status = umi_application_presentation_product_surface_init_for_audience(
        UMI_BANK_APPLICATION_ID, audience,
        umi_bank_application_surface_controllers_register, surface,
        &surface->product);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        umi_bank_application_surface_destroy(surface);
        return status;
    }
    *out_surface = surface;
    return UMI_STATUS_OK;
}

/*
 * Release or reset state held by bank application surface so the same storage can be
 * reused safely.
 */
void umi_bank_application_surface_destroy(UmiBankApplicationSurface *surface)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (surface == NULL) return;
    umi_application_presentation_product_surface_dispose(&surface->product);
    free(surface);
}

/*
 * Provide the bank application surface activate operation used by this module and its
 * client applications.
 */
UmiStatus umi_bank_application_surface_activate(
    UmiBankApplicationSurface *surface, const char *component_id)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_activate(
              &surface->product, component_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

/*
 * Provide the bank application surface deactivate operation used by this module and its
 * client applications.
 */
UmiStatus umi_bank_application_surface_deactivate(
    UmiBankApplicationSurface *surface, const char *component_id)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_deactivate(
              &surface->product, component_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

/*
 * Provide the bank application surface focus operation used by this module and its client
 * applications.
 */
UmiStatus umi_bank_application_surface_focus(
    UmiBankApplicationSurface *surface, const char *component_id)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_focus(
              &surface->product, component_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

/*
 * Provide the bank application surface refresh operation used by this module and its
 * client applications.
 */
UmiStatus umi_bank_application_surface_refresh(
    UmiBankApplicationSurface *surface)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_refresh(
              &surface->product)
        : UMI_STATUS_INVALID_ARGUMENT;
}

/*
 * Provide the bank application surface command operation used by this module and its
 * client applications.
 */
UmiStatus umi_bank_application_surface_command(
    UmiBankApplicationSurface *surface,
    const char *component_id,
    const char *command_id)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_command(
              &surface->product, component_id, command_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

/*
 * Provide the bank application surface context changed operation used by this module and
 * its client applications.
 */
UmiStatus umi_bank_application_surface_context_changed(
    UmiBankApplicationSurface *surface,
    const char *component_id,
    const char *context_value)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_context_changed(
              &surface->product, component_id, context_value)
        : UMI_STATUS_INVALID_ARGUMENT;
}

/*
 * Provide the bank application surface advance operation used by this module and its
 * client applications.
 */
UmiStatus umi_bank_application_surface_advance(
    UmiBankApplicationSurface *surface, uint32_t elapsed_seconds)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_advance(
              &surface->product, elapsed_seconds)
        : UMI_STATUS_INVALID_ARGUMENT;
}

/*
 * Provide the bank application surface set background operation used by this module and
 * its client applications.
 */
UmiStatus umi_bank_application_surface_set_background(
    UmiBankApplicationSurface *surface, int background)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_set_background(
              &surface->product, background)
        : UMI_STATUS_INVALID_ARGUMENT;
}

/*
 * Provide the bank application surface snapshot operation used by this module and its
 * client applications.
 */
UmiStatus umi_bank_application_surface_snapshot(
    const UmiBankApplicationSurface *surface,
    UmiApplicationPresentationSurfaceSnapshot *out_snapshot)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_snapshot(
              &surface->product, out_snapshot)
        : UMI_STATUS_INVALID_ARGUMENT;
}

/*
 * Provide the bank application surface runtime operation used by this module and its
 * client applications.
 */
UmiApplicationPresentationSurfaceRuntime *
umi_bank_application_surface_runtime(UmiBankApplicationSurface *surface)
{
    return surface != NULL
        ? umi_application_presentation_product_surface_runtime(
              &surface->product)
        : NULL;
}

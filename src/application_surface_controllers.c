/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: src/application_surface_controllers.c
 *
 * PURPOSE:
 *   Describe honest Bank panel states and keep financial commands staged for
 *   authorization instead of pretending a live transaction was executed.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/bank/application_surface_controllers.h"

#include <stdio.h>
#include <string.h>

static const char *bank_guidance(const char *component_id)
{
    if (strcmp(component_id, "umicom.shared.banking") == 0)
        return "Connect an approved banking service to load accounts.";
    if (strcmp(component_id, "umicom.shared.payments") == 0)
        return "No payments are awaiting preparation.";
    if (strcmp(component_id, "umicom.shared.positions") == 0)
        return "Positions will appear after account data is connected.";
    if (strcmp(component_id, "umicom.shared.digital-assets") == 0)
        return "Digital asset services are disconnected by default.";
    if (strcmp(component_id, "umicom.shared.risk") == 0)
        return "Risk measures require approved account data.";
    if (strcmp(component_id, "umicom.shared.settlement") == 0)
        return "No settlement instructions are pending.";
    if (strcmp(component_id, "umicom.shared.collateral") == 0)
        return "No collateral movements are pending.";
    return "Audit history will record approved banking activity.";
}

static UmiStatus bank_controller(
    void *context,
    const UmiApplicationPresentationPanelPlacement *placement,
    UmiApplicationPresentationSurfaceEvent event,
    const char *payload,
    UmiApplicationPresentationSurfaceUpdate *out_update)
{
    (void)context;
    if (placement == NULL || placement->panel == NULL || out_update == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    (void)memset(out_update, 0, sizeof(*out_update));
    if (event == UMI_APPLICATION_PRESENTATION_EVENT_UNMOUNT ||
        event == UMI_APPLICATION_PRESENTATION_EVENT_DEACTIVATE) {
        out_update->state = UMI_APPLICATION_PRESENTATION_STATE_DORMANT;
        return UMI_STATUS_OK;
    }
    if (event == UMI_APPLICATION_PRESENTATION_EVENT_COMMAND) {
        out_update->state =
            UMI_APPLICATION_PRESENTATION_STATE_PERMISSION_REQUIRED;
        (void)snprintf(out_update->message, sizeof(out_update->message),
                       "Bank command '%s' is staged and requires authorization.",
                       payload != NULL ? payload : "");
        (void)snprintf(out_update->badge, sizeof(out_update->badge), "SAFE");
        return UMI_STATUS_OK;
    }
    out_update->state = UMI_APPLICATION_PRESENTATION_STATE_EMPTY;
    (void)snprintf(out_update->message, sizeof(out_update->message), "%s",
                   bank_guidance(placement->panel->component_id));
    return UMI_STATUS_OK;
}

UmiStatus umi_bank_application_surface_controllers_register(
    UmiApplicationPresentationSurfaceRuntime *runtime, void *context)
{
    if (runtime == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    return umi_application_presentation_surface_runtime_register_controller_for_all(
        runtime, bank_controller, context);
}

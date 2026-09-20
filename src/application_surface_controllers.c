/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: src/application_surface_controllers.c
 *
 * PURPOSE:
 *   Describe Bank panel state and reject unavailable financial commands
 *   without claiming that a payment or approval request has been created.
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

/* Provide the bank guidance operation used by this module and its client applications. */
static const char *bank_guidance(const char *component_id)
{
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.shared.banking") == 0)
        return "Connect an approved banking service to load accounts.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.shared.payments") == 0)
        return "No payments are awaiting preparation.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.shared.positions") == 0)
        return "Positions will appear after account data is connected.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.shared.digital-assets") == 0)
        return "Digital asset services are disconnected by default.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.shared.risk") == 0)
        return "Risk measures require approved account data.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.shared.settlement") == 0)
        return "No settlement instructions are pending.";
    /* Use the stable identifier comparison to choose the matching record or policy. */
    if (strcmp(component_id, "umicom.shared.collateral") == 0)
        return "No collateral movements are pending.";
    return "Audit history will record approved banking activity.";
}

/* Provide the bank controller operation used by this module and its client applications. */
static UmiStatus bank_controller(
    void *context,
    const UmiApplicationPresentationPanelPlacement *placement,
    UmiApplicationPresentationSurfaceEvent event,
    const char *payload,
    UmiApplicationPresentationSurfaceUpdate *out_update)
{
    (void)context;
    (void)payload;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (placement == NULL || placement->panel == NULL || out_update == NULL) {
        return UMI_STATUS_INVALID_ARGUMENT;
    }
    (void)memset(out_update, 0, sizeof(*out_update));
    /* Apply this branch only when its contract condition is satisfied. */
    if (event == UMI_APPLICATION_PRESENTATION_EVENT_UNMOUNT ||
        event == UMI_APPLICATION_PRESENTATION_EVENT_DEACTIVATE) {
        out_update->state = UMI_APPLICATION_PRESENTATION_STATE_DORMANT;
        return UMI_STATUS_OK;
    }
    /* Apply this branch only when its contract condition is satisfied. */
    if (event == UMI_APPLICATION_PRESENTATION_EVENT_COMMAND) {
        return UmiApplicationPresentationSurfaceFailure(UMI_STATUS_NOT_IMPLEMENTED,
            "No banking command service is connected. No payment or approval request was created.", out_update);
    }
    out_update->state = UMI_APPLICATION_PRESENTATION_STATE_EMPTY;
    (void)snprintf(out_update->message, sizeof(out_update->message), "%s",
                   bank_guidance(placement->panel->component_id));
    return UMI_STATUS_OK;
}

/*
 * Add bank application surface controllers only after its inputs and available capacity
 * have been checked.
 */
UmiStatus umi_bank_application_surface_controllers_register(
    UmiApplicationPresentationSurfaceRuntime *runtime, void *context)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (runtime == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    return umi_application_presentation_surface_runtime_register_controller_for_all(
        runtime, bank_controller, context);
}

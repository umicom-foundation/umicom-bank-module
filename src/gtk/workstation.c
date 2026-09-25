/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: src/gtk/workstation.c
 *
 * PURPOSE:
 *   Supply Bank identity and safe controllers to the Framework-owned native
 *   product workstation without copying layout or GTK panel implementation.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/bank/gtk_workstation.h"

#include <stdlib.h>
#include "umicom/ui/gtk4/finance_review.h"
#include "umicom/ui/gtk4/bank_operations.h"

#include "umicom/bank/application_surface.h"
#include "umicom/bank/application_surface_controllers.h"

struct UmiBankGtkWorkstation {
    UmiApplicationProductGtk4Workstation *framework_workstation;
    GtkWidget *root;
};

/* Create only the product boundary. Framework owns rendering, commands,
 * appearance, docking, floating, grouping and layout persistence. */
UmiStatus umi_bank_gtk_workstation_create(
    UmiBankGtkWorkstation **out_workstation)
{
    UmiBankGtkWorkstation *workstation;
    UmiApplicationProductGtk4WorkstationConfig config;
    UmiStatus status;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (out_workstation == NULL) return UMI_STATUS_INVALID_ARGUMENT;
    *out_workstation = NULL;
    workstation = (UmiBankGtkWorkstation *)calloc(1U, sizeof(*workstation));
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) return UMI_STATUS_OUT_OF_MEMORY;
    config = umi_application_product_gtk4_workstation_config_default(
        UMI_BANK_APPLICATION_ID,
        "Umicom Bank",
        umi_bank_application_surface_controllers_register,
        NULL);
    /* Until an approved provider and authenticated session are present, the
     * window reports its truthful operating state instead of a security claim. */
    config.mode_badge = "Offline";
    status = umi_application_product_gtk4_workstation_create(
        &config, &workstation->framework_workstation);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        umi_bank_gtk_workstation_destroy(workstation);
        return status;
    }
    /* Compose the reusable manual review above the existing workstation.
     * Calculations remain in Framework; this product owns only the assembly. */
    GtkWidget *review = NULL;
    status = UmiGtk4FinanceReviewCreate(UMI_GTK4_FINANCE_REVIEW_ACCOUNT, &review);
    if (status != UMI_STATUS_OK) {
        umi_bank_gtk_workstation_destroy(workstation);
        return status;
    }
    workstation->root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    g_object_ref_sink(workstation->root);
    /* Add the shared operational workspace without replacing the manual review
     * or existing product body. Framework owns the launcher, per-user storage,
     * domain commands and projections; this application supplies composition. */
    gtk_box_append(GTK_BOX(workstation->root), UmiGtk4BankOperationsLauncherCreate());
    gtk_box_append(GTK_BOX(workstation->root), review);
    GtkWidget *inner = umi_application_product_gtk4_workstation_widget(workstation->framework_workstation);
    gtk_widget_set_vexpand(inner, TRUE);
    gtk_box_append(GTK_BOX(workstation->root), inner);
    *out_workstation = workstation;
    return UMI_STATUS_OK;
}

/* Forward explicit native titlebar adoption to the existing Framework owner. */
UmiStatus umi_bank_gtk_workstation_bind_window(
    UmiBankGtkWorkstation *workstation, GtkWindow *window)
{
    return workstation != NULL
        ? umi_application_product_gtk4_workstation_bind_window(workstation->framework_workstation, window)
        : UMI_STATUS_INVALID_ARGUMENT;
}

/* Delegate explicit persistence to the existing shared layout owner. */
UmiStatus umi_bank_gtk_workstation_enable_checkpoint_storage(
    UmiBankGtkWorkstation *workstation, int restore_saved)
{
    return workstation != NULL
        ? umi_application_product_gtk4_workstation_enable_checkpoint_storage(
            workstation->framework_workstation, restore_saved)
        : UMI_STATUS_INVALID_ARGUMENT;
}


/* Destruction follows the reverse ownership order used during creation. */
/* Release this composition after its layout-owned storage and widgets. */
void umi_bank_gtk_workstation_destroy(UmiBankGtkWorkstation *workstation)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (workstation == NULL) return;
    /* A native window may retain the composition root. Remove the service-
     * backed body and disable the wrapper before releasing its owner reference. */
    if (workstation->root != NULL) {
        gtk_widget_set_sensitive(workstation->root, FALSE);
        GtkWidget *inner = umi_application_product_gtk4_workstation_widget(workstation->framework_workstation);
        if (inner != NULL && gtk_widget_get_parent(inner) == workstation->root)
            gtk_box_remove(GTK_BOX(workstation->root), inner);
    }
    umi_application_product_gtk4_workstation_destroy(
        workstation->framework_workstation);
    workstation->framework_workstation = NULL;
    g_clear_object(&workstation->root);
    free(workstation);
}

/* Migration: the returned widget now belongs to this thin composition;
 * it contains the borrowed Framework body and the new Framework review.
 * The old body-only ownership comment is retained immediately below. */
/* The returned widget is borrowed; the Framework workstation destroys it. */
GtkWidget *umi_bank_gtk_workstation_widget(
    UmiBankGtkWorkstation *workstation)
{
    /* The old body-only return is preserved here. The wrapper now contains
     * both the Framework review component and the unchanged product body. */
    // return workstation != NULL
    // ? umi_application_product_gtk4_workstation_widget(
    // workstation->framework_workstation)
    // : NULL;
    return workstation != NULL ? workstation->root : NULL;
}

/* Layout changes remain Framework operations so Bank stores no geometry. */
UmiStatus umi_bank_gtk_workstation_select_layout(
    UmiBankGtkWorkstation *workstation,
    const char *layout_id)
{
    return workstation != NULL
        ? umi_application_product_gtk4_workstation_select_layout(
              workstation->framework_workstation, layout_id)
        : UMI_STATUS_INVALID_ARGUMENT;
}

/* Diagnostics receive copied evidence rather than mutable product objects. */
UmiStatus umi_bank_gtk_workstation_snapshot(
    const UmiBankGtkWorkstation *workstation,
    UmiApplicationProductGtk4WorkstationSnapshot *out_snapshot)
{
    return workstation != NULL
        ? umi_application_product_gtk4_workstation_snapshot(
              workstation->framework_workstation, out_snapshot)
        : UMI_STATUS_INVALID_ARGUMENT;
}

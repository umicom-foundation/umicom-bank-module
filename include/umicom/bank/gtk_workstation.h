/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: include/umicom/bank/gtk_workstation.h
 *
 * PURPOSE:
 *   Expose the thin native Bank composition over the Framework-owned product
 *   workstation, presentation controllers and customisable suite layouts.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_BANK_GTK_WORKSTATION_H
#define UMICOM_BANK_GTK_WORKSTATION_H

#include <gtk/gtk.h>

#include "umicom/application/suite_layout/gtk4_product_workstation.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Represent the bank gtk workstation data shared with callers of this public contract.
 */
typedef struct UmiBankGtkWorkstation UmiBankGtkWorkstation;

/** Create Bank's thin shell over the shared native product workstation. */
UmiStatus umi_bank_gtk_workstation_create(
    UmiBankGtkWorkstation **out_workstation);
/** Release the thin shell and the Framework services which it owns. */
void umi_bank_gtk_workstation_destroy(UmiBankGtkWorkstation *workstation);
/** Borrow the Framework root widget for a native application window. */
GtkWidget *umi_bank_gtk_workstation_widget(
    UmiBankGtkWorkstation *workstation);
/** Select one canonical banking, global-money or digital-assets layout. */
UmiStatus umi_bank_gtk_workstation_select_layout(
    UmiBankGtkWorkstation *workstation,
    const char *layout_id);
/** Copy current layout, runtime and panel-coverage evidence. */
UmiStatus umi_bank_gtk_workstation_snapshot(
    const UmiBankGtkWorkstation *workstation,
    UmiApplicationProductGtk4WorkstationSnapshot *out_snapshot);

#ifdef __cplusplus
}
#endif

#endif

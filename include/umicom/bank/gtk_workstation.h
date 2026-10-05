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
/** Bind the existing shared product identity to a native window before its
 * first realization. The Framework owns titlebar composition and lifetime;
 * no application catalogue or appearance state is duplicated. */
UmiStatus umi_bank_gtk_workstation_bind_window(
    UmiBankGtkWorkstation *workstation, GtkWindow *window);
/** Explicitly enable shared user-local SQLite layout checkpoints.
 * Native launchers opt in after construction; constructors do no checkpoint I/O.
 * A failed restore leaves the current layout visible and reports its error. */
UmiStatus umi_bank_gtk_workstation_enable_checkpoint_storage(
    UmiBankGtkWorkstation *workstation, int restore_saved);

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

/** Explicitly bind a borrowed server for layout checkpoints only. It must
 * outlive the workstation or be unbound with NULL first. No banking ledger
 * is attached or modified; cached storage evidence is reset by Framework. */
UmiStatus umi_bank_gtk_workstation_bind_checkpoint_storage(UmiBankGtkWorkstation *workstation, UmiDataServer *server);
/** Copy named layouts on the GTK owner thread; no I/O or product command. */
UmiStatus umi_bank_gtk_workstation_library_snapshot(UmiBankGtkWorkstation *workstation, UmiUiWorkspaceLibrarySnapshot *out_snapshot);
/** Read a saved layout comparison without restoring, posting or saving data.
 * Later Restore rereads storage. Failure leaves output unchanged. */
UmiStatus umi_bank_gtk_workstation_library_preview(UmiBankGtkWorkstation *workstation, UmiUiWorkspaceLibraryPreview *out_preview);

/* Layout recovery stays with Framework. This owner-thread call neither
 * saves product data nor runs a trade, payment or project command. */
UmiStatus umi_bank_gtk_workstation_library_history_read(UmiBankGtkWorkstation *workstation,
    UmiUiWorkspaceLibraryHistoryState *out_state);
/* Layout recovery stays with Framework. This owner-thread call neither
 * saves product data nor runs a trade, payment or project command. */
UmiStatus umi_bank_gtk_workstation_library_history_navigate(UmiBankGtkWorkstation *workstation,
    UmiUiWorkspaceLibraryHistoryDirection direction, uint64_t expected_revision);
/* Layout recovery stays with Framework. This owner-thread call neither
 * saves product data nor runs a trade, payment or project command. */
UmiStatus umi_bank_gtk_workstation_library_export(UmiBankGtkWorkstation *workstation,
    char *bytes, size_t capacity, size_t *out_size);
/* Layout recovery stays with Framework. This owner-thread call neither
 * saves product data nor runs a trade, payment or project command. */
UmiStatus umi_bank_gtk_workstation_library_import_review(UmiBankGtkWorkstation *workstation,
    const void *bytes, size_t size, UmiUiWorkspaceLibraryImport **out_review);
/* Layout recovery stays with Framework. This owner-thread call neither
 * saves product data nor runs a trade, payment or project command. */
UmiStatus umi_bank_gtk_workstation_library_import_apply(UmiBankGtkWorkstation *workstation,
    const UmiUiWorkspaceLibraryImport *review);
/* Layout recovery stays with Framework. This owner-thread call neither
 * saves product data nor runs a trade, payment or project command. */
UmiStatus umi_bank_gtk_workstation_library_apply(UmiBankGtkWorkstation *workstation,
    const UmiUiWorkspaceLibraryRequest *request);

#ifdef __cplusplus
}
#endif

#endif

/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: src/gtk/main.c
 *
 * PURPOSE:
 *   Start the native Bank workstation through the Framework-owned startup
 *   surface and then transfer the application window to the completed product
 *   workspace.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include <gtk/gtk.h>
#include <stdio.h>

#include "umicom/bank/gtk_workstation.h"
#include "umicom/ui/gtk4/workstation/shell_header.h"

typedef struct UmiBankGtkApplicationState {
    GtkWindow *window;
    UmiBankGtkWorkstation *workstation;
    UmiGtk4WorkstationStartupSplash *splash;
    guint startup_source_id;
} UmiBankGtkApplicationState;

/* Clear the borrowed window pointer when the native window is destroyed. */
static void on_window_destroyed(gpointer data, GObject *where_the_object_was)
{
    UmiBankGtkApplicationState *state =
        (UmiBankGtkApplicationState *)data;

    (void)where_the_object_was;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (state != NULL) state->window = NULL;
}

/* Finish product construction after GTK has had an opportunity to present the
 * startup surface. A failure remains readable in the same window. */
static gboolean complete_startup(gpointer user_data)
{
    UmiBankGtkApplicationState *state =
        (UmiBankGtkApplicationState *)user_data;
    GtkWidget *content;
    UmiStatus status;

    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (state == NULL) return G_SOURCE_REMOVE;
    state->startup_source_id = 0U;
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (state->window == NULL || state->splash == NULL) {
        return G_SOURCE_REMOVE;
    }

    (void)umi_gtk4_ws_startup_splash_set_status(
        state->splash,
        "Preparing accounts, payments and workspace services…",
        "Offline");
    (void)umi_gtk4_ws_startup_splash_set_progress(
        state->splash, 0.55, 1);
    status = umi_bank_gtk_workstation_create(&state->workstation);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        char message[192U];

        (void)snprintf(
            message,
            sizeof(message),
            "Bank startup could not complete: %s",
            umi_status_text(status));
        (void)umi_gtk4_ws_startup_splash_set_status(
            state->splash, message, "Action required");
        (void)umi_gtk4_ws_startup_splash_set_progress(
            state->splash, 1.0, 0);
        (void)fprintf(
            stderr, "Umicom Bank workstation failed: %s\n",
            umi_status_text(status));
        return G_SOURCE_REMOVE;
    }

    content = umi_bank_gtk_workstation_widget(state->workstation);
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (content == NULL) {
        (void)umi_gtk4_ws_startup_splash_set_status(
            state->splash,
            "Bank startup completed without a visible workspace.",
            "Action required");
        (void)umi_gtk4_ws_startup_splash_set_progress(
            state->splash, 1.0, 0);
        return G_SOURCE_REMOVE;
    }

    (void)umi_gtk4_ws_startup_splash_set_status(
        state->splash, "Bank workspace is ready", "Offline");
    (void)umi_gtk4_ws_startup_splash_set_progress(
        state->splash, 1.0, 1);
    gtk_window_set_child(state->window, content);
    umi_gtk4_ws_startup_splash_destroy(state->splash);
    state->splash = NULL;
    return G_SOURCE_REMOVE;
}

/* Present the startup surface immediately, then construct the heavier product
 * workstation from the main context. */
static void on_activate(GtkApplication *application, gpointer user_data)
{
    UmiBankGtkApplicationState *state =
        (UmiBankGtkApplicationState *)user_data;
    UmiGtk4WorkstationStartupSplashConfig splash_config;
    UmiStatus status;

    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (state == NULL) return;
    /* Apply this branch only when its contract condition is satisfied. */
    if (state->window != NULL) {
        gtk_window_present(state->window);
        return;
    }

    state->window = GTK_WINDOW(gtk_application_window_new(application));
    gtk_window_set_title(state->window, "Umicom Bank");
    gtk_window_set_default_size(state->window, 1180, 760);
    g_object_weak_ref(
        G_OBJECT(state->window), on_window_destroyed, state);

    splash_config = umi_gtk4_ws_startup_splash_config_default(
        "org.umicom.bank", "Umicom Bank");
    splash_config.subtitle =
        "Accounts, payments, liquidity and financial operations";
    splash_config.status = "Starting secure application services…";
    splash_config.mode_badge = "Offline";
    status = umi_gtk4_ws_startup_splash_create(
        &splash_config, &state->splash);
    /* Preserve the original failure result so the caller can respond to the correct cause. */
    if (status != UMI_STATUS_OK) {
        (void)fprintf(
            stderr, "Umicom Bank startup surface failed: %s\n",
            umi_status_text(status));
        gtk_window_destroy(state->window);
        return;
    }

    gtk_window_set_child(
        state->window,
        umi_gtk4_ws_startup_splash_widget(state->splash));
    gtk_window_present(state->window);
    state->startup_source_id = g_timeout_add(
        80U, complete_startup, state);
}

/* Release the Framework-owned startup and product controllers in reverse
 * creation order after the native application loop ends. */
static void application_state_dispose(UmiBankGtkApplicationState *state)
{
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (state == NULL) return;
    /* Apply this branch only when its contract condition is satisfied. */
    if (state->startup_source_id != 0U) {
        (void)g_source_remove(state->startup_source_id);
        state->startup_source_id = 0U;
    }
    umi_gtk4_ws_startup_splash_destroy(state->splash);
    state->splash = NULL;
    umi_bank_gtk_workstation_destroy(state->workstation);
    state->workstation = NULL;
    state->window = NULL;
}

int main(int argc, char **argv)
{
    GtkApplication *application;
    UmiBankGtkApplicationState state = {0};
    int result;

    application = gtk_application_new(
        "org.umicom.bank.gtk",
        G_APPLICATION_DEFAULT_FLAGS);
    /*
     * Protect caller-owned memory by checking that required state is available before it is
     * used.
     */
    if (application == NULL) return 1;
    g_signal_connect(
        application, "activate", G_CALLBACK(on_activate), &state);
    result = g_application_run(G_APPLICATION(application), argc, argv);
    application_state_dispose(&state);
    g_object_unref(application);
    return result;
}

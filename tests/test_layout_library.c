/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: tests/test_layout_library.c
 * PURPOSE: Verify Bank composes shared layout activation, storage and read-only preview.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/bank/gtk_workstation.h"
#include <stdio.h>
#include <string.h>
#define CHECK(x) do { if(!(x)){fprintf(stderr,"%s:%d: %s\n",__FILE__,__LINE__,#x);failed=1;goto cleanup;} } while(0)
static GtkWidget *Find(GtkWidget *root,const char *id)
{
    if(root==NULL)return NULL;
    if(g_strcmp0(g_object_get_data(G_OBJECT(root),"umicom-automation-id"),id)==0)return root;
    for(GtkWidget *child=gtk_widget_get_first_child(root);child!=NULL;child=gtk_widget_get_next_sibling(child)){
        GtkWidget *found=Find(child,id);if(found!=NULL)return found;
    }return NULL;
}
static void Drain(void){for(size_t i=0;i<128 && g_main_context_pending(NULL);++i)(void)g_main_context_iteration(NULL,FALSE);}
int main(void)
{
    if(!gtk_init_check())return 77;
    UmiBankGtkWorkstation *bank=NULL;UmiDataServer *server=NULL;
    UmiUiWorkspaceLibrarySnapshot before,after;UmiUiWorkspaceLibraryPreview preview;
    UmiApplicationProductGtk4WorkstationSnapshot product_before,product_after;
    int failed=0;size_t target=SIZE_MAX;
    CHECK(umi_bank_gtk_workstation_create(&bank)==UMI_STATUS_OK);
    CHECK(umi_data_server_create_memory(&server)==UMI_STATUS_OK);
    CHECK(umi_bank_gtk_workstation_bind_checkpoint_storage(bank,server)==UMI_STATUS_OK);
    CHECK(umi_bank_gtk_workstation_library_snapshot(bank,&before)==UMI_STATUS_OK && before.layout_count>1);
    GtkWidget *menu=Find(umi_bank_gtk_workstation_widget(bank),"umicom.layout.library");CHECK(GTK_IS_MENU_BUTTON(menu));
    GtkWidget *popover=GTK_WIDGET(gtk_menu_button_get_popover(GTK_MENU_BUTTON(menu)));
    GtkWidget *list=Find(popover,"workstation.layout-library.list"),*save=Find(popover,"workstation.layout-library.save-library");
    CHECK(GTK_IS_LIST_BOX(list) && GTK_IS_BUTTON(save));CHECK(!gtk_list_box_get_activate_on_single_click(GTK_LIST_BOX(list)));
    for(size_t i=0;i<before.layout_count;++i)if(!before.rows[i].active){target=i;break;}
    CHECK(target!=SIZE_MAX);GtkListBoxRow *row=gtk_list_box_get_row_at_index(GTK_LIST_BOX(list),(int)target);
    CHECK(row!=NULL);g_signal_emit_by_name(list,"row-activated",row);Drain();
    CHECK(umi_bank_gtk_workstation_library_snapshot(bank,&after)==UMI_STATUS_OK && after.rows[target].active);
    CHECK(after.customisation_revision==before.customisation_revision+1);
    g_signal_emit_by_name(save,"clicked");Drain();CHECK(umi_data_server_count(server)>0);
    CHECK(umi_bank_gtk_workstation_snapshot(bank,&product_before)==UMI_STATUS_OK);
    size_t records=umi_data_server_count(server);
    CHECK(umi_bank_gtk_workstation_library_preview(bank,&preview)==UMI_STATUS_OK);
    CHECK(preview.comparison.changed_count==0 && preview.comparison.added_count==0 && preview.comparison.removed_count==0);
    CHECK(preview.saved.rows[target].active && umi_data_server_count(server)==records);
    CHECK(umi_bank_gtk_workstation_snapshot(bank,&product_after)==UMI_STATUS_OK);
    CHECK(product_after.surface.revision==product_before.surface.revision);
    CHECK(product_after.surface.dirty_count==product_before.surface.dirty_count);
    CHECK(product_after.surface.guarded_command_count==product_before.surface.guarded_command_count);
    CHECK(umi_bank_gtk_workstation_bind_checkpoint_storage(bank,NULL)==UMI_STATUS_OK);
cleanup:
    umi_bank_gtk_workstation_destroy(bank);umi_data_server_destroy(server);return failed;
}

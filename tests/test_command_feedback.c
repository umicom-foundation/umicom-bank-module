/*-----------------------------------------------------------------------------
 * Umicom Framework
 * File: tests/test_command_feedback.c
 * PURPOSE: Check actual bank composition rejects commands without a connected service.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/
#include "umicom/bank/application_surface.h"
#include "umicom/application/presentation/presentation.h"
#include <stdio.h>
#include <string.h>
#define REQUIRE(c) do { if (!(c)) { fprintf(stderr, "%d: %s\n",__LINE__,#c); result=1; goto cleanup; } } while(0)
int main(int argc,char **argv)
{
    UmiBankApplicationSurface *surface=NULL;
    UmiApplicationPresentationSurfaceRuntime *runtime;
    UmiApplicationPresentationSurfaceItem *item=NULL;
    size_t index;int result=0;
    if(argc!=2)return 2;
    REQUIRE(umi_bank_application_surface_create(&surface)==UMI_STATUS_OK);
    runtime=umi_bank_application_surface_runtime(surface);
    for(index=0;index<runtime->session.item_count;++index){
        UmiApplicationPresentationSurfaceItem *candidate=&runtime->session.items[index];
        if(candidate->visible && candidate->behavior->command_mode!=UMI_APPLICATION_PRESENTATION_COMMAND_READ_ONLY){item=candidate;break;}
    }
    REQUIRE(item!=NULL);
    if(strcmp(argv[1],"dirty")==0)item->dirty=1;
    REQUIRE(umi_bank_application_surface_command(surface,item->placement->panel->component_id,
        "test.request")==UMI_STATUS_NOT_IMPLEMENTED);
    REQUIRE(item->state==UMI_APPLICATION_PRESENTATION_STATE_OFFLINE);
    REQUIRE(strstr(item->message,"No ")!=NULL && strstr(item->message,"created")!=NULL);
    REQUIRE(strstr(item->message,"staged")==NULL && strcmp(item->badge,"SAFE")!=0);
    if(strcmp(argv[1],"dirty")==0)REQUIRE(item->dirty==1);
    if(strcmp(argv[1],"refresh")==0){
        REQUIRE(umi_bank_application_surface_refresh(surface)==UMI_STATUS_OK);
        REQUIRE(item->state==UMI_APPLICATION_PRESENTATION_STATE_EMPTY);
    }
cleanup:
    umi_bank_application_surface_destroy(surface);return result;
}

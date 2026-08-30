/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: tests/test_application_surface.c
 *
 * PURPOSE:
 *   Verify Bank starts standard, focus and learning layouts through the shared
 *   Framework product surface with no live service dependency.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/bank/application_surface.h"

#include <assert.h>

int main(void)
{
    const UmiApplicationComponentRecipeAudience audiences[] = {
        UMI_APPLICATION_COMPONENT_RECIPE_AUDIENCE_LEARNING,
        UMI_APPLICATION_COMPONENT_RECIPE_AUDIENCE_STANDARD,
        UMI_APPLICATION_COMPONENT_RECIPE_AUDIENCE_FOCUS};
    size_t index;
    for (index = 0U; index < sizeof(audiences) / sizeof(audiences[0]); ++index) {
        UmiBankApplicationSurface *surface = NULL;
        UmiApplicationPresentationSurfaceSnapshot snapshot;
        assert(umi_bank_application_surface_create_for_audience(
                   audiences[index], &surface) == UMI_STATUS_OK);
        assert(umi_bank_application_surface_snapshot(surface, &snapshot) ==
               UMI_STATUS_OK);
        assert(snapshot.panel_count > 0U);
        assert(snapshot.visible_count > 0U);
        umi_bank_application_surface_destroy(surface);
    }
    return 0;
}

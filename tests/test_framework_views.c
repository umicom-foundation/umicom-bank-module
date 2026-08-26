/*-----------------------------------------------------------------------------
 * Umicom Bank Module
 * File: tests/test_framework_views.c
 *
 * PURPOSE:
 *   Verify Bank renders Framework banking snapshots through the shared finance UI projection.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/
#include <assert.h>

#include "umicom/bank/runtime.h"

int main(void)
{
    UmiBankingBankingSnapshot snapshot;
    UmiUiViewModel *view = NULL;
    assert(umi_banking_banking_snapshot_init(&snapshot, "bank", 2U, 3U, 1U,
        90000, 20000) == UMI_STATUS_OK);
    assert(umi_bank_runtime_create_overview_view(&snapshot, &view) == UMI_STATUS_OK);
    assert(view != NULL);
    umi_ui_view_model_destroy(view);
    return 0;
}

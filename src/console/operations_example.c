/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: src/console/operations_example.c
 *
 * PURPOSE:
 *   Demonstrate the thin Bank-to-Framework customer, account, approval and transfer workflow.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/bank/operations.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

/* This example composes public commands as a client would. It neither writes
 * SQL nor calculates a balance itself, and it never opens the user's database. */
int main(void)
{
    UmiBankOperations *operations = NULL;
    UmiBankActor maker = {{{0}}, UMI_BANK_CAP_ALL}, checker = {{{0}}, UMI_BANK_CAP_APPROVE};
    UmiBankCommand command; UmiBankReceipt receipt; UmiBankBalance source, destination;
    UmiStatus status = UmiBankOpenVolatileOperations(&operations);
    if (status != UMI_STATUS_OK) return 1;
    (void)umi_financial_id_assign(&maker.id, "example-maker");
    (void)umi_financial_id_assign(&checker.id, "example-checker");
    for (unsigned step = 0U; step < 8U && status == UMI_STATUS_OK; ++step) {
        UmiBankCounts counts;
        status = UmiBankOperationsCounts(operations, &counts);
        if (status != UMI_STATUS_OK) break;
        UmiBankCommandInit(&command, UMI_BANK_CUSTOMER_CREATE);
        (void)snprintf(command.requestId.value, sizeof command.requestId.value, "example-request-%u", step);
        command.businessDate = (UmiFinancialDate){2026, 9U, 25U};
        command.timestampMillis = (int64_t)step; command.expectedRevision = counts.revision;
        if (step == 0U) {
            (void)umi_financial_id_assign(&command.id, "example-customer"); strcpy(command.name, "Umicom Example Customer");
        } else if (step == 1U || step == 2U) {
            command.action = UMI_BANK_ACCOUNT_OPEN;
            (void)umi_financial_id_assign(&command.id, step == 1U ? "example-current" : "example-savings");
            (void)umi_financial_id_assign(&command.ownerId, "example-customer"); strcpy(command.name, "Example account");
            (void)umi_accounting_currency_from_code("GBP", &command.amount.currency); command.amount.scale = 2U;
        } else if (step == 3U) {
            command.action = UMI_BANK_TEST_CREDIT; (void)umi_financial_id_assign(&command.id, "example-funding");
            (void)umi_financial_id_assign(&command.sourceAccountId, "example-current");
            command.amount.minor_units = 100000; command.amount.scale = 2U;
            (void)umi_accounting_currency_from_code("GBP", &command.amount.currency);
        } else if (step == 4U) {
            command.action = UMI_BANK_BENEFICIARY_CREATE; (void)umi_financial_id_assign(&command.id, "example-beneficiary");
            (void)umi_financial_id_assign(&command.ownerId, "example-customer");
            (void)umi_financial_id_assign(&command.destinationAccountId, "example-savings"); strcpy(command.name, "Example savings beneficiary");
        } else {
            (void)umi_financial_id_assign(&command.id, "example-transfer");
            command.action = step == 5U ? UMI_BANK_TRANSFER_SUBMIT : step == 6U ? UMI_BANK_TRANSFER_APPROVE : UMI_BANK_TRANSFER_EXECUTE;
            if (step == 5U) {
                (void)umi_financial_id_assign(&command.ownerId, "example-beneficiary");
                (void)umi_financial_id_assign(&command.sourceAccountId, "example-current");
                command.amount.minor_units = 25000; command.amount.scale = 2U;
                (void)umi_accounting_currency_from_code("GBP", &command.amount.currency);
            }
        }
        status = UmiBankOperationsExecute(operations, step == 6U ? &checker : &maker, &command, &receipt);
    }
    if (status == UMI_STATUS_OK) status = UmiBankOperationsBalance(operations, "example-current", &source);
    if (status == UMI_STATUS_OK) status = UmiBankOperationsBalance(operations, "example-savings", &destination);
    if (status == UMI_STATUS_OK && (source.booked.minor_units != 75000 || destination.booked.minor_units != 25000 ||
        source.reserved.minor_units != 0 || destination.reserved.minor_units != 0)) status = UMI_STATUS_INTERNAL_ERROR;
    if (status == UMI_STATUS_OK) {
        (void)printf("PASS: Framework-owned local banking example.\n"
            "Current: GBP %" PRId64 " minor; savings: GBP %" PRId64 " minor.\n"
            "A distinct checker approved the transfer. No file, card provider or payment network was used.\n",
            source.booked.minor_units, destination.booked.minor_units);
    } else (void)fprintf(stderr, "Bank composition example failed with status %d\n", (int)status);
    UmiBankOperationsDestroy(operations);
    return status == UMI_STATUS_OK ? 0 : 1;
}

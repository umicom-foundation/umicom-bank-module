/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: src/console/reconciliation_example.c
 * PURPOSE: Teach recorded mismatches, evidence review and reopening through the real product service.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/bank/operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The fixed memory-only lesson explicitly elects each action. Interactive
 * applications must wait for a separate user Submit after presenting review. */
static UmiStatus LessonCommand(UmiBankOperations *bank, const UmiBankActor *actor,
    UmiBankCommand *command, unsigned serial, int showReview)
{
    UmiBankCounts counts; UmiStatus status = UmiBankOperationsCounts(bank, &counts);
    if (status != UMI_STATUS_OK) return status;
    command->expectedRevision = counts.revision;
    command->businessDate = (UmiFinancialDate){2026, 10, 1}; command->timestampMillis = serial;
    (void)snprintf(command->requestId.value, sizeof command->requestId.value, "reconciliation-lesson-%u", serial);
    UmiBankReview *review = NULL; UmiBankReceipt receipt;
    status = UmiBankOperationsReview(bank, actor, command, &review);
    if (status == UMI_STATUS_OK && showReview) {
        char *text = malloc(UMI_BANK_REVIEW_TEXT_CAPACITY);
        if (text == NULL) status = UMI_STATUS_OUT_OF_MEMORY;
        else {
            status = UmiBankReviewDescribe(review, text, UMI_BANK_REVIEW_TEXT_CAPACITY, NULL);
            if (status == UMI_STATUS_OK) puts(text);
            free(text);
        }
    }
    if (status == UMI_STATUS_OK) status = UmiBankOperationsExecuteReviewed(bank, actor, review, &receipt);
    UmiBankReviewDestroy(review); return status;
}

/* Record a zero-balance account, an erroneous observation, and its correction. */
int main(void)
{
    UmiBankOperations *bank = NULL; UmiBankCommand command;
    UmiBankActor maker = {0}, operator = {0}; UmiBankReconciliation record;
    UmiStatus status; int result = 1;
#define STEP(call) do { status = (call); if (status != UMI_STATUS_OK) { fprintf(stderr, "Reconciliation lesson: %s\n", umi_status_text(status)); goto done; } } while (0)
#define EXPECT(condition) do { if (!(condition)) { fprintf(stderr, "Reconciliation lesson mismatch: %s\n", #condition); goto done; } } while (0)
    STEP(UmiBankOpenVolatileOperations(&bank));
    STEP(umi_financial_id_assign(&maker.id, "lesson-maker")); maker.capabilities = UMI_BANK_CAP_CUSTOMERS;
    STEP(umi_financial_id_assign(&operator.id, "lesson-operator")); operator.capabilities = UMI_BANK_CAP_OPERATE;
    UmiBankCommandInit(&command, UMI_BANK_CUSTOMER_CREATE);
    strcpy(command.id.value, "customer"); strcpy(command.name, "Reconciliation learner");
    STEP(LessonCommand(bank, &maker, &command, 1U, 0));
    UmiBankCommandInit(&command, UMI_BANK_ACCOUNT_OPEN);
    strcpy(command.id.value, "account"); strcpy(command.ownerId.value, "customer");
    strcpy(command.name, "Practice account"); memcpy(command.amount.currency.code, "GBP", 4U); command.amount.scale = 2U;
    STEP(LessonCommand(bank, &maker, &command, 2U, 0));
    UmiBankCommandInit(&command, UMI_BANK_RECONCILE);
    strcpy(command.id.value, "break"); strcpy(command.sourceAccountId.value, "account");
    memcpy(command.amount.currency.code, "GBP", 4U); command.amount.scale = 2U; command.amount.minor_units = 100;
    STEP(LessonCommand(bank, &operator, &command, 3U, 0));
    STEP(UmiBankFindReconciliation(bank, "break", &record));
    EXPECT(!record.matched && record.externalBalance.minor_units == 100 && record.bookedBalance.minor_units == 0);
    puts("The comparison records a GBP 1.00 difference. It does not credit the account.");
    UmiBankCommandInit(&command, UMI_BANK_RECONCILE);
    strcpy(command.id.value, "match"); strcpy(command.sourceAccountId.value, "account");
    memcpy(command.amount.currency.code, "GBP", 4U); command.amount.scale = 2U;
    STEP(LessonCommand(bank, &operator, &command, 4U, 0));
    UmiBankCommandInit(&command, UMI_BANK_RECONCILIATION_RESOLVE);
    strcpy(command.id.value, "break"); strcpy(command.ownerId.value, "match");
    strcpy(command.name, "Corrected the observed statement balance after investigation");
    STEP(LessonCommand(bank, &operator, &command, 5U, 1));
    STEP(UmiBankFindReconciliation(bank, "break", &record));
    EXPECT(record.disposition == UMI_BANK_RECONCILIATION_RESOLVED && !record.matched && record.externalBalance.minor_units == 100);
    UmiBankCommandInit(&command, UMI_BANK_RECONCILIATION_REOPEN);
    strcpy(command.id.value, "break"); strcpy(command.name, "New information requires another comparison");
    STEP(LessonCommand(bank, &operator, &command, 6U, 1));
    STEP(UmiBankFindReconciliation(bank, "break", &record));
    EXPECT(record.disposition == UMI_BANK_RECONCILIATION_REOPENED && strcmp(record.evidenceId.value, "match") == 0);
    UmiBankBalance balance; UmiBankCounts counts;
    STEP(UmiBankOperationsBalance(bank, "account", &balance)); STEP(UmiBankOperationsCounts(bank, &counts));
    EXPECT(balance.booked.minor_units == 0 && balance.reserved.minor_units == 0 && counts.journals == 0U && counts.events == 6U);
    puts("The break is reopened. Original evidence and every review remain recorded; the balance is still zero.");
    puts("Resolving again requires a new matching comparison recorded after the reopen.");
    result = 0;
done:
    UmiBankOperationsDestroy(bank); return result;
#undef STEP
#undef EXPECT
}

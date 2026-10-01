/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: src/console/charges_example.c
 * PURPOSE: Compose reviewed local charges with the Framework journal owner.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/


#include "umicom/bank/operations.h"
#include "umicom/bank_operations/review.h"
#include "umicom/bank_operations/statement_text.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This complete lesson selects memory storage through Bank's product entry
 * point. Framework owns validation, approval, money and journal persistence. */
static UmiStatus Reviewed(UmiBankOperations *bank, const UmiBankActor *actor,
    UmiBankCommand *command, uint64_t serial)
{
    UmiBankCounts counts;
    UmiBankReview *review = NULL;
    UmiBankReceipt receipt;
    UmiStatus status = UmiBankOperationsCounts(bank, &counts);
    if (status != UMI_STATUS_OK) return status;
    command->expectedRevision = counts.revision;
    command->businessDate = (UmiFinancialDate){2026,9,30};
    command->timestampMillis = (int64_t)serial;
    (void)snprintf(command->requestId.value, sizeof(command->requestId.value),
        "charge-lesson-%" PRIu64, serial);
    status = UmiBankOperationsReview(bank, actor, command, &review);
    if (status == UMI_STATUS_OK && command->action >= UMI_BANK_CHARGE_SUBMIT) {
        char *text = malloc(UMI_BANK_REVIEW_TEXT_CAPACITY);
        if (text == NULL) status = UMI_STATUS_OUT_OF_MEMORY;
        else {
            status = UmiBankReviewDescribe(review, text, UMI_BANK_REVIEW_TEXT_CAPACITY, NULL);
            if (status == UMI_STATUS_OK) puts(text);
            free(text);
        }
    }
    if (status == UMI_STATUS_OK)
        status = UmiBankOperationsExecuteReviewed(bank, actor, review, &receipt);
    UmiBankReviewDestroy(review);
    return status;
}

int main(void)
{
    UmiBankOperations *bank = NULL;
    UmiBankActor maker = {0}, checker = {0}, operator = {0};
    UmiBankCommand command;
    UmiBankBalance balance;
    UmiBankChargeRequest charge;
    UmiBankCounts counts;
    UmiStatus status;
    uint64_t serial = 0;
    char *statement = NULL;
    int failed = 1;
#define STEP(call) do { status = (call); if (status != UMI_STATUS_OK) { \
    fprintf(stderr, "Charge lesson failed: %s\n", umi_status_text(status)); goto cleanup; } } while (0)
#define EXPECT(condition) do { if (!(condition)) { \
    fprintf(stderr, "Charge lesson result mismatch: %s\n", #condition); goto cleanup; } } while (0)
    STEP(umi_financial_id_assign(&maker.id, "lesson-maker"));
    maker.capabilities = UMI_BANK_CAP_CUSTOMERS | UMI_BANK_CAP_PAYMENTS;
    STEP(umi_financial_id_assign(&checker.id, "lesson-checker"));
    checker.capabilities = UMI_BANK_CAP_APPROVE;
    STEP(umi_financial_id_assign(&operator.id, "lesson-operator"));
    operator.capabilities = UMI_BANK_CAP_OPERATE | UMI_BANK_CAP_TEST_FUNDING;
    STEP(UmiBankOpenVolatileOperations(&bank));

    UmiBankCommandInit(&command, UMI_BANK_CUSTOMER_CREATE);
    strcpy(command.id.value, "learner"); strcpy(command.name, "Practice customer");
    STEP(Reviewed(bank, &maker, &command, ++serial));
    UmiBankCommandInit(&command, UMI_BANK_ACCOUNT_OPEN);
    strcpy(command.id.value, "account"); strcpy(command.ownerId.value, "learner");
    strcpy(command.name, "Practice account");
    memcpy(command.amount.currency.code, "GBP", 4); command.amount.scale = 2;
    STEP(Reviewed(bank, &maker, &command, ++serial));
    UmiBankCommandInit(&command, UMI_BANK_TEST_CREDIT);
    strcpy(command.id.value, "funding"); strcpy(command.sourceAccountId.value, "account");
    memcpy(command.amount.currency.code, "GBP", 4);
    command.amount.scale = 2; command.amount.minor_units = 100000;
    STEP(Reviewed(bank, &operator, &command, ++serial));

    UmiBankCommandInit(&command, UMI_BANK_CHARGE_SUBMIT);
    strcpy(command.id.value, "service-charge"); strcpy(command.ownerId.value, "statement-2026-09");
    strcpy(command.sourceAccountId.value, "account"); strcpy(command.name, "Practice service charge");
    memcpy(command.amount.currency.code, "GBP", 4);
    command.amount.scale = 2; command.amount.minor_units = 250;
    STEP(Reviewed(bank, &maker, &command, ++serial));
    UmiBankCommandInit(&command, UMI_BANK_CHARGE_APPROVE);
    strcpy(command.id.value, "service-charge");
    STEP(Reviewed(bank, &checker, &command, ++serial));
    STEP(UmiBankOperationsBalance(bank, "account", &balance));
    EXPECT(balance.booked.minor_units == 100000 && balance.reserved.minor_units == 0);

    UmiBankCommandInit(&command, UMI_BANK_CHARGE_POST);
    strcpy(command.id.value, "service-charge");
    STEP(Reviewed(bank, &operator, &command, ++serial));
    STEP(UmiBankOperationsBalance(bank, "account", &balance));
    EXPECT(balance.booked.minor_units == 99750 && balance.available.minor_units == 99750);
    STEP(UmiBankOperationsChargeAt(bank, 0, &charge));
    EXPECT(charge.state == UMI_BANK_TRANSFER_EXECUTED && charge.postedRevision == 6);

    UmiBankCommandInit(&command, UMI_BANK_CHARGE_REVERSE);
    strcpy(command.id.value, "service-charge");
    STEP(Reviewed(bank, &operator, &command, ++serial));
    STEP(UmiBankOperationsBalance(bank, "account", &balance));
    EXPECT(balance.booked.minor_units == 100000 && balance.reserved.minor_units == 0);
    STEP(UmiBankOperationsCounts(bank, &counts));
    EXPECT(counts.journals == 3 && counts.chargeRequests == 1 && counts.revision == 7);
    statement = malloc(UMI_BANK_STATEMENT_TEXT_CAPACITY);
    if (statement == NULL) goto cleanup;
    STEP(UmiBankOperationsDescribeStatement(bank, "account", 6, 7,
        statement, UMI_BANK_STATEMENT_TEXT_CAPACITY, NULL));
    puts(statement);
    puts("The GBP 2.50 practice charge was separately approved, posted and reversed. Both journals remain. No data was written to disk.");
    failed = 0;
cleanup:
    free(statement);
    UmiBankOperationsDestroy(bank);
    return failed;
#undef STEP
#undef EXPECT
}

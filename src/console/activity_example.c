/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: src/console/activity_example.c
 * PURPOSE: Teach captured account activity through the thin Bank facade and exact money.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/bank/operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* This lesson uses volatile practice data and never opens a database or
 * connects to a financial institution. Its actor is supplied by trusted code. */
#define REQUIRE(x) do { if (!(x)) { fprintf(stderr, "Lesson failed at line %d: %s\n", __LINE__, #x); exit(1); } } while (0)
static UmiBankCommand Command(UmiBankOperations *operations, UmiBankAction action,
    const char *id, unsigned sequence, unsigned day)
{
    UmiBankCommand command; UmiBankCounts counts;
    REQUIRE(UmiBankOperationsCounts(operations, &counts) == UMI_STATUS_OK);
    UmiBankCommandInit(&command, action);
    REQUIRE(umi_financial_id_assign(&command.id, id) == UMI_STATUS_OK);
    (void)snprintf(command.requestId.value, sizeof command.requestId.value, "activity-lesson-%u", sequence);
    command.businessDate = (UmiFinancialDate){2026,10,(uint8_t)day};
    command.timestampMillis = sequence; command.expectedRevision = counts.revision;
    return command;
}
static UmiMoney Pounds(int64_t minor)
{ UmiMoney value = {0}; memcpy(value.currency.code, "GBP", 4); value.scale = 2; value.minor_units = minor; return value; }
static void Submit(UmiBankOperations *operations, const UmiBankActor *actor, UmiBankCommand command)
{ UmiBankReceipt receipt; REQUIRE(UmiBankOperationsExecute(operations, actor, &command, &receipt) == UMI_STATUS_OK); }
int main(void)
{
    UmiBankOperations *operations = NULL; REQUIRE(UmiBankOpenVolatileOperations(&operations) == UMI_STATUS_OK);
    UmiBankActor actor = {0}; REQUIRE(umi_financial_id_assign(&actor.id, "lesson-operator") == UMI_STATUS_OK);
    actor.capabilities = UMI_BANK_CAP_ALL;
    UmiBankCommand command = Command(operations, UMI_BANK_CUSTOMER_CREATE, "customer", 1, 1);
    strcpy(command.name, "Activity lesson"); Submit(operations, &actor, command);
    command = Command(operations, UMI_BANK_ACCOUNT_OPEN, "account", 2, 1);
    REQUIRE(umi_financial_id_assign(&command.ownerId, "customer") == UMI_STATUS_OK);
    strcpy(command.name, "Practice account"); command.amount = Pounds(0); Submit(operations, &actor, command);
    command = Command(operations, UMI_BANK_TEST_CREDIT, "opening-funds", 3, 1);
    REQUIRE(umi_financial_id_assign(&command.sourceAccountId, "account") == UMI_STATUS_OK);
    command.amount = Pounds(10000); Submit(operations, &actor, command);
    command = Command(operations, UMI_BANK_HOLD_PLACE, "reservation", 4, 2);
    REQUIRE(umi_financial_id_assign(&command.sourceAccountId, "account") == UMI_STATUS_OK);
    command.amount = Pounds(2000); Submit(operations, &actor, command);
    command = Command(operations, UMI_BANK_TEST_CREDIT, "later-funds", 5, 3);
    REQUIRE(umi_financial_id_assign(&command.sourceAccountId, "account") == UMI_STATUS_OK);
    command.amount = Pounds(250); Submit(operations, &actor, command);

    UmiBankActivityQuery query = {0};
    REQUIRE(umi_financial_id_assign(&query.accountId, "account") == UMI_STATUS_OK);
    REQUIRE(UmiBankActivityDateParse("2026-10-03", &query.fromDate) == UMI_STATUS_OK);
    query.toDate = query.fromDate;
    UmiBankActivity *activity = NULL; REQUIRE(UmiBankCaptureActivity(operations, &query, &activity) == UMI_STATUS_OK);
    UmiBankActivitySummary summary; REQUIRE(UmiBankActivityReadSummary(activity, &summary) == UMI_STATUS_OK);
    REQUIRE(summary.count == 1 && summary.totalPostings == 2 && summary.creditMinor == 250);
    REQUIRE(summary.balance.booked.minor_units == 10250 && summary.balance.reserved.minor_units == 2000 && summary.balance.available.minor_units == 8250);
    /* The report owns its evidence; source teardown cannot change it. */
    UmiBankOperationsDestroy(operations);
    size_t required = 0; REQUIRE(UmiBankActivityDescribe(activity, NULL, 0, &required) == UMI_STATUS_OK);
    char *text = malloc(required); REQUIRE(text != NULL);
    REQUIRE(UmiBankActivityDescribe(activity, text, required, NULL) == UMI_STATUS_OK); fputs(text, stdout);
    UmiCsvDocument *csv = NULL; REQUIRE(UmiBankActivityExportCsv(activity, &csv) == UMI_STATUS_OK);
    REQUIRE(UmiCsvDocumentRows(csv) == 3 && strstr(UmiCsvDocumentData(csv), "\"8250\"") != NULL);
    UmiCsvDocumentDestroy(csv); free(text); UmiBankActivityDestroy(activity); return 0;
}

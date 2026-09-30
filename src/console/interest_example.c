/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: src/console/interest_example.c
 * PURPOSE: Walk through reviewed practice interest using the shared Framework service.
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

/* This program deliberately opens memory storage. It never touches the user's
 * native banking database and cannot route funds to a payment network. */
static UmiStatus Reviewed(UmiBankOperations *bank, const UmiBankActor *actor,
    UmiBankCommand *command, uint64_t requestNumber)
{
    UmiBankCounts counts; UmiBankReview *review=NULL; UmiBankReceipt receipt;
    UmiStatus status=UmiBankOperationsCounts(bank,&counts);
    if(status!=UMI_STATUS_OK)return status;
    command->expectedRevision=counts.revision;
    command->businessDate=(UmiFinancialDate){2026,9,30}; command->timestampMillis=(int64_t)requestNumber;
    (void)snprintf(command->requestId.value,sizeof command->requestId.value,"lesson-%" PRIu64,requestNumber);
    status=UmiBankOperationsReview(bank,actor,command,&review);
    if(status==UMI_STATUS_OK && command->action==UMI_BANK_INTEREST_POST){
        char *text=malloc(UMI_BANK_REVIEW_TEXT_CAPACITY);
        if(text==NULL)status=UMI_STATUS_OUT_OF_MEMORY;
        else {status=UmiBankReviewDescribe(review,text,UMI_BANK_REVIEW_TEXT_CAPACITY,NULL);if(status==UMI_STATUS_OK)puts(text);free(text);}
    }
    if(status==UMI_STATUS_OK)status=UmiBankOperationsExecuteReviewed(bank,actor,review,&receipt);
    UmiBankReviewDestroy(review);return status;
}
int main(void)
{
    UmiBankOperations *bank=NULL; UmiBankCommand command; UmiBankBalance balance; UmiBankCounts counts;
    UmiBankActor maker={0},checker={0},operator={0}; UmiStatus status; uint64_t serial=0;
    char *statement=NULL; int failed=1;
#define STEP(x) do {status=(x);if(status!=UMI_STATUS_OK){fprintf(stderr,"Lesson failed: %s\n",umi_status_text(status));goto cleanup;}} while(0)
    STEP(umi_financial_id_assign(&maker.id,"lesson-maker")); maker.capabilities=UMI_BANK_CAP_CUSTOMERS|UMI_BANK_CAP_PAYMENTS;
    STEP(umi_financial_id_assign(&checker.id,"lesson-checker")); checker.capabilities=UMI_BANK_CAP_APPROVE;
    STEP(umi_financial_id_assign(&operator.id,"lesson-operator")); operator.capabilities=UMI_BANK_CAP_OPERATE|UMI_BANK_CAP_TEST_FUNDING;
    STEP(UmiBankOpenVolatileOperations(&bank));
    UmiBankCommandInit(&command,UMI_BANK_CUSTOMER_CREATE);strcpy(command.id.value,"learner");strcpy(command.name,"Practice customer");STEP(Reviewed(bank,&maker,&command,++serial));
    UmiBankCommandInit(&command,UMI_BANK_ACCOUNT_OPEN);strcpy(command.id.value,"savings");strcpy(command.ownerId.value,"learner");strcpy(command.name,"Practice savings");
    memcpy(command.amount.currency.code,"GBP",4);command.amount.scale=2;STEP(Reviewed(bank,&maker,&command,++serial));
    UmiBankCommandInit(&command,UMI_BANK_TEST_CREDIT);strcpy(command.id.value,"opening-funds");strcpy(command.sourceAccountId.value,"savings");
    memcpy(command.amount.currency.code,"GBP",4);command.amount.scale=2;command.amount.minor_units=100000;STEP(Reviewed(bank,&operator,&command,++serial));
    UmiBankCommandInit(&command,UMI_BANK_INTEREST_SUBMIT);strcpy(command.id.value,"september-interest");strcpy(command.ownerId.value,"2026-09");strcpy(command.sourceAccountId.value,"savings");
    command.interest=(UmiBankInterestTerms){500,30,365};STEP(Reviewed(bank,&maker,&command,++serial));
    UmiBankCommandInit(&command,UMI_BANK_INTEREST_APPROVE);strcpy(command.id.value,"september-interest");STEP(Reviewed(bank,&checker,&command,++serial));
    UmiBankCommandInit(&command,UMI_BANK_INTEREST_POST);strcpy(command.id.value,"september-interest");STEP(Reviewed(bank,&operator,&command,++serial));
    STEP(UmiBankOperationsBalance(bank,"savings",&balance));
    if(balance.booked.minor_units!=100410 || balance.reserved.minor_units!=0){fputs("Unexpected posted balance\n",stderr);goto cleanup;}
    statement=malloc(UMI_BANK_STATEMENT_TEXT_CAPACITY);if(statement==NULL)goto cleanup;
    STEP(UmiBankOperationsCounts(bank,&counts));STEP(UmiBankOperationsDescribeStatement(bank,"savings",6,counts.revision,statement,UMI_BANK_STATEMENT_TEXT_CAPACITY,NULL));puts(statement);
    UmiBankCommandInit(&command,UMI_BANK_INTEREST_REVERSE);strcpy(command.id.value,"september-interest");STEP(Reviewed(bank,&operator,&command,++serial));
    STEP(UmiBankOperationsBalance(bank,"savings",&balance));
    if(balance.booked.minor_units!=100000 || balance.reserved.minor_units!=0){fputs("Unexpected reversed balance\n",stderr);goto cleanup;}
    puts("Practice interest posted and reversed; the original journal was retained. No data was written to disk.");failed=0;
cleanup:
    free(statement);UmiBankOperationsDestroy(bank);return failed;
#undef STEP
}

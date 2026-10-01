/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: src/console/review_queue_example.c
 * PURPOSE: Teach captured request selection and separate review/execute using the product memory service.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/bank/operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static UmiStatus SubmitLessonCommand(UmiBankOperations *bank,const UmiBankActor *actor,UmiBankCommand *command,unsigned serial)
{
    UmiBankCounts counts;UmiStatus status=UmiBankOperationsCounts(bank,&counts);
    if(status!=UMI_STATUS_OK)return status;
    command->expectedRevision=counts.revision;command->timestampMillis=serial;command->businessDate=(UmiFinancialDate){2026,10,1};
    (void)snprintf(command->requestId.value,sizeof(command->requestId.value),"queue-lesson-%u",serial);
    UmiBankReview *review=NULL;UmiBankReceipt receipt;status=UmiBankOperationsReview(bank,actor,command,&review);
    if(status==UMI_STATUS_OK)status=UmiBankOperationsExecuteReviewed(bank,actor,review,&receipt);
    UmiBankReviewDestroy(review);return status;
}
int main(void)
{
    UmiBankOperations *bank=NULL;UmiBankWorkQueue *queue=NULL;UmiBankReview *review=NULL;UmiCsvDocument *csv=NULL;
    UmiBankActor maker={0},checker={0};UmiBankCommand command;UmiBankCounts before,after;
    UmiBankWorkQueueSummary summary;UmiBankWorkQueueRow row;UmiBankAction action;UmiBankReceipt receipt;
    UmiStatus status;int result=1;char *text=NULL;
#define STEP(call) do{status=(call);if(status!=UMI_STATUS_OK){fprintf(stderr,"Queue lesson: %s\n",umi_status_text(status));goto done;}}while(0)
#define EXPECT(condition) do{if(!(condition)){fprintf(stderr,"Queue lesson mismatch: %s\n",#condition);goto done;}}while(0)
    STEP(UmiBankOpenVolatileOperations(&bank));
    STEP(umi_financial_id_assign(&maker.id,"lesson-maker"));maker.capabilities=UMI_BANK_CAP_CUSTOMERS|UMI_BANK_CAP_PAYMENTS;
    STEP(umi_financial_id_assign(&checker.id,"lesson-checker"));checker.capabilities=UMI_BANK_CAP_APPROVE;
    UmiBankCommandInit(&command,UMI_BANK_CUSTOMER_CREATE);strcpy(command.id.value,"customer");strcpy(command.name,"Queue learner");
    STEP(SubmitLessonCommand(bank,&maker,&command,1));
    UmiBankCommandInit(&command,UMI_BANK_ACCOUNT_OPEN);strcpy(command.id.value,"account");strcpy(command.ownerId.value,"customer");
    strcpy(command.name,"Practice account");memcpy(command.amount.currency.code,"GBP",4);command.amount.scale=2;
    STEP(SubmitLessonCommand(bank,&maker,&command,2));
    /* A charge submission and approval do not debit or reserve money. */
    UmiBankCommandInit(&command,UMI_BANK_CHARGE_SUBMIT);strcpy(command.id.value,"charge");strcpy(command.ownerId.value,"reference");
    strcpy(command.sourceAccountId.value,"account");strcpy(command.name,"Practice service charge");
    memcpy(command.amount.currency.code,"GBP",4);command.amount.scale=2;command.amount.minor_units=250;
    STEP(SubmitLessonCommand(bank,&maker,&command,3));
    STEP(UmiBankCaptureWorkQueue(bank,NULL,&queue));STEP(UmiBankWorkQueueSummaryRead(queue,&summary));
    EXPECT(summary.count==1&&summary.pending==1);STEP(UmiBankWorkQueueRowAt(queue,0,&row));
    EXPECT(row.kind==UMI_BANK_WORK_CHARGE&&row.amount.minor_units==250);
    STEP(UmiBankWorkQueueExportCsv(queue,&csv));puts(UmiCsvDocumentData(csv));
    STEP(UmiBankWorkQueueResolveAction(queue,0,UMI_BANK_WORK_APPROVE,&action));
    UmiBankCommandInit(&command,action);command.id=row.id;strcpy(command.requestId.value,"queue-checker-review");
    command.expectedRevision=summary.revision;command.businessDate=(UmiFinancialDate){2026,10,1};command.timestampMillis=4;
    STEP(UmiBankOperationsCounts(bank,&before));
    STEP(UmiBankReviewQueuedRequest(bank,queue,0,&checker,&command,&review));
    STEP(UmiBankOperationsCounts(bank,&after));EXPECT(after.revision==before.revision);
    text=malloc(UMI_BANK_REVIEW_TEXT_CAPACITY);if(text==NULL)goto done;
    STEP(UmiBankReviewDescribe(review,text,UMI_BANK_REVIEW_TEXT_CAPACITY,NULL));puts(text);
    /* The lesson explicitly elects to approve this fixed practice example.
     * An interactive application waits for the user's separate Submit action. */
    STEP(UmiBankOperationsExecuteReviewed(bank,&checker,review,&receipt));EXPECT(receipt.revision==4);
    STEP(UmiBankWorkQueueSummaryRead(queue,&summary));EXPECT(summary.revision==3&&summary.pending==1);
    UmiBankReviewDestroy(review);review=NULL;
    EXPECT(UmiBankReviewQueuedRequest(bank,queue,0,&checker,&command,&review)==UMI_STATUS_BUSY&&review==NULL);
    UmiBankWorkQueueDestroy(queue);queue=NULL;STEP(UmiBankCaptureWorkQueue(bank,NULL,&queue));
    STEP(UmiBankWorkQueueSummaryRead(queue,&summary));EXPECT(summary.approved==1&&summary.pending==0);
    UmiBankBalance balance;STEP(UmiBankOperationsBalance(bank,"account",&balance));EXPECT(balance.booked.minor_units==0&&balance.reserved.minor_units==0);
    puts("The request was separately reviewed and approved. No charge was posted; no disk storage or network was used.");result=0;
done:
    free(text);UmiCsvDocumentDestroy(csv);UmiBankReviewDestroy(review);UmiBankWorkQueueDestroy(queue);UmiBankOperationsDestroy(bank);return result;
#undef STEP
#undef EXPECT
}

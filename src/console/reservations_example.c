/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: src/console/reservations_example.c
 * PURPOSE: Teach captured available-funds evidence and a separate reviewed release through the Bank facade.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/bank/operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Volatile local practice only: this lesson opens no file and contacts no bank.
 * Actor capabilities come from this trusted host, not a user-entered role claim. */
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Lesson failed at %d: %s\n",__LINE__,#x);exit(1);}}while(0)
static UmiMoney Pounds(int64_t minor)
{UmiMoney value={0};memcpy(value.currency.code,"GBP",4);value.scale=2;value.minor_units=minor;return value;}
static UmiBankCommand Command(UmiBankOperations *operations,UmiBankAction action,const char *id,unsigned sequence)
{
    UmiBankCounts counts;REQUIRE(UmiBankOperationsCounts(operations,&counts)==UMI_STATUS_OK);
    UmiBankCommand c;UmiBankCommandInit(&c,action);REQUIRE(umi_financial_id_assign(&c.id,id)==UMI_STATUS_OK);
    (void)snprintf(c.requestId.value,sizeof c.requestId.value,"reservation-lesson-%u",sequence);
    c.expectedRevision=counts.revision;c.businessDate=(UmiFinancialDate){2026,10,1};c.timestampMillis=sequence;return c;
}
static void Submit(UmiBankOperations *operations,const UmiBankActor *actor,UmiBankCommand command)
{UmiBankReceipt receipt;REQUIRE(UmiBankOperationsExecute(operations,actor,&command,&receipt)==UMI_STATUS_OK);}
int main(void)
{
    UmiBankOperations *operations=NULL;REQUIRE(UmiBankOpenVolatileOperations(&operations)==UMI_STATUS_OK);
    UmiBankActor actor={0};REQUIRE(umi_financial_id_assign(&actor.id,"lesson-operator")==UMI_STATUS_OK);actor.capabilities=UMI_BANK_CAP_ALL;
    UmiBankCommand c=Command(operations,UMI_BANK_CUSTOMER_CREATE,"customer",1);strcpy(c.name,"Reservation lesson");Submit(operations,&actor,c);
    c=Command(operations,UMI_BANK_ACCOUNT_OPEN,"account",2);strcpy(c.ownerId.value,"customer");strcpy(c.name,"Practice account");c.amount=Pounds(0);Submit(operations,&actor,c);
    c=Command(operations,UMI_BANK_TEST_CREDIT,"funding",3);strcpy(c.sourceAccountId.value,"account");c.amount=Pounds(10000);Submit(operations,&actor,c);
    c=Command(operations,UMI_BANK_HOLD_PLACE,"manual-reservation",4);strcpy(c.sourceAccountId.value,"account");c.amount=Pounds(2000);Submit(operations,&actor,c);
    UmiBankReservations *report=NULL;REQUIRE(UmiBankCaptureReservations(operations,"account",&report)==UMI_STATUS_OK);
    UmiBankReservationsSummary summary;REQUIRE(UmiBankReservationsReadSummary(report,&summary)==UMI_STATUS_OK);
    REQUIRE(summary.count==1U && summary.balance.booked.minor_units==10000 && summary.balance.reserved.minor_units==2000 && summary.balance.available.minor_units==8000);
    size_t required=0;REQUIRE(UmiBankReservationsDescribe(report,NULL,0,&required)==UMI_STATUS_OK);char *text=malloc(required);REQUIRE(text!=NULL);
    REQUIRE(UmiBankReservationsDescribe(report,text,required,NULL)==UMI_STATUS_OK);fputs(text,stdout);free(text);
    UmiBankAction action;REQUIRE(UmiBankReservationsReleaseAction(report,0U,&action)==UMI_STATUS_OK && action==UMI_BANK_HOLD_RELEASE);
    c=Command(operations,action,"manual-reservation",5);UmiBankReview *review=NULL;
    REQUIRE(UmiBankReviewReservationRelease(operations,report,0U,&actor,&c,&review)==UMI_STATUS_OK);
    UmiBankBalance balance;REQUIRE(UmiBankOperationsBalance(operations,"account",&balance)==UMI_STATUS_OK && balance.available.minor_units==8000);
    REQUIRE(UmiBankReviewDescribe(review,NULL,0,&required)==UMI_STATUS_OK);text=malloc(required);REQUIRE(text!=NULL);
    REQUIRE(UmiBankReviewDescribe(review,text,required,NULL)==UMI_STATUS_OK);fputs(text,stdout);free(text);
    /* The lesson deliberately chooses to apply the reviewed local release here.
     * A product must wait for a separate explicit user decision at this point. */
    UmiBankReceipt receipt;REQUIRE(UmiBankOperationsExecuteReviewed(operations,&actor,review,&receipt)==UMI_STATUS_OK);
    REQUIRE(UmiBankOperationsBalance(operations,"account",&balance)==UMI_STATUS_OK && balance.booked.minor_units==10000 && balance.reserved.minor_units==0 && balance.available.minor_units==10000);
    UmiBankReviewDestroy(review);UmiBankOperationsDestroy(operations);
    /* Captured evidence intentionally keeps the earlier GBP 80.00 availability. */
    UmiCsvDocument *csv=NULL;REQUIRE(UmiBankReservationsExportCsv(report,&csv)==UMI_STATUS_OK);
    REQUIRE(UmiCsvDocumentRows(csv)==3U && strstr(UmiCsvDocumentData(csv),"\"8000\"")!=NULL);
    fputs("Release applied locally. Booked GBP 100.00; reserved GBP 0.00; available GBP 100.00.\n",stdout);
    UmiCsvDocumentDestroy(csv);UmiBankReservationsDestroy(report);return 0;
}

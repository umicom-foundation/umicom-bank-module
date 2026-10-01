/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: src/console/audit_example.c
 * PURPOSE: Teach exact accepted-command history and journal investigation through the product facade.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/bank/operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Lesson failed at %d: %s\n",__LINE__,#x);exit(1);}}while(0)
static UmiMoney Pounds(int64_t minor){UmiMoney money={0};memcpy(money.currency.code,"GBP",4);money.scale=2;money.minor_units=minor;return money;}
static UmiBankCommand Command(UmiBankOperations *operations,UmiBankAction action,const char *id,unsigned sequence)
{
    UmiBankCounts counts;REQUIRE(UmiBankOperationsCounts(operations,&counts)==UMI_STATUS_OK);UmiBankCommand c;UmiBankCommandInit(&c,action);
    REQUIRE(umi_financial_id_assign(&c.id,id)==UMI_STATUS_OK);(void)snprintf(c.requestId.value,sizeof c.requestId.value,"audit-lesson-%u",sequence);
    c.expectedRevision=counts.revision;c.businessDate=(UmiFinancialDate){2026,10,1};c.timestampMillis=sequence;return c;
}
static void Send(UmiBankOperations *operations,const UmiBankActor *actor,UmiBankCommand c)
{UmiBankReceipt receipt;REQUIRE(UmiBankOperationsExecute(operations,actor,&c,&receipt)==UMI_STATUS_OK);}
int main(void)
{
    /* This educational host supplies distinct trusted actors. Its isolated
     * memory book has no files, payment network or authentication service. */
    UmiBankOperations *operations=NULL;REQUIRE(UmiBankOpenVolatileOperations(&operations)==UMI_STATUS_OK);
    UmiBankActor maker={0},checker={0},operator={0};strcpy(maker.id.value,"maker");maker.capabilities=UMI_BANK_CAP_CUSTOMERS|UMI_BANK_CAP_PAYMENTS;
    strcpy(checker.id.value,"checker");checker.capabilities=UMI_BANK_CAP_APPROVE;strcpy(operator.id.value,"operator");operator.capabilities=UMI_BANK_CAP_OPERATE|UMI_BANK_CAP_TEST_FUNDING;
    UmiBankCommand c=Command(operations,UMI_BANK_CUSTOMER_CREATE,"customer",1);strcpy(c.name,"Audit lesson");Send(operations,&maker,c);
    c=Command(operations,UMI_BANK_ACCOUNT_OPEN,"account",2);strcpy(c.ownerId.value,"customer");strcpy(c.name,"Practice account");c.amount=Pounds(0);Send(operations,&maker,c);
    c=Command(operations,UMI_BANK_TEST_CREDIT,"funding",3);strcpy(c.sourceAccountId.value,"account");c.amount=Pounds(10000);Send(operations,&operator,c);
    c=Command(operations,UMI_BANK_CHARGE_SUBMIT,"charge",4);strcpy(c.sourceAccountId.value,"account");strcpy(c.ownerId.value,"reference");strcpy(c.name,"Practice charge");c.amount=Pounds(250);Send(operations,&maker,c);
    Send(operations,&checker,Command(operations,UMI_BANK_CHARGE_APPROVE,"charge",5));
    Send(operations,&operator,Command(operations,UMI_BANK_CHARGE_POST,"charge",6));
    Send(operations,&operator,Command(operations,UMI_BANK_CHARGE_REVERSE,"charge",7));
    UmiBankAuditQuery query={0};query.family=UMI_BANK_AUDIT_CHARGE;strcpy(query.entityId.value,"charge");
    UmiBankAuditReport *report=NULL;REQUIRE(UmiBankCaptureAudit(operations,&query,&report)==UMI_STATUS_OK);
    UmiBankAuditSummary summary;REQUIRE(UmiBankAuditReadSummary(report,&summary)==UMI_STATUS_OK && summary.count==4 && summary.journalCount==2 && summary.revision==7);
    UmiBankAuditRow row;REQUIRE(UmiBankAuditRowAt(report,1,&row)==UMI_STATUS_OK && row.journalCount==0);
    UmiBankJournal posted,reversed;REQUIRE(UmiBankAuditJournalAt(report,2,0,&posted)==UMI_STATUS_OK && !posted.reversal && posted.revision==6);
    REQUIRE(UmiBankAuditJournalAt(report,3,0,&reversed)==UMI_STATUS_OK && reversed.reversal && reversed.revision==7);
    UmiBankBalance balance;REQUIRE(UmiBankOperationsBalance(operations,"account",&balance)==UMI_STATUS_OK && balance.booked.minor_units==10000);
    UmiBankOperationsDestroy(operations);
    /* The snapshot keeps copied evidence after the service closes. */
    size_t required=0;REQUIRE(UmiBankAuditDescribeEvent(report,3,NULL,0,&required)==UMI_STATUS_OK);char *text=malloc(required);REQUIRE(text!=NULL);
    REQUIRE(UmiBankAuditDescribeEvent(report,3,text,required,NULL)==UMI_STATUS_OK);fputs(text,stdout);free(text);
    UmiCsvDocument *csv=NULL;REQUIRE(UmiBankAuditExportJournalsCsv(report,&csv)==UMI_STATUS_OK && UmiCsvDocumentRows(csv)==6);UmiCsvDocumentDestroy(csv);
    UmiBankAuditDestroy(report);return 0;
}

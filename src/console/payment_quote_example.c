/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: src/console/payment_quote_example.c
 * PURPOSE: Compose the Framework quote service while proving no practice ledger change.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/


#include "umicom/bank/operations.h"
#include "umicom/finance/payments/payment_quote.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    UmiBankOperations *bank=NULL;UmiPaymentQuote *quote=NULL;UmiCsvDocument *csv=NULL;
    UmiPaymentQuoteRequest request={0};UmiPaymentQuoteSnapshot snapshot;UmiBankCounts before,after;
    char text[UMI_PAYMENT_QUOTE_TEXT_CAPACITY];int result=1;UmiStatus status;
#define STEP(call) do { status=(call);if(status!=UMI_STATUS_OK){fprintf(stderr,"Quote lesson: %s\n",umi_status_text(status));goto done;} } while(0)
    /* Bank supplies composition only. The quote service has no ledger handle,
     * so it cannot post the illustrative fee or authorise a payment. */
    STEP(UmiBankOpenVolatileOperations(&bank));STEP(UmiBankOperationsCounts(bank,&before));
    STEP(umi_payments_id_assign(&request.quoteId,"lesson-quote"));
    STEP(umi_payments_id_assign(&request.paymentId,"illustrative-payment"));
    STEP(umi_payments_currency_from_code("GBP",&request.principal.currency));
    request.principal.minor_units=12345;request.principal.scale=2;
    STEP(umi_payments_payment_fee_rule_init(&request.rule,"illustrative-rule",10,25,1000));
    request.feeRounding=UMI_MONEY_HALF_EVEN;request.taxBasisPoints=2000;request.taxRounding=UMI_MONEY_HALF_EVEN;
    STEP(UmiPaymentQuoteCreate(&request,&quote));STEP(UmiPaymentQuoteRead(quote,&snapshot));
    if(snapshot.fee.minor_units!=41||snapshot.tax.minor_units!=8||snapshot.totalDebit.minor_units!=12394)goto done;
    STEP(UmiPaymentQuoteDescribe(quote,text,sizeof(text),NULL));puts(text);
    STEP(UmiPaymentQuoteExportCsv(quote,&csv));puts(UmiCsvDocumentData(csv));
    STEP(UmiBankOperationsCounts(bank,&after));
    if(after.revision!=before.revision||after.journals!=before.journals||after.chargeRequests!=before.chargeRequests)goto done;
    memset(&request,0,sizeof(request));STEP(UmiPaymentQuoteRead(quote,&snapshot));
    if(snapshot.request.principal.minor_units!=12345)goto done;
    puts("The GBP 123.94 scenario includes GBP 0.41 fee and GBP 0.08 illustrative tax. No banking command ran and no data was written to disk.");
    result=0;
done:
    UmiCsvDocumentDestroy(csv);UmiPaymentQuoteDestroy(quote);UmiBankOperationsDestroy(bank);return result;
#undef STEP
}

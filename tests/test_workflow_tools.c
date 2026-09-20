/*-----------------------------------------------------------------------------
 * Umicom Framework
 * File: applications/bank/tests/test_workflow_tools.c
 * PURPOSE:
 *   Verify the thin bank entry point delegates exact manual reviews to Framework.
 * AUTHOR AND ORGANISATION: Sammy Hegab, Umicom Foundation
 * LICENCE: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/bank/runtime.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"%s:%d: %s\n",__FILE__,__LINE__,#x); return 1; } } while(0)

static UmiMoney Money(int64_t amount) {
    UmiMoney value={0};value.minor_units=amount;value.scale=2;
    memcpy(value.currency.code,"GBP",4);return value;
}
int main(int argc,char **argv) {
    if(argc!=2)return 2;
    UmiMoney a=Money(100000),b=Money(10000),c=Money(25000);
    UmiFinanceAccountReview out={0},unchanged={0};
    memset(&out,0x4a,sizeof out);unchanged=out;
    if(strcmp(argv[1],"valid")==0) {
        CHECK(UmiBankRuntimeReviewFunds(&a,&b,&c,&out)==UMI_STATUS_OK);
        CHECK(out.available.minor_units==90000 && out.remaining_after_payment.minor_units==65000);
    } else if(strcmp(argv[1],"currency")==0) {
        memcpy(b.currency.code,"USD",4);
        CHECK(UmiBankRuntimeReviewFunds(&a,&b,&c,&out)!=UMI_STATUS_OK);
        CHECK(memcmp(&out,&unchanged,sizeof out)==0);
    } else if(strcmp(argv[1],"overflow")==0) {
        a.minor_units=INT64_MIN;
        CHECK(UmiBankRuntimeReviewFunds(&a,&b,&c,&out)==UMI_STATUS_CAPACITY_EXCEEDED);
        CHECK(memcmp(&out,&unchanged,sizeof out)==0);
    } else if(strcmp(argv[1],"no-side-effects")==0) {
        UmiMoney original=a;
        CHECK(UmiBankRuntimeReviewFunds(&a,&b,&c,&out)==UMI_STATUS_OK);
        CHECK(memcmp(&a,&original,sizeof a)==0 && b.minor_units==10000 && c.minor_units==25000);
    } else return 2;
    return 0;
}

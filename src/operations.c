/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: src/operations.c
 *
 * PURPOSE:
 *   Delegate product banking-session creation without copying the financial engine.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#include "umicom/bank/operations.h"

/* Product assembly supplies the storage choice; Framework retains transaction,
 * memory ownership, recovery, authorisation and every financial invariant. */
UmiStatus UmiBankOpenOperations(const char *absolutePath, UmiBankOperations **outOperations)
{
    return UmiBankOperationsOpenSqlite(absolutePath, outOperations);
}

UmiStatus UmiBankOpenVolatileOperations(UmiBankOperations **outOperations)
{
    return UmiBankOperationsOpenMemory(outOperations);
}

/* Exact monetary representation and statement scope stay in Framework. */
UmiStatus UmiBankExportStatementCsv(const UmiBankOperations *operations,
    const char *accountId, uint64_t firstRevision, uint64_t lastRevision,
    UmiCsvDocument **outDocument)
{
    return UmiBankOperationsExportStatementCsv(operations, accountId,
        firstRevision, lastRevision, outDocument);
}

/* Products select shared queue policy rather than implementing another request
 * catalogue, history guard or approval path. Returned objects are caller-owned. */
UmiStatus UmiBankCaptureWorkQueue(const UmiBankOperations *operations,
    const UmiBankWorkQueueFilter *filter,UmiBankWorkQueue **outQueue)
{
    return UmiBankWorkQueueCapture(operations,filter,outQueue);
}
UmiStatus UmiBankReviewQueuedRequest(const UmiBankOperations *operations,
    const UmiBankWorkQueue *queue,size_t index,const UmiBankActor *actor,
    const UmiBankCommand *command,UmiBankReview **outReview)
{
    return UmiBankWorkQueueReview(operations,queue,index,actor,command,outReview);
}

/* Applications expose the canonical record rather than caching a second case. */
UmiStatus UmiBankFindReconciliation(const UmiBankOperations *operations,
    const char *id, UmiBankReconciliation *outRecord)
{
    return UmiBankOperationsFindReconciliation(operations, id, outRecord);
}


/* Date filtering, exact totals and copied report ownership remain in Framework;
 * the product supplies only the selected account and presentation filters. */
UmiStatus UmiBankCaptureActivity(const UmiBankOperations *operations,
    const UmiBankActivityQuery *query, UmiBankActivity **outActivity)
{
    return UmiBankActivityCapture(operations, query, outActivity);
}

/* Framework owns the reservation projection and captured-history checks.
 * Bank supplies the user's account and explicit intent without a second ledger. */
UmiStatus UmiBankCaptureReservations(const UmiBankOperations *operations,
    const char *accountId, UmiBankReservations **outReport)
{
    return UmiBankReservationsCapture(operations, accountId, outReport);
}
UmiStatus UmiBankReviewReservationRelease(const UmiBankOperations *operations,
    const UmiBankReservations *report, size_t index, const UmiBankActor *actor,
    const UmiBankCommand *command, UmiBankReview **outReview)
{
    return UmiBankReservationsReviewRelease(operations, report, index, actor, command, outReview);
}

/* Shared evidence ownership stays in Framework; the product adds no ledger. */
UmiStatus UmiBankCaptureAudit(const UmiBankOperations *operations,
    const UmiBankAuditQuery *query,UmiBankAuditReport **outReport)
{return UmiBankAuditCapture(operations,query,outReport);}

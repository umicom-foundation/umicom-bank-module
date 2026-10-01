/*-----------------------------------------------------------------------------
 * Umicom Bank
 * File: include/umicom/bank/operations.h
 *
 * PURPOSE:
 *   Expose thin product entry points to the shared Framework banking service.
 *
 * AUTHOR AND ORGANISATION:
 * Sammy Hegab
 * Umicom Foundation
 *
 * LICENCE:
 * MIT
 *---------------------------------------------------------------------------*/
#ifndef UMICOM_BANK_OPERATIONS_H
#define UMICOM_BANK_OPERATIONS_H
#include "umicom/bank_operations/operations.h"
#include "umicom/bank_operations/statement_csv.h"
#include "umicom/bank_operations/work_queue.h"
#include "umicom/bank_operations/reconciliation.h"
#include "umicom/bank_operations/activity.h"
#include "umicom/bank_operations/reservations.h"
#include "umicom/bank_operations/audit_report.h"

#ifdef __cplusplus
extern "C" {
#endif
/** Open this product's explicitly local banking simulation through Framework.
 * The caller supplies an absolute path and owns the returned Framework handle. */
UmiStatus UmiBankOpenOperations(const char *absolutePath, UmiBankOperations **outOperations);
/** Open a volatile test/example session. It is never a fallback for failed disk
 * storage and is never presented as a real bank or network connection. */
UmiStatus UmiBankOpenVolatileOperations(UmiBankOperations **outOperations);
/** Copy a local-practice statement through Framework's canonical ledger.
 * The caller destroys successful output with UmiCsvDocumentDestroy. */
UmiStatus UmiBankExportStatementCsv(const UmiBankOperations *operations,
    const char *accountId, uint64_t firstRevision, uint64_t lastRevision,
    UmiCsvDocument **outDocument);
/** Capture local pending/approved requests through the shared service. The
 * caller owns the immutable queue and destroys it with UmiBankWorkQueueDestroy. */
UmiStatus UmiBankCaptureWorkQueue(const UmiBankOperations *operations,
    const UmiBankWorkQueueFilter *filter,UmiBankWorkQueue **outQueue);
/** Prepare a review for a captured row. No command is submitted here. */
UmiStatus UmiBankReviewQueuedRequest(const UmiBankOperations *operations,
    const UmiBankWorkQueue *queue,size_t index,const UmiBankActor *actor,
    const UmiBankCommand *command,UmiBankReview **outReview);
/** Copy retained comparison and investigation state through Framework.
 * Resolve/reopen commands use the same shared review and execution APIs. */
UmiStatus UmiBankFindReconciliation(const UmiBankOperations *operations,
    const char *id, UmiBankReconciliation *outRecord);
/** Capture filtered practice activity using the canonical Framework ledger.
 * The caller owns the report and destroys it with UmiBankActivityDestroy. */
UmiStatus UmiBankCaptureActivity(const UmiBankOperations *operations,
    const UmiBankActivityQuery *query, UmiBankActivity **outActivity);
/** Capture an exact explanation of one local practice account's reserved funds. */
UmiStatus UmiBankCaptureReservations(const UmiBankOperations *operations,
    const char *accountId, UmiBankReservations **outReport);
/** Prepare, but never execute, a release/cancellation from captured evidence. */
UmiStatus UmiBankReviewReservationRelease(const UmiBankOperations *operations,
    const UmiBankReservations *report, size_t index, const UmiBankActor *actor,
    const UmiBankCommand *command, UmiBankReview **outReview);

/** Capture exact accepted-command and journal evidence through Framework.
 * All filters are explicit; no command execution, reload or write occurs. */
UmiStatus UmiBankCaptureAudit(const UmiBankOperations *operations,
    const UmiBankAuditQuery *query,UmiBankAuditReport **outReport);

#ifdef __cplusplus
}
#endif
#endif

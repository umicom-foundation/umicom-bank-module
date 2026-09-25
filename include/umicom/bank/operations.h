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
#ifdef __cplusplus
extern "C" {
#endif
/** Open this product's explicitly local banking simulation through Framework.
 * The caller supplies an absolute path and owns the returned Framework handle. */
UmiStatus UmiBankOpenOperations(const char *absolutePath, UmiBankOperations **outOperations);
/** Open a volatile test/example session. It is never a fallback for failed disk
 * storage and is never presented as a real bank or network connection. */
UmiStatus UmiBankOpenVolatileOperations(UmiBankOperations **outOperations);
#ifdef __cplusplus
}
#endif
#endif

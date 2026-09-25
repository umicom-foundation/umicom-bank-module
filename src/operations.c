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

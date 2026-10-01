# Investigate a practice reconciliation difference

A reconciliation compares a booked account balance with a balance you enter
from another source. A difference is called a break. Recording or resolving a
break does not move money or change either original comparison amount.

This is a local practice workflow. The application does not connect to a bank
statement feed or verify the external number you type.

## Record the comparison

1. Open the banking operations window and choose **Test operator**.
2. Choose **Reconcile account** in the action list.
3. Use a new entity ID, such as **comparison-01**. Enter the account ID in
   **Source account** and the observed amount in **Integer minor units**, with its
   currency and scale. For GBP with scale 2, 100 minor units means GBP 1.00.
4. Check the business date. Choose **Review command**, inspect the proposed
   comparison, then choose **Submit command** to record it.
5. Open **Reconciliation**. **Original result** says Matched or Unmatched.
   An unmatched row's **Investigation** says Open break.

Investigate the cause before deciding what to do. Do not enter a matching number
just to clear a break. If a genuine ledger correction is needed, use its proper
reviewed financial workflow first. Reconciliation itself cannot correct a ledger.

## Resolve an investigated break

1. Record a new **Reconcile account** comparison for the same account after
   completing the investigation. Give it another entity ID, such as
   **comparison-02**. Its external and booked balances must actually match.
2. Choose **Resolve reconciliation break** and keep **Test operator** selected.
3. In **Entity ID**, enter the original unmatched comparison ID.
4. In **Owner / beneficiary / card ID**, enter the new matching comparison ID.
5. In **Name**, explain the investigation and why it can close.
   Keep the explanation brief; the field accepts at most 95 text bytes.
6. Choose **Review command**. Check the original amounts, linked comparison,
   account, revisions and explanation. Nothing has changed yet.
7. Choose **Submit command**. The original row now says Resolved break.
   Its original result still says Unmatched. The evidence ID, reviewer, review
   revision and explanation appear beside it.

The linked comparison must be the latest comparison for that account. A later
comparison or any later account posting makes it unsuitable. This includes a
debit followed by a reversal that returns the balance to the same number.
Record and review fresh evidence instead. Comparisons or postings on unrelated
accounts do not replace this account's evidence.

## Reopen when new information arrives

1. Choose **Reopen reconciliation break** and **Test operator**.
2. Enter the resolved break's ID and explain the new information in the name
   field. The owner field is disabled for this action.
3. Review the proposed change, then submit it.

The row says Reopened break and keeps its previous evidence ID for history.
To resolve again, record a new matching comparison after the reopening.
Resolved cases do not automatically reopen when ordinary account activity continues.
The audit retains both earlier resolve and reopen commands, even though the
table displays the most recent reviewer and explanation.

## If an action is refused

If the displayed state changed, use **Reload committed data**, inspect the current rows, choose
**New request**, and review again. Another window may have saved a newer event.
Do not reuse an old preview or automatically retry it.

A matched original comparison cannot be resolved or reopened. Only a resolved
break can reopen. Resolving needs an open or reopened break, a later matching
comparison for the same account, a nonblank reason, and operator capability.
Editing a field or changing the selected test identity invalidates the review.

These administrative actions are available even for blocked or closed accounts
because they change no balances, holds or financial eligibility. Matching
compares booked balances; it does not reconcile pending holds or available funds.

## Storage and limits

Successful actions are committed to the same local event log as the existing
banking workflows and are restored when that profile reopens. A failed commit
does not publish a partly resolved case. The record and event limits are still
64 comparisons and 256 committed events per practice profile.

The role selector supplies test capabilities; it is not authentication. There
are no case assignments, deadlines, attachments, automatic statement import or
separate checker approval for reconciliation investigation in this workflow.

Keep a copy of the profile database before returning to an older executable.
Older versions reject the new action numbers. Framework and applications must
be rebuilt together because the copied reconciliation and review structures
have gained fields.

The complete memory-only lesson in **src/console/reconciliation_example.c**
creates a zero-balance account, records a difference, resolves it against a
later match, and reopens it. It uses Bank's public entry points and creates no
database file or network connection.

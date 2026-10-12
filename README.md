# Umicom Bank Module

Thin C23 product composition built from Framework-owned banking, payments,
positions, digital assets, risk, settlement, collateral and audit components.

Bank can now start learning, standard and focus layouts through the shared
Framework product surface. Product controllers provide honest empty states and
stage financial commands for authorization without claiming that money moved.
See [the Bank application surface guide](docs/APPLICATION_SURFACE_GUIDE.md).

The native `umicom-bank` workstation renders these live product surfaces with
Framework-owned identity, appearance, docking, floating, grouping and layout
editing. The console remains available for small verification and automation
tasks. Umicom Desk launches the native workstation when Bank is selected.

Follow [Practice interest and readable statements](docs/PRACTICE_INTEREST_AND_STATEMENTS.md)
to submit, separately approve, post and reverse fixed-principal practice interest,
then inspect a revision-bounded statement. The lesson also covers keyboard
opening of named layouts and includes a complete memory-only C23 example.

## Copy reports for review

Review exact local-practice amounts outside Bank with [Copy a practice account statement](docs/COPYING_STATEMENT_REPORTS.md).

## Review an account charge

Follow [Review a practice account charge](docs/PRACTICE_CHARGES.md) to submit, separately approve, post and reverse a fixed local charge, then inspect its retained statement entries.

## Review payment pricing assumptions

Use [Review a payment fee scenario](docs/REVIEWING_PAYMENT_FEES.md) to calculate and copy a fee quote without submitting a payment or changing account balances.

## Review open requests together

[Review pending banking requests](docs/REVIEWING_PENDING_REQUESTS.md) shows how
to filter transfers, interest and charges, inspect their captured details,
prepare a guarded review and copy the queue without submitting an action.

## Investigate reconciliation differences

Use [Investigate a practice reconciliation difference](docs/INVESTIGATING_RECONCILIATION_BREAKS.md)
to resolve an unmatched comparison against fresh matching evidence or reopen it
when new information arrives. Both actions retain the original facts and audit
history without adjusting account balances.


## Review account activity by date

[Review account activity](docs/REVIEWING_ACCOUNT_ACTIVITY.md) explains inclusive
business-date filters, debit/credit selection, reference searches and copying a
captured report while retaining the complete account balance context.

## Explain available account funds

[Understand reserved funds](docs/UNDERSTANDING_RESERVED_FUNDS.md) shows the active
holds and transfers behind an available balance, then guides a separate reviewed
release or cancellation using the existing command workflow.

## Investigate accepted command history

[Follow an accepted banking request](docs/INVESTIGATING_ACCEPTED_COMMANDS.md) explains exact filters, captured command details and linked posting or reversal journals.

## Review linked context values

Bank exposes Framework's reviewed context changes through
`umi_bank_workspace_context_review`, `umi_bank_workspace_context_apply`
and `umi_bank_workspace_clear_context` in
`umicom/bank/workspace_commands.h`. A context group is a named value that
related panels can share; for example, a host could use `accounts.primary` with the sample
value `practice-account`. The host must connect that name to its panel consumers.

1. Use a runtime initialised with this product's canonical experience. Prepare
   the requested changes with the review function; preparation changes no live state.
2. Display the copied Framework summary and rows. Keep the runtime and its
   workbench alive while the user reviews the proposed values. If the summary
   reports UI differences, show the captured UI value beside the cached value.
3. Apply only after acceptance, then destroy the review. If the workspace changed,
   prepare a fresh review. Cancelling only destroys the review.

This is a module API; native review screens are separate host work. Context edits
do not execute product commands or external operations. The shared guide at
`framework/docs/guides/REVIEWING_LINKED_CONTEXTS.html` in the Applications checkout
explains capacity, ownership, thread coordination and recovery in more detail.

Workspace recovery: [Review saved workspace arrangements](../../framework/docs/learning/restore-saved-workspaces.html). The guide explains the complete comparison, explicit confirmation and recovery limits.


### Dated cash planning

Open **Cash planning** to maintain local dated inflows and outflows. Apply the
plan's settings, add entries with unique IDs, and inspect end-of-day balances and
the first projected buffer shortfall. You can disable or edit an entry, undo the
last stored change, save a new JSON file, review a loaded plan before applying
it, and export a CSV report with explicit currency and minor units.

Framework owns the complete workflow. These are manual assumptions; the planner
does not read account balances, broker buying power, submit payments or place
orders. Save explicitly before closing. See Framework's
`docs/learning/cash-planning.html` for a worked example and recovery steps.


### Statement balance comparison
Open **Bank transaction reports**, import a saved Open Banking JSON response,
select an account and currency, then choose **Reconcile selected account**.
The shared Framework review compares dated booked activity with opening and
closing balances you supply. Pending entries remain separate, and edits
invalidate the previous export. This does not connect to a bank or post money.
See [the statement review guide](docs/STATEMENT_RECONCILIATION.html).

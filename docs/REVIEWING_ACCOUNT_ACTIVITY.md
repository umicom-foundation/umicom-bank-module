# Review account activity by business date

The **Account activity** page shows posted entries for one local practice
account. You can select business dates, debits or credits, and a reference.
The report also shows the complete account balance at the moment you capture
it. Capturing or copying this report does not move money or approve a request.

## Capture a report

1. Open the Bank operations window and select **Account activity**.
2. Enter the exact **Account ID** from your account list.
3. Leave both date fields blank for all retained dates, or enter **YYYY-MM-DD**.
   Both ends are inclusive: **2026-10-01** through **2026-10-31** includes
   postings dated on either day. One blank end leaves that side unbounded.
4. Choose **All postings**, **Debits only** or **Credits only**.
5. Optionally enter part of a reference or journal ID in
   **Reference / journal contains**. Matching is case-sensitive and literal:
   **charge** matches **monthly-charge**, but **CHARGE** does not. Wildcards
   are not used. IDs use letters, digits, dot, dash and underscore.
6. Choose **Capture activity**. Read the account, filters and captured revision
   at the top before inspecting the entries.
7. Choose **Copy captured activity CSV** to copy exactly that report.

Dates must be real Gregorian dates between 1600 and 9999. Invalid dates, a
reversed date range or an unknown account produce an explanation. A valid
account with no matching postings produces a report that says so.

Changing a filter clears the old report and disables copying until you
capture again. The report does not refresh automatically when you submit
another banking command. Capture again for newer local state. If another
window committed changes, use the existing reload action first.

## Read balances and totals correctly

The report has two separate sections:

- **Complete balances at capture** shows the account's booked, reserved and
  available funds across all retained activity, regardless of your filters.
- **Matching postings only** totals just the displayed debits and credits.
  Its net is credits minus debits. It is not an opening, closing or available
  balance.

For example, an account may have **GBP 102.50** booked and **GBP 20.00**
reserved, leaving **GBP 82.50** available. A date filter may show only one
credit of **GBP 2.50**. That filtered total does not replace the complete
account balance.

Holds and unposted requests do not appear as journal entries. Active holds
and pending or approved outgoing transfers can still affect the reserved
and available figures. A reversal appears as a separate compensating posting;
the original posting remains in history.

## Understand the order of entries

Rows stay in posting revision order. A revision identifies when the local
service committed the posting. The business date is the date supplied with
that command; it can be earlier than a previously posted entry.

Each row includes the original journal, reference, actor, action and recorded
booked balance after that posting. That balance includes all prior postings,
including rows excluded by your filters. It is not a running sum of the
filtered list. Use the existing revision-based **Statement report** when you
need the opening and closing balances of a consecutive posting interval.

## Export and troubleshoot

CSV includes the captured revision, filters, currency and scale. Amounts are
exact integer minor units: **250** with scale **2** means **2.50**. Complete
balances and matching totals occur only on the summary row. Posting rows
have separate debit, credit and recorded-balance columns.

If matching debit or credit turnover exceeds the signed 64-bit amount range,
capture fails without a partial report. Narrow the date range or reference.
This can happen even when the current account balance itself is representable.

The report reads cached local simulation data. It does not fetch a live bank
statement, reconcile external records or authenticate an actor. Clipboard
availability after the application closes depends on the desktop; paste or
save a copy before closing when you need a lasting export.

The complete C23 example **src/console/activity_example.c** creates the amounts
above in memory, captures a single-date report through the Bank facade and
demonstrates that a captured report owns its data. It does not open a database
or contact a financial institution.

See [practice statements](PRACTICE_INTEREST_AND_STATEMENTS.md) and
[statement CSV reports](COPYING_STATEMENT_REPORTS.md) for the existing workflow.

## Explain the reserved balance

Activity lists posted ledger rows. [Reserved funds](UNDERSTANDING_RESERVED_FUNDS.md)
shows the active manual holds, card authorisations and outgoing transfers that
reduce availability before a posting. Its captured totals explain the complete
reserved amount and support a separate reviewed release or cancellation.

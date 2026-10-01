# Copy a practice account statement

Bank's local-practice statement lists booked ledger entries for one account.
A CSV copy includes the chosen revision range and exact integer amounts so you
can review a practice session outside the application.

1. Open the banking operations workspace and enter the **Statement account**
   identity in the form.
2. Open **Statement report**. Enter the first and last revisions. Blank fields
   mean revision 1 through the displayed committed revision. Use **Reload**
   first if you need to include newer committed work.
3. Select **Show statement range** to read the formatted report, or select
   **Copy current statement range CSV** to export the current account and range.
   If you edit a field after viewing a report, the copy action uses those new
   fields; it does not copy the older report still visible below.
4. Paste into a text editor and save as UTF-8 with a `.csv` extension. Import
   into a spreadsheet with comma separators and quoted fields if needed.

The summary row identifies **LOCAL PRACTICE**, the account, currency, scale,
inclusive revision range, capture revision, entry count and storage mode. Each
entry has a journal ID, reference ID, business date, debit, credit and running
balance. The range is a revision range, not a business-date filter. A reversal
appears as another entry; the original remains present in its own range.

Amounts ending in `_minor` are integer minor units. For GBP with scale 2,
100410 means GBP 1004.10. Opening and closing balances describe booked money;
holds and unposted requests are excluded. The exporter does not convert through
floating-point arithmetic or round these integers. Import long integers as
text, because a spreadsheet may otherwise round them. Import account and
journal IDs as text as well.

A range without entries still has its summary and balances. Copying does not
post a transaction, calculate new interest, approve a request or connect to a
payment network. A memory-only report is explicitly labelled; a durable store
means the local service has persistent storage, not that this is a real bank
statement. Later ledger changes do not update a copied report.

If the account or range is invalid, correct the form and try again. A failed
copy leaves the previous clipboard content unchanged. Potential spreadsheet
formula text receives an apostrophe prefix; the ledger itself is unchanged.


## Filter by business date or reference

Use [Account activity](REVIEWING_ACCOUNT_ACTIVITY.md) for business-date,
debit/credit and reference filters. That report keeps matching totals separate
from complete balances. This revision-based statement remains available for
the opening and closing balances of a consecutive posting interval.

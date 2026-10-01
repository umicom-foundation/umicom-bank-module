# Follow an accepted banking request

Use **Audit investigation** in the local-practice operations window to follow a
request from submission through approval, posting and reversal. Its captured
details explain which command was accepted and which ledger journal it created.

1. Open the operations window and select **Audit investigation**.
2. Choose a workflow, such as **charge**, and enter its exact entity ID.
3. Choose **Capture audit**. Read the captured revision and matching count.
4. Select an accepted command from the dropdown. Inspect its request ID, supplied
   actor, business date and payload. Submission holds the original charge amount;
   later approval or posting commands identify the request without repeating it.
5. Select a posting to inspect its exact journal, debit line and credit line.
   An approval has no booked journal. A reversal has a separate compensating one.
6. Choose **Copy captured commands CSV** or **Copy captured journal lines CSV**
   according to the evidence you want to review.

All filters intersect. IDs are exact and case-sensitive. A workflow distinguishes
records that share an entity ID. Actor and request filters are optional. Dates
use `YYYY-MM-DD`; blank bounds mean any date or revision, and revision `0` also
means unbounded. Bounds are inclusive. Rows stay in accepted revision order even
when a business date is backdated.

Changing filters leaves the displayed capture and its copy buttons unchanged.
Capture again to apply the new filters. If capture fails, the earlier report
remains visible. An empty successful capture means no accepted command matched.
Each successful capture clears the selected row for you to choose deliberately.

Use the existing reload action before capturing another window's committed work.
Capture reads the already loaded service; it does not reload storage. The report
stays at its captured revision while other commands are accepted.

This is evidence from the local practice book. It is not proof that an external
payment moved or that a person was authenticated. Failed attempts and repeated
idempotent submissions do not create new events. Actor identities and capability
flags were supplied by the trusted application host.

Journal CSV has one row per debit or credit line. Both sides of a posting appear;
they are not two customer payments. Amounts are integer minor units with currency
and scale. No currency conversion or cross-currency total is calculated.

## Run the memory-only lesson

After building the Bank console examples, run the lesson from the Applications
directory in PowerShell:

```powershell
& ".\build\windows-ucrt64-all-debug\bin\umicom-bank-audit-example.exe"
```

It creates GBP 100.00 of practice funds, submits a GBP 2.50 charge, separately
approves it, posts it and reverses it. The charge history contains four accepted
commands and two journals. The balance returns to GBP 100.00. The lesson closes
the service, then reads its owned audit capture and exports the journal lines.
It opens no database file and contacts no payment network.

Product code can call `UmiBankCaptureAudit` from `umicom/bank/operations.h`; the
returned Framework report has the same ownership and filtering as the native
view. See [practice charges](PRACTICE_CHARGES.md) to create a similar reviewed
workflow interactively, and [account activity](REVIEWING_ACCOUNT_ACTIVITY.md)
for an account's monetary postings rather than accepted-command history.

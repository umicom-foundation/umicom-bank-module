# Practice interest and readable statements

This lesson adds interest to an imaginary savings account and then reverses it.
Use **Banking operations** in Umicom Bank. That window uses its existing local
SQLite database. Its test identities help you learn approval steps; they do not
authenticate real people or connect to a payment network.

An amount in **minor units** is an integer: with GBP and scale 2, `100000` means
GBP 1,000.00. A **basis point** is one hundredth of a percentage point, so `500`
basis points means 5% per year. A **revision** is the number of an accepted
command in this local account book.

## Create a practice account

Use new identifiers if you have run this lesson before. At every step, choose
**Review command**, inspect **Command review**, and then choose **Submit command**.
An error means the command did not complete. Correct it and review again.

1. Select **Test maker** and **Create customer**. Enter entity `interest-learner`
   and name `Practice customer`. Review and submit.
2. Select **Open account**. Enter entity `interest-savings`, owner
   `interest-learner`, name `Practice savings`, currency `GBP`, scale `2`, and
   integer minor units `0`. Review and submit.
3. Select **Test operator** and **Test credit**. Enter entity `interest-funding`,
   source account `interest-savings`, currency `GBP`, scale `2`, and minor units
   `100000`. Review and submit. The booked balance should be GBP 1,000.00.

## Review, approve and post interest

4. Select **Test maker** and **Submit practice interest**. Enter entity
   `interest-september`, owner/period `2026-09`, and source `interest-savings`.
   Enter interest rate `500`, days `30`, and year basis `365`.
5. Choose **Review command**. The fixed principal should be GBP 1,000.00 and the
   interest GBP 4.10. The calculation is `100000 × 500 × 30 / (10000 × 365)`,
   truncated once to 410 minor units. Review does not reserve or post anything.
   Submit the request; the account balance remains GBP 1,000.00.
6. Select **Test checker** and **Approve practice interest**. Enter entity
   `interest-september`. Review the captured principal, period, rate and amount,
   then submit. Approval alone still leaves the balance unchanged.
7. Select **Test operator** and **Post practice interest**. Enter the same entity,
   review and submit. The balance should become GBP 1,004.10. The journal records
   equal debit and credit amounts; the **Practice interest** page shows the request.

Changing an action prepares a new request identifier. To issue another command
with the same action, choose **New request** first. A repeated identical accepted
request returns its previous receipt. Reusing its identifier with different
fields is rejected.

## Read a statement and reverse the posting

8. Enter `interest-savings` in the **Statement account ID** field. Open
   **Statement report**, leave the first revision at `1` and the last blank,
   then choose **Show statement range**. The report shows the captured revision,
   currency, opening balance, entries and closing balance. Select and copy text
   if needed. It is a fixed report, not an automatically updating display.
9. To inspect only the interest posting, use its revision from **Audit** as both
   first and last revision. Earlier journals still determine the opening balance.
   These are inclusive revision numbers, not calendar dates. Choose **Reload
   committed data** before preparing a report that needs another window's commits.
10. Select **Test operator** and **Reverse practice interest**, using entity
    `interest-september`. Review and submit. With no other spending, the balance
    returns to GBP 1,000.00. A new compensating journal keeps the original posting
    visible. Refresh the statement range to include the reversal.

## If a step is refused

- The maker cannot approve or reject their own request. Choose the separate checker.
- A pending or approved request can be cancelled by its maker or the operator.
  A rejected, cancelled or reversed period can be corrected with a new entity ID.
- An account cannot have two pending, approved or posted interest requests with
  the same period ID. Keep period IDs consistent: different labels are different
  periods, even if you intended them to refer to the same dates.
- Blocked accounts cannot receive a posting. A reversal also needs sufficient
  available money; funds reserved by a hold cannot be spent by the reversal.
- On a stale-state message, reload, inspect what changed, and review again.
  A review does not reserve a database revision for you.
- A zero rounded amount, invalid basis, overflow or capacity limit is refused.
  The profile retains up to 64 records per catalogue and 256 accepted commands.

The interest calculation uses the booked balance at submission as a fixed
principal. Later deposits do not reprice it. The period label does not calculate
dates or detect overlapping differently named periods. Daily changing balances,
compounding, tax and scheduled accrual are separate features, not implied by
this lesson. Back up the database with your normal procedure while Bank is
closed before trying a new development build. Rebuild all consumers of the
updated Framework headers; an older executable cannot replay new interest actions.

## Try the complete memory-only example

After building Applications with the Bank console enabled:

```powershell
Set-Location "C:\umicom\Umicom-Applications"
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"
& ".\build\windows-ucrt64-all-debug\bin\umicom-bank-interest-example.exe"
```

The example uses an isolated in-memory book, prints the review and statement,
checks the expected balances, and destroys the book on exit.

## Open layouts with the keyboard

Open **Layout Library**, search if needed, and select a visible row with the
keyboard. Press **Enter**, or double-click the row, to open that layout. A single
click only selects it. The **Open** button remains available. Layout editing must
finish first. These actions change layout selection; they do not submit banking
commands. **Save library**, **Preview saved** and confirmed **Restore library**
use the shared layout storage and remain separate from banking account data.

# Review a practice account charge

A charge subtracts an explicitly chosen amount from a local practice account.
Bank uses Framework's account book, approval rules and journal for this workflow.
There is no connection to a payment network or an automatic billing schedule.

Open **Banking operations** in Umicom Bank. This window saves accepted commands
to its existing local SQLite database. The **Test maker**, **Test checker** and
**Test operator** choices demonstrate different permissions. They do not sign in
or authenticate real people.

## Prepare an account

Use new entity IDs if these names already exist in your database. An **entity
ID** identifies a customer, account or charge. A **request ID** identifies one
attempted command. Changing the action prepares a new request ID. For another
command with the same action, select **New request** first.

At each step, choose **Review command**, read **Command review**, and then choose
**Submit command**. Review predicts the result; it does not change or reserve
money. If the form or saved account book changes, review it again.

1. Select **Test maker** and **Create customer**. Enter entity `charge-learner`
   and name `Practice customer`. Review and submit.
2. Select **Open account**. Enter entity `charge-account`, owner `charge-learner`,
   name `Practice account`, currency `GBP`, scale `2` and minor units `0`.
   Review and submit.
3. Select **Test operator** and **Test credit**. Enter entity `charge-funding`,
   source `charge-account`, currency `GBP`, scale `2` and minor units `100000`.
   Review and submit. The account should show GBP 1,000.00.

Minor units are whole-number subdivisions of a currency. At scale 2, `250`
means GBP 2.50. Enter an integer such as `250`, rather than `2.50` or `2,50`.
Currency and scale must match the account. A positive fixed amount is required.

## Submit, approve and post

4. Select **Test maker** and **Submit practice charge**. Enter entity
   `september-charge`, owner/reference `statement-2026-09`, source `charge-account`,
   name/reason `Practice service charge`, currency `GBP`, scale `2` and minor
   units `250`. Review the account, reference, reason and amount, then submit.
5. Open **Practice charges**. The request should be pending. The account still
   contains GBP 1,000.00: submission does not reserve the GBP 2.50.
6. Select **Test checker** and **Approve practice charge**. Enter entity
   `september-charge`. Review the original amount and reason, then submit.
   Approval leaves the balance unchanged. The submitting maker cannot approve
   or reject their own request, even if given additional permissions.
7. Select **Test operator** and **Post practice charge**. Enter the same entity,
   review and submit. With no other transactions, the balance becomes GBP 997.50.
   **Practice charges** shows the posted state and posting revision. A revision
   is the number of an accepted command in this local account book.

Posting rechecks available funds. Holds and pending or approved transfers keep
their reserved money. An approved charge can therefore fail to post after other
activity changes the account. It remains approved so you can inspect the cause.

## Read the statement and reverse the charge

8. In **Statement report**, enter `charge-account` in **Statement account ID**,
   first revision `1`, and leave the last revision blank. Choose **Show statement
   range**. The fixed report shows the debit and the closing balance. Use
   **Copy current statement range CSV** to inspect the same retained account book in a table.
9. Select **Test operator** and **Reverse practice charge**. Enter entity
   `september-charge`, review and submit. The full GBP 2.50 returns to the same
   active account. Both the original journal and its compensating reversal remain.
   Without other activity, the balance is GBP 1,000.00 again.
10. Regenerate the statement to include the reversal. The existing displayed
    report is a snapshot, so it does not refresh automatically. **Practice charges**
    keeps both posting and reversal revisions.

## Correct a mistake

- Before posting, its maker or the operator can **Cancel practice charge**.
  The checker can **Reject practice charge** while it is pending.
- Captured amounts, references and reasons cannot be edited after submission.
  Cancel or reject the unposted charge and submit a corrected one with a new
  entity ID. A posted charge must first be fully reversed.
- Each account may have only one pending, approved or posted charge with a given
  reference. After cancellation, rejection or reversal, you can use the reference
  for a new charge. Different reference labels are different requests; the service
  does not guess whether two labels refer to the same fee.
- Repeating the identical accepted request returns its original receipt without
  another debit. Reusing that request ID with changed fields is refused. A later
  lifecycle step needs a new request ID but keeps the same charge entity ID.
- On a stale-state message, use **Reload committed data**, inspect the changes
  and review again. Another window may have saved a new command.
- Blocked accounts or customers cannot be charged or credited by reversal.
  Reactivate them through the supported record-state action before trying again.
  Closed accounts cannot reopen. Finish any necessary reversal before closing
  an account. Pending and approved charges prevent account closure.
- Amount overflow, a full catalogue or a full event history is refused. The local
  profile retains up to 64 charge requests and 256 accepted commands in total.
  Cancelling a request retains its record and does not recover a catalogue slot.

Only fixed amounts and full reversals are supported here. Percentage fees, tax,
recurring billing, fee waivers and external collection are separate features.
The reason and reference remain visible in the charge review and catalogue;
statements show the charge ID and monetary journals.

Before using a new development executable, back up the local database with Bank
closed. Rebuild applications that use the updated Framework headers. The new
reader accepts old event records, but an old executable cannot read new charge
actions. A replay error must be investigated; do not erase or rewrite saved events
to make the database open.

The complete `umicom-bank-charges-example` console lesson uses an independent
in-memory book through Bank's product entry point. It prints the reviews and
statement and checks the GBP 1,000.00 -> 997.50 -> 1,000.00 sequence. It does not
open the native window's database or save files.

[Audit investigation](INVESTIGATING_ACCEPTED_COMMANDS.md) lets you inspect each accepted step and the journals at its exact revision, including the original posting and its separate reversal.

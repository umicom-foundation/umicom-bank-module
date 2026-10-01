# Review pending banking requests

The **Review queue** page brings open transfers, practice interest and practice
charges together. Use it when you want to find a request and inspect its next
action without typing its entity ID into the command form.

This page belongs to **Banking operations**, Bank's local practice workspace.
Its test identities demonstrate separate responsibilities; they do not
authenticate a person or connect to a bank or payment network.

## Find a request

1. Open **Banking operations** and choose the **Review queue** tab.
2. If another window has changed the same store, choose **Reload committed
   data** first. **Refresh queue** reads the data already loaded in this window.
3. Choose all types or just Transfers, Interest or Charges.
4. Choose Pending, Approved or both. Optionally enter an exact account ID;
   leave it blank for every account. A transfer can match its paying or receiving
   account.
5. Choose **Refresh queue**. Read the captured revision and counts. “Shown” is
   the filtered count; “open” includes every pending or approved request.
6. Select a request in the list. Read its type, ID, state, accounts, reference,
   amount, maker and checker. Interest includes its fixed principal and terms;
   charges include their submitted reason.

Changing a filter clears the old captured list and review. Refresh explicitly
to apply it. An unknown account gives an error, rather than a misleading empty
result. If a selected request disappears after refresh, choose another request
yourself; the page does not automatically choose a replacement.

## Review the next action

1. Choose the appropriate **Simulation identity** in the command form above.
   A different Test checker approves or rejects a maker's request. Test operator
   posts an approved request. The original maker or an operator can cancel it.
2. In the queue, choose **Approve**, **Reject**, **Cancel** or **Post / Execute**.
   Pending requests cannot be posted. Approved requests cannot be approved or
   rejected again.
3. Choose **Review selected request**. Bank fills the action and entity ID,
   creates a new request ID and uses the current local date and timestamp.
   Your selected test identity stays unchanged.
4. Read the **Command review** page. Check the exact accounts and amounts and
   the predicted effects. No action has been committed yet.
5. Choose **Submit command** only after reviewing those details. Changing form
   fields or the test identity invalidates the review; review again deliberately.

Approval is separate from posting. A transfer's existing reservation rules
remain in force. Interest and charge approval do not reserve money, and posting
checks the current account and funds again. A posted, cancelled or rejected
request leaves the open queue while remaining in its original table and audit.

The queue reviews the captured version of the request. If the data changed,
reload and refresh, select the current row and review again. Do not repeat
Submit blindly after a failure. Two request types can use the same ID; the
queue keeps their types separate when preparing an action.

## Copy the captured list

Choose **Copy captured queue CSV** to copy the displayed capture and filters.
Paste it into a text editor or import it as text into a spreadsheet. Currency,
scale and integer minor-unit amounts are separate columns: GBP, scale 2 and
250 minor units represent GBP 2.50. Do not combine different currencies into
one total. The export includes its captured revision and marks local practice
data; it is not a statement of money moved through a payment network.

Copying does not refresh the queue, submit commands or save a report file.
Clipboard availability after closing Bank depends on the operating system.

## Learn through the complete C example

`src/console/review_queue_example.c` uses Bank's public entry points with
memory-only storage. It creates a practice charge, captures and exports the
queue, reviews a separate checker's approval and explicitly executes that
approval. It then demonstrates that the earlier queue is stale. No charge is
posted and no disk profile is opened.

The example target is `umicom-bank-review-queue-example`. Its printed review
and checks explain the same separation between capture, review and submission
used by the graphical page.

The current queue does not offer bulk approvals, scheduled posting, notification
delivery or completed-history search. Use the existing tables, statements and
audit pages for completed work.

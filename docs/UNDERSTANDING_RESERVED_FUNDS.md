# Understand reserved funds

The **Reserved funds** page explains why the available amount can be smaller
than the booked account balance. It combines active manual holds, card
authorisations and pending or approved outgoing transfers in one captured view.
It works with local practice data only.

## Read the report

1. Open banking operations and choose **Reserved funds**.
2. Enter your exact account ID. If another window changed the database, use
   **Reload committed data** before capturing.
3. Select **Capture reserved funds**. Read the captured revision and whether
   storage is memory-only or local disk.
4. Compare the booked, reserved and available amounts. The manual-hold,
   card-authorisation and transfer totals explain the reserved amount.
5. Inspect the rows. Transfers name their destination and beneficiary;
   card reservations name the card. Each row retains its creator and revision.
6. Select **Copy captured reservations CSV** if you need that exact capture
   outside the application. Changing the account clears the capture first.

For example, GBP 100.00 booked with a GBP 20.00 manual hold leaves GBP 80.00
available. The hold is not a GBP 20.00 debit. Releasing it restores GBP 100.00
available while the booked balance stays GBP 100.00.

## Release one reservation deliberately

1. Select **Test operator** in the existing command form. The original transfer
   maker can also cancel their own transfer with the maker identity.
2. Return to Reserved funds and explicitly select the intended row.
3. Click its review button. A manual hold offers release, a card authorisation
   offers void, and a pending or approved transfer offers cancellation.
4. Read **Command review**. Check the account, amount, record ID, action and
   predicted change in availability. Review alone changes no money or state.
5. Use **Submit** only when you want that local action applied.
6. Choose **Capture reserved funds** again to see the new state. The displayed
   earlier capture stays unchanged until you do so.

If the report is stale, reload committed data if necessary, capture again and
review again. Do not repeatedly submit the old review. Account or row changes,
edited form fields and changed identities invalidate a prepared review.
The application never chooses a different row for you after recapture.

## Understand what is excluded

Released holds, captured/refunded card authorisations and completed transfers no
longer reserve funds. A partial final card capture posts only its captured amount
and releases the unused reservation. Use [account activity](REVIEWING_ACCOUNT_ACTIVITY.md)
to inspect posted debits and credits. Interest and charge requests reserve
nothing; find them in [Review queue](REVIEWING_PENDING_REQUESTS.md).

This feature does not expire holds automatically, release part of a manual
hold, release many rows at once or contact a card/payment network. Test identities
are practice roles supplied by the application, not authentication. Reports are
captured evidence and do not refresh or save themselves automatically.

## Follow the complete C23 lesson

The complete source is [reservations_example.c](../src/console/reservations_example.c).
It uses the Bank facade, creates an in-memory GBP 100.00 account, reserves
GBP 20.00, captures the GBP 80.00 available balance, prepares a release review,
then explicitly applies it. The final current balance is GBP 100.00 available,
while the earlier exported capture still shows GBP 80.00. No file or network
service is opened. The example is built with Bank's console examples or tests.

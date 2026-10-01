# Review a payment fee scenario

The fee calculator helps you compare explicitly chosen amounts and rounding.
It uses the shared Framework service and cannot submit a payment or alter an
account balance. Your inputs are illustrative assumptions, not a bank tariff.

1. Open Umicom Bank and select **Banking operations**.
2. Open the **Payment fee quote** tab. Existing command and statement tabs stay
   available. The calculator has its own fields and does not copy them into a
   banking command.
3. Enter references for the quote, payment and rule. Enter a three-letter
   uppercase currency code and its explicit decimal scale. Scale 2 means two
   decimal places: enter `123.45` for 12345 minor units. Do not use grouping
   commas or scientific notation.
4. Enter a fixed fee, variable rate in basis points and maximum fee amount.
   One basis point is 0.01 percent. A maximum of zero means zero fee, not
   unlimited; it must be at least the fixed fee.
5. Enter a tax rate on the capped fee, if you want to model one. Zero means no
   tax in this scenario. The calculator does not decide whether tax applies.
6. Choose fee and tax rounding separately, then select **Calculate quote**.
   Read the captured assumptions, fee, tax, total charges and total debit.
7. Select **Copy captured CSV** to copy this scenario for review elsewhere.
   Editing any input disables Copy until you calculate a new quote.

For a worked example, use principal `123.45`, fixed fee `0.10`, variable rate
`25`, maximum fee `10.00`, tax rate `2000`, and **Nearest, ties to even** for
both rounding fields. Expected fee is `0.41`, illustrative tax is `0.08`, and
total debit is `123.94`. The cap applies before tax.

If calculation fails, the previous result stays visible and the message explains
that it was not replaced. Check the currency, scale, cap and nonnegative amounts.
Rates must be whole numbers from 0 to 10000. Amounts must fit a signed 64-bit
integer after conversion to minor units; no overflow is silently wrapped.

The default scenario has zero tax. A quote does not prove available funds or
authorise a transfer. It does not create a practice charge request, and closing
the page does not save the quote to the banking database. CSV copy uses the
clipboard; paste into a file yourself if you want to retain the scenario.
Import large integer columns as text when a spreadsheet cannot preserve them.

Developers can read `src/console/payment_quote_example.c` for a short complete
example. It uses an in-memory Bank service and verifies that quoting leaves
the ledger revision, journal count and charge-request count unchanged.

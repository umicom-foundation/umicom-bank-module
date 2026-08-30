# Umicom Bank Application Surface

Umicom Bank now opens its Framework-owned learning, standard and focus recipes
through the shared product surface. The application repository supplies only
Bank wording and safe command staging; Framework owns the panels, layouts,
window rules, refresh rules and lifecycle.

The standard workspace contains banking, payments, positions, digital assets,
risk, settlement, collateral and audit components. Learning shows the essential
banking journey. Focus keeps banking, payments and audit visible.

No live banking connection is required to start the surface. Empty panels say
what information is missing. A financial command is marked as staged and
requiring authorization; the controller never reports that money moved.

Use `umi_bank_application_surface_create()` for the standard layout or
`umi_bank_application_surface_create_for_audience()` for learning or focus.
Future GTK, Qt and web frontends should render the returned Framework runtime
instead of rebuilding the Bank layout themselves.

# Changelog

## 1.x-1.2.0 (unreleased)

Recommended upgrade: earlier versions did not record visits served from the
page cache and counted bots as visitors, so most sites' figures were wrong.

Run update.php after upgrading. Updates 1003-1005 add a database column, hash
stored session IDs, create the daily history table and switch recording to
the browser.

### Added
- Visits are recorded from the visitor's browser by default, so pages served
  from the page cache are counted and most bots, which do not run
  JavaScript, are not. Recording on the server remains available as an
  option; the settings page warns when it is used with page caching on. This
  replaces the "Use Ajax to increment the counter" option.
- Daily history: views per page per day, kept permanently and independent of
  the access log's retention, with Views integration ("Statistics history")
  for totals over any period, trends and charts.
- Trending content in the Popular content block: most viewed over a rolling
  1 to 30 days, from the daily history.
- The settings page shows how much the access log, view counts and history
  currently hold.

### Security
- The access log stored each visitor's live session ID, which is enough to
  hijack that session for anyone who can read the log (a View exposing
  "Session ID", a database backup). It now stores a SHA-256 hash, and
  update 1004 hashes the existing rows.
- The node "Pageviews" Views fields now respect the "View content hits"
  permission, as the Content statistics fields already did. Views showing
  them to other roles need that permission granted.
- statistics.php only counts existing nodes of a counted content type.
  Before, any number posted to it created a counter row.
- Top visitors report escapes the hostname.

### Changed
- Reports are grouped under Reports > Statistics as tabs
  (`admin/reports/statistics`). The old `admin/reports/hits`, `pages`,
  `visitors`, `referrers` and `access/%` paths are gone. (#27)
- The access log records each visitor's user agent. (#26)
- Excluded roles are now excluded from the access log as well as from
  content view counts, and have their own section on the settings page
  (previously hidden unless content view counting was on).
- The settings page is written for site owners: plain labels ("Page visit
  log", "Keep individual page visits for", "Content view counts"), and it
  explains the difference between the access log and view counts.
- New installs keep access log entries for 4 weeks (was 3 days).
- In browser mode the report time columns show server response time.

### Fixed
- Existing sites failed every logged page request with "Unknown column
  'user_agent'" because no update added the column. (#34)
- `statistics_exit()` fatal errors when hook_exit() runs on a page cache
  hit with `page_cache_invoke_hooks` enabled. (#33)
- Warning when a node page is not accessible. (#23)
- Views: added sortable, filterable "Views this week / month / year" fields
  under Content statistics. The node "Pageviews" fields sort and aggregate
  on the node ID, so their help text now points to these. (#6, #4)
- Views: the "Most recent view" field used a handler class that did not
  exist.
- `statistics.pages.inc` was duplicated, so the Track tabs failed with a
  parse error.
- PHP 8.1+ deprecations for requests without a User-Agent header or a page
  title, and warnings for nodes that have never been viewed.
- Sites upgraded from Drupal 7 no longer fail before the settings form has
  been saved.
- The referrer report escapes the host name in its LIKE condition.
- Node statistics tokens no longer warn for nodes that have never been
  viewed.
- Views: removed a stray `name field` on the access ID argument.
- With access logs kept forever ("Never"), report titles read "Top pages in
  the past 0 sec".
- Viewing a node's edit or revisions page was counted as a view of the
  content.
- statistics.php kept its settings in a global `$config`, which replaces
  Backdrop's own and breaks any later config() call.

# Changelog

## 1.x-1.1.0 (unreleased)

Run update.php after upgrading: update 1003 adds a database column, and
update 1004 replaces stored session IDs with their hashes.

### Security
- The access log stored each visitor's live session ID, which is enough to
  hijack that session for anyone who can read the log (a View exposing
  "Session ID", a database backup). It now stores a SHA-256 hash, and
  update 1004 hashes the existing rows.
- The node "Pageviews" Views fields now respect the "View content hits"
  permission, as the Content statistics fields already did. Views showing
  them to other roles need that permission granted.
- The Ajax counter only counts existing nodes of a counted content type.
  Before, any number posted to statistics.php created a counter row.
- Top visitors report escapes the hostname.

### Changed
- Reports are grouped under Reports > Statistics as tabs
  (`admin/reports/statistics`). The old `admin/reports/hits`, `pages`,
  `visitors`, `referrers` and `access/%` paths are gone. (#27)
- The access log records each visitor's user agent. (#26)
- Excluded roles are now excluded from the access log as well as from
  content view counts.

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

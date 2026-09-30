# Changelog

## 1.x-1.1.0 (unreleased)

Run update.php after upgrading: update 1003 adds a database column.

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

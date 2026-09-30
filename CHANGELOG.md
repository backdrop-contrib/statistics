# Changelog

## 1.x-1.2.1 (unreleased)

## 1.x-1.2.0 (2026-09-30)

Git tag: `1.x-1.2.0`.

**Recommended upgrade.** Earlier versions did not record visits served from
Backdrop's page cache and counted search engines and other bots as visitors,
so on most sites the figures were far from accurate.

**After upgrading, run update.php.** Then note that:
- The reports have moved to **Reports > Statistics**, as tabs.
- Views that show the content "Pageviews" fields to other roles need the
  **View content hits** permission granted.

### New
- **Accurate counting with page caching.** Visits are now recorded from the
  visitor's browser, so pages served from the page cache are counted, and
  most bots, which do not run JavaScript, are not. Recording on the server
  is still available; the settings page warns if it is used with page
  caching on. This replaces the "Use Ajax to increment the counter" option.
- **Daily history.** Views per page per day are kept permanently, however
  long individual page visits are kept. Use them in Views ("Statistics
  history") for totals over any period, trends and charts.
- **Trending content** in the Popular content block: the most viewed over a
  rolling 1 to 30 days.
- **Clearer settings page**, written for site owners, showing how much is
  currently recorded. Excluded roles have their own section and apply to
  everything the module records.
- Sortable, filterable "Views this week / month / year" fields under
  Content statistics in Views. (#6)
- The access log records each visitor's browser (user agent). (#26)

### Changed
- Reports are grouped under Reports > Statistics as tabs. (#27)
- Users with an excluded role are now left out of the access log too, not
  only the view counts.
- Viewing a content item's edit or revisions page no longer counts as a
  view of it.
- New installs keep individual page visits for 4 weeks (was 3 days).

### Fixed
- Sites upgraded to the development version failed every page with
  "Unknown column 'user_agent'". (#34)
- Fatal errors when `page_cache_invoke_hooks` is enabled. (#33)
- Warning when a content page is not accessible. (#23)
- Sorting or totalling the content "Pageviews" fields in Views gave wrong
  numbers; use the Content statistics fields instead. (#4)
- The Track tabs failed with an error.
- The "Most recent view" Views field did not work.
- PHP 8 warnings, including for content never viewed and requests without a
  browser user agent.
- Sites upgraded from Drupal 7 failed until the settings were saved.
- Report titles read "in the past 0 sec" when page visits are kept forever.

### Security hardening
- The access log no longer stores visitors' session IDs, which could have
  been used to take over a session by anyone able to read the log; it stores
  a one-way hash instead, and existing entries are converted on update.
- The content "Pageviews" Views fields now respect the View content hits
  permission.
- The visit counter only accepts existing content of a counted type.
- The Top visitors report escapes host names, and the Top referrers report
  escapes its search condition.

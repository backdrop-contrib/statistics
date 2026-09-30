Statistics
=====

The Statistics module shows you how often pages and content are viewed, where
visitors came from (referrer URL), and when. It is for recent activity and
popular content on your site; for detailed visitor analytics, use a dedicated
analytics service.

**Upgrading from an earlier version:** before 1.x-1.2.0, visits served from
Backdrop's page cache were not recorded, so sites using page caching
significantly undercounted anonymous visitors, and search engines and other
bots were counted as visitors. Version 1.2.0 records visits in the visitor's
browser, which fixes both, and keeps a daily history. Run update.php after
upgrading.

Fork of the Drupal core Statistics module.

How visits are recorded
-----------------------

By default each page reports its own visit from the visitor's browser once it
has loaded. This counts pages served from the page cache, and skips most search
engines and bots, which do not run JavaScript. Visitors with JavaScript turned
off are not counted.

Alternatively, visits can be recorded on the server while each page is built.
This also records how long each page took to build, but misses every page
served from the page cache and counts bots as visitors. The settings page warns
when this is chosen while page caching is on.

What is recorded
----------------

The module keeps three kinds of statistics, set up on the Statistics settings
page (Configuration > System > Statistics).

**The page visit log** (access log) records every page visit, of any page, as a
separate entry: the page and its title, the referring page, the visitor's IP
address and user account, their browser, and the time. The reports and the
Track tabs are built from it.

The "Keep individual page visits for" setting deletes log entries older than
the chosen period each time cron runs, so the reports only ever cover that
period. It needs a correctly configured cron task. "Never delete" keeps every
entry, and the log then grows with every page visit.

**Content view counts** are a running total kept for each piece of content,
of the content types you select: views today, this week, this month, this
year and of all time, plus when it was last viewed. Cron resets the day,
week, month and year totals to zero at the end of each period; the all-time
total is never reset. The counts are not affected by deleting old page visits.

**The daily history** keeps a total of views per page per day, permanently. It
covers every page while the page visit log is on, and counted content
regardless, and is not affected by deleting old page visits. It is what makes
trends and rolling periods possible: see Trending and Views below.

Viewing site usage
-------------

The module offers four reports, as tabs under Reports > Statistics
(admin/reports/statistics), built from the page visit log:

- Recent hits displays information about the latest activity on your site,
  including the URL and title of the page that was accessed and the user name
  (if available). Each entry has a details link.
- Top pages displays a list of pages ordered by how often they were viewed.
- Top visitors shows you the most active visitors for your site.
- Top referrers displays where visitors came from (referrer URL).

Content also gets a Track tab listing its recent views, and user accounts get a
Track page visits tab.

Displaying popular content
--------------------------

The module includes a **Popular content** block that can list the most viewed
content today, this week, this month, this year and of all time, the content
viewed most recently, and **trending** content: the most viewed over a rolling
period of 1 to 30 days ending today, from the daily history.

To use it:

1. On the statistics settings page, turn on "Count how often content is
   viewed" and choose the content types to count. The block is only offered
   while content view counts are on.
2. Go to Structure > Layouts, edit the layout for the pages where it should
   appear, and use "Add block" in a region. Choose "Popular content".
3. In the block's settings, choose how many items each list shows. Every
   list is disabled until given a number, so the block shows nothing until
   at least one is set.

Page view counter
------------------

The Statistics module includes a counter for each piece of content that
increases whenever it is viewed. To use the counter, enable content view counts
on the statistics settings page, and set the necessary permissions (View
content hits) so that the counter is visible to the users.

You may limit the counted content by content type. Roles selected under
Exclude roles are neither counted nor recorded; if the administrator role is
excluded, user 1 is excluded too.

Views
-----

The module provides Views fields, filters and sorts for:

- the access log, as its own base table;
- content, under "Content statistics": total views and views today, this week,
  this month and this year, plus the most recent view;
- the daily history, as the "Statistics history" base table and joined to
  content: day, hits, path and page title. Aggregate on Hits (SUM) with a Day
  filter for totals over any period, trending lists or charts.

Use these fields for sorting, filtering or aggregation. The older "Pageviews"
fields on content are for display only; sorting or aggregating on them uses the
node ID, not the count.

Permissions
-----------

- Administer statistics: change the statistics settings.
- View content access statistics: see the reports and the Track tabs.
- View content hits: see the view counter on content and the view count fields
  in Views.

Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

Current Maintainers
-------------------

- Docwilmot (https://github.com/docwilmot)
- DrAlbany (https://github.com/albanycomputers)

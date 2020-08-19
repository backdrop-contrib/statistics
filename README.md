Statistics
=====

Fork of the Drupal core Statistics module.

The Statistics module shows you how often a given page is viewed, who viewed 
it, the previous page the user visited (referrer URL), and when it was viewed. 
These statistics are useful in determining how users are visiting and 
navigating your site.

To enable collection of statistics, the Enable access log checkbox on the 
Statistics settings page must be checked. This access log is used to store 
data about every page accessed, such as the remote host's IP address, where 
they came from (referrer), what node they've viewed, and their user name. 
Enabling the log adds one database call per page displayed by Backdrop.

The Discard access logs older than setting on the settings page specifies the 
length of time entries are kept in the log before they are deleted. This 
setting requires a correctly configured cron maintenance task to run.

Enable Count content views to turn on and off the node-counting functionality 
of this module. If it is turned on, an extra database query is added for each 
node displayed, which increments a counter.

Viewing site usage
-------------

The Statistics module can help you break down details about your users and how 
they are using the site. The module offers four reports:

Recent hits displays information about the latest activity on your site, 
including the URL and title of the page that was accessed, the user name (if 
available) and the IP address of the viewer.
Top referrers displays where visitors came from (referrer URL).
Top pages displays a list of pages ordered by how often they were viewed.
Top visitors shows you the most active visitors for your site and allows you 
to ban abusive visitors.
Displaying popular content

The module includes a Popular content block that displays the most viewed 
pages today and for all time, and the last content viewed. To use the block, 
enable Count content views on the statistics settings page, and then you can 
enable and configure the block on the blocks administration page.

Page view counter
------------------

The Statistics module includes a counter for each page that increases whenever 
the page is viewed. To use the counter, enable Count content views on the 
statistics settings page, and set the necessary permissions (View content 
hits) so that the counter is visible to the users.

You may limit the tracked pages by content type and by the role of the current 
user.
Permissions
-----------

To access statistics one needs the proper permissions.

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

- Docwilmot (https://github.com/docwilmot

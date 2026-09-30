<?php

/**
 * @file
 * Records visits, shared by statistics_exit() and statistics.php.
 *
 * This file must only use functions available from BACKDROP_BOOTSTRAP_SESSION
 * onwards, because statistics.php loads it without a full bootstrap. The
 * caller must include core/includes/unicode.inc first.
 */

/**
 * Records one page visit in the access log, daily history and view counts.
 *
 * Each store is written only if it is enabled in the settings. The caller is
 * responsible for skipping excluded roles.
 *
 * @param array $visit
 *   The visit, with keys:
 *   - path: Internal path of the page, such as "node/1".
 *   - title: Page title, as returned by backdrop_get_title().
 *   - referrer: URL of the referring page, or an empty string.
 *   - nid: Node ID when the page is a node page, otherwise 0.
 *   - node_type: Optional node type of nid, looked up when omitted.
 *   - timer: Page generation or response time in milliseconds.
 */
function statistics_record_visit(array $visit) {
  global $user;
  $config = config('statistics.settings');
  $visit += array('path' => '', 'title' => '', 'referrer' => '', 'nid' => 0, 'timer' => 0);
  $path = truncate_utf8(_statistics_clean_utf8((string) $visit['path']), 255);
  $title = truncate_utf8(strip_tags((string) $visit['title']), 255);
  $nid = (int) $visit['nid'];

  // Content view counts only apply to existing nodes of a counted type.
  $count_node = FALSE;
  if ($nid > 0 && $config->get('count_content_views')) {
    $type = isset($visit['node_type']) ? $visit['node_type'] : db_query('SELECT type FROM {node} WHERE nid = :nid', array(':nid' => $nid))->fetchField();
    $count_node = $type && in_array($type, (array) $config->get('count_node_types'));
  }
  if (!$count_node) {
    $nid = 0;
  }

  if ($config->get('enable_access_log')) {
    $user_agent = isset($_SERVER['HTTP_USER_AGENT']) ? $_SERVER['HTTP_USER_AGENT'] : '';
    db_insert('accesslog')
      ->fields(array(
        'title' => $title,
        'path' => $path,
        'url' => (string) $visit['referrer'],
        'hostname' => ip_address(),
        'uid' => $user->uid,
        // Store a hash, never the live session ID: anyone able to read the
        // log (Views, a database backup) could otherwise hijack the session.
        // Rows from one session still group together.
        'sid' => session_id() === '' ? '' : hash('sha256', session_id()),
        'timer' => max(0, (int) $visit['timer']),
        'timestamp' => REQUEST_TIME,
        // Requests without a User-Agent header (monitors, some bots) must not
        // pass NULL to substr() and md5(), deprecated since PHP 8.1.
        'user_agent' => substr($user_agent, 0, 222) . ' ' . md5($user_agent),
      ))
      ->execute();
  }

  // Daily history outlives the access log: every page while the access log is
  // on, and counted content regardless.
  if ($config->get('enable_access_log') || $count_node) {
    db_merge('statistics_daily')
      ->key(array('day' => statistics_day_start(REQUEST_TIME), 'path' => $path))
      ->fields(array('nid' => $nid, 'title' => $title, 'hits' => 1))
      ->expression('hits', 'hits + 1')
      ->execute();
  }

  if ($count_node) {
    db_merge('node_counter')
      ->key(array('nid' => $nid))
      ->fields(array(
        'yearcount' => 1,
        'monthcount' => 1,
        'weekcount' => 1,
        'daycount' => 1,
        'totalcount' => 1,
        'timestamp' => REQUEST_TIME,
      ))
      ->expression('yearcount', 'yearcount + 1')
      ->expression('monthcount', 'monthcount + 1')
      ->expression('weekcount', 'weekcount + 1')
      ->expression('daycount', 'daycount + 1')
      ->expression('totalcount', 'totalcount + 1')
      ->execute();
  }
}

/**
 * Returns the start of the day containing a timestamp, in the site timezone.
 *
 * The site timezone rather than the visitor's, so every visit on one calendar
 * day lands in the same history row.
 *
 * @param int $timestamp
 *   A Unix timestamp.
 *
 * @return int
 *   The Unix timestamp of local midnight at the start of that day.
 */
function statistics_day_start($timestamp) {
  $timezone = config_get('system.date', 'default_timezone');
  $date = new DateTime('@' . $timestamp);
  $date->setTimezone(new DateTimeZone($timezone ? $timezone : date_default_timezone_get()));
  $date->setTime(0, 0);
  return $date->getTimestamp();
}

/**
 * Determines if the current user is excluded from tracking by role.
 *
 * @return bool
 *   TRUE if the user has an excluded role, or is user 1 while the
 *   administrator role is excluded.
 */
function statistics_node_count_excluded_role() {
  global $user;
  $config = config('statistics.settings');
  $admin_role = config_get('system.core', 'user_admin_role');
  $excluded = FALSE;
  // Missing on sites upgraded from Drupal 7 until the settings form is saved.
  $excluded_roles = (array) $config->get('node_count_excluded_roles');

  if (array_intersect($user->roles, $excluded_roles)) {
    $excluded = TRUE;
  }
  // If the admin role is excluded, we assume user 1 shouldn't be tracked
  // either.
  if (in_array($admin_role, $excluded_roles) && $user->uid == 1) {
    $excluded = TRUE;
  }

  return $excluded;
}

/**
 * Clean a path that might be invalid UTF-8.
 */
function _statistics_clean_utf8($path) {
  if (backdrop_validate_utf8($path)) {
    return $path;
  }
  // Oh nos! Invalid UTF-8. Now we fix it.
  return backdrop_convert_to_utf8($path, 'CP1252');
}

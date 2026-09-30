<?php
/**
 * @file
 * Records a visit reported by the page's JavaScript, with minimal bootstrap.
 *
 * Used when visits are recorded in the browser (the default). Pages served
 * from the page cache never reach the module's PHP, but their JavaScript still
 * runs, so it reports the visit here.
 */

/**
* Root directory of Backdrop installation.
*/
define('BACKDROP_ROOT', substr($_SERVER['SCRIPT_FILENAME'], 0, strrpos($_SERVER['SCRIPT_FILENAME'], '/modules/')));
// Change the directory to the Backdrop root.
chdir(BACKDROP_ROOT);

// Without $base_url in settings.php, Backdrop takes the base path, and from it
// the session cookie name, from the directory of SCRIPT_NAME. Run from here
// that is ".../modules/statistics", so the visitor's session would not be
// found. Present the request as the site's own index.php.
$_SERVER['SCRIPT_NAME'] = substr($_SERVER['SCRIPT_NAME'], 0, strrpos($_SERVER['SCRIPT_NAME'], '/modules/')) . '/index.php';

include_once BACKDROP_ROOT . '/core/includes/bootstrap.inc';
// The session phase loads the current user, so excluded roles can be honoured.
backdrop_bootstrap(BACKDROP_BOOTSTRAP_SESSION);
include_once BACKDROP_ROOT . '/core/includes/unicode.inc';
require_once __DIR__ . '/statistics.record.inc';

statistics_record_posted_visit();

/**
 * Records the visit posted by statistics.js.
 *
 * A function rather than top-level code: a global $config would replace
 * Backdrop's own $config (the settings.php overrides), and the next config()
 * call then fails with "Cannot use object of type Config as array".
 */
function statistics_record_posted_visit() {
  $config = config('statistics.settings');
  if ($_SERVER['REQUEST_METHOD'] != 'POST' || $config->get('record_method') == 'server' || !($config->get('count_content_views') || $config->get('enable_access_log'))) {
    return;
  }
  if (statistics_node_count_excluded_role()) {
    return;
  }

  // Everything posted here is supplied by the visitor, so it is only trusted
  // as log text: statistics_record_visit() truncates and strips it, and only
  // counts a nid that is an existing node of a counted type.
  $post = function ($key) {
    return isset($_POST[$key]) && is_string($_POST[$key]) ? $_POST[$key] : '';
  };
  $path = $post('path');
  if ($path === '') {
    return;
  }
  $nid = filter_var($post('nid'), FILTER_VALIDATE_INT, array('options' => array('min_range' => 1)));
  $timer = filter_var($post('timer'), FILTER_VALIDATE_INT, array('options' => array('min_range' => 0, 'max_range' => 600000)));
  statistics_record_visit(array(
    'path' => $path,
    'title' => $post('title'),
    'referrer' => substr($post('referrer'), 0, 2048),
    'nid' => $nid ? $nid : 0,
    'timer' => $timer ? $timer : 0,
  ));
}

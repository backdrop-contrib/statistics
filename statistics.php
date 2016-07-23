<?php

/**
 * @file
 * Handles counts of node views via Ajax with minimal bootstrap.
 */

/**
* Root directory of Backdrop installation.
*/
define('DRUPAL_ROOT', substr($_SERVER['SCRIPT_FILENAME'], 0, strpos($_SERVER['SCRIPT_FILENAME'], '/modules/statistics/statistics.php')));
// Change the directory to the Backdrop root.
chdir(DRUPAL_ROOT);

include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
backdrop_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES);
$config = config('statistics.settings');
if ($config->get('statistics_count_content_views') && $config->get('statistics_count_content_views_ajax')) {
  if (isset($_POST['nid'])) {
    $nid = $_POST['nid'];
    if (is_numeric($nid)) {
      db_merge('node_counter')
        ->key(array('nid' => $nid))
        ->fields(array(
          'daycount' => 1,
          'totalcount' => 1,
          'timestamp' => REQUEST_TIME,
        ))
        ->expression('daycount', 'daycount + 1')
        ->expression('totalcount', 'totalcount + 1')
        ->execute();
    }
  }
}

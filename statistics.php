<?php
/**
 * @file
 * Handles counts of node views via Ajax with minimal bootstrap.
 */

/**
* Root directory of Backdrop installation.
*/
define('BACKDROP_ROOT', substr($_SERVER['SCRIPT_FILENAME'], 0, strrpos($_SERVER['SCRIPT_FILENAME'], '/modules/')));
// Change the directory to the Backdrop root.
chdir(BACKDROP_ROOT);

include_once BACKDROP_ROOT . '/core/includes/bootstrap.inc';
backdrop_bootstrap(BACKDROP_BOOTSTRAP_VARIABLES);
$config = config('statistics.settings');
if ($config->get('count_content_views') && $config->get('count_content_views_ajax')) {
  // Anyone can post here, so only count an existing node of a counted type:
  // otherwise any integer adds a node_counter row.
  $nid = isset($_POST['nid']) ? filter_var($_POST['nid'], FILTER_VALIDATE_INT, array('options' => array('min_range' => 1))) : FALSE;
  if ($nid) {
    $type = db_query('SELECT type FROM {node} WHERE nid = :nid', array(':nid' => $nid))->fetchField();
    if ($type && in_array($type, (array) $config->get('count_node_types'))) {
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
}

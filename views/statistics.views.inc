<?php

/**
 * @file
 * Statistics: Views support
 */

/**
 * Implements hook_views_data_alter().
 */
function statistics_views_data_alter(&$data) {
  $data['node']['daycount'] = array(
    'title' => t('Pageviews today'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'statistics_views_handler_field_daycount_numeric',
      'help' => t('The number of times the node has been viewed today.'),
     ),
  );
  $data['node']['weekcount'] = array(
    'title' => t('Pageviews this week'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'statistics_views_handler_field_weekcount_numeric',
      'help' => t('The number of times the node has been viewed this week.'),
     ),
  );
  $data['node']['monthcount'] = array(
    'title' => t('Pageviews this month'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'statistics_views_handler_field_monthcount_numeric',
      'help' => t('The number of times the node has been viewed this month.'),
     ),
  );
  $data['node']['yearcount'] = array(
    'title' => t('Pageviews this year'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'statistics_views_handler_field_yearcount_numeric',
      'help' => t('The number of times the node has been viewed this year.'),
     ),
  );
  $data['node']['totalcount'] = array(
    'title' => t('Total pageviews'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'statistics_views_handler_field_totalcount_numeric',
      'help' => t('The total number of times the node has been viewed.'),
     ),
  );
}

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
    'help' => t('The number of times the node has been viewed today.'),
    'field' => array(
      'handler' => 'statistics_views_handler_field_daycount_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
     ),
  );
  $data['node']['weekcount'] = array(
    'title' => t('Pageviews this week'),
    'real field' => 'nid',
    'help' => t('The number of times the node has been viewed this week.'),
    'field' => array(
      'handler' => 'statistics_views_handler_field_weekcount_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
     ),
  );
  $data['node']['monthcount'] = array(
    'title' => t('Pageviews this month'),
    'real field' => 'nid',
    'help' => t('The number of times the node has been viewed this month.'),
    'field' => array(
      'handler' => 'statistics_views_handler_field_monthcount_numeric',
      'click sortable' => TRUE,
     ),
     'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['node']['yearcount'] = array(
    'title' => t('Pageviews this year'),
    'real field' => 'nid',
    'help' => t('The number of times the node has been viewed this year.'),
    'field' => array(
      'handler' => 'statistics_views_handler_field_yearcount_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['node']['totalcount'] = array(
    'title' => t('Total pageviews'),
    'real field' => 'nid',
    'help' => t('The total number of times the node has been viewed.'),
    'field' => array(
      'handler' => 'statistics_views_handler_field_totalcount_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
}

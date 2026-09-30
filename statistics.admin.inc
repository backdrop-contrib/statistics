<?php

/**
 * @file
 * Admin page callbacks for the Statistics module.
 */

/**
 * Page callback: Displays the "recent hits" page.
 *
 * This displays the pages with recent hits in a given time interval that
 * haven't been flushed yet. The flush interval is set on the statistics
 * settings form, but is dependent on cron running.
 *
 * @return
 *   A render array containing information about the most recent hits.
 */
function statistics_recent_hits() {
  backdrop_add_library('system', 'backdrop.ajax');
  $header = array(
    array('data' => t('Timestamp'), 'field' => 'a.timestamp', 'sort' => 'desc'),
    array('data' => t('Page'), 'field' => 'a.path'),
    array('data' => t('User'), 'field' => 'u.name'),
    array('data' => t('Operations'))
  );

  $query = db_select('accesslog', 'a', array('target' => 'slave'))->extend('PagerDefault')->extend('TableSort');
  $query->join('users', 'u', 'a.uid = u.uid');
  $query
    ->fields('a', array('aid', 'timestamp', 'path', 'title', 'uid'))
    ->fields('u', array('name'))
    ->limit(30)
    ->orderByHeader($header);

  $result = $query->execute();
  $rows = array();
  foreach ($result as $log) {
    $options = array(
      'attributes' => array(
        'class' => array(
          'use-ajax'),
        'data-dialog' => 'true',
        'data-dialog-options' => json_encode(array('width' => 700, 'dialogClass' => 'project-dialog')),
      )
    );
    $rows[] = array(
      array('data' => format_date($log->timestamp, 'short'), 'class' => array('nowrap')),
      _statistics_format_item($log->title, $log->path),
      theme('username', array('account' => $log)),
      l(t('details'), "admin/reports/statistics/hits/$log->aid", $options)
    );
  }

  $build['statistics_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No statistics available.'),
  );
  $build['statistics_pager'] = array('#theme' => 'pager');
  return $build;
}

/**
 * Page callback: Displays statistics for the "top pages" (most accesses).
 *
 * This displays the pages with the most hits (the "top pages") within a given
 * time period that haven't been flushed yet. The flush interval is set on the
 * statistics settings form, but is dependent on cron running.
 *
 * @return
 *   A render array containing information about the top pages.
 */
function statistics_top_pages() {
  $header = array(
    array('data' => t('Hits'), 'field' => 'hits', 'sort' => 'desc'),
    array('data' => t('Page'), 'field' => 'path'),
    array('data' => statistics_record_method() == 'browser' ? t('Average response time') : t('Average page generation time'), 'field' => 'average_time'),
    array('data' => statistics_record_method() == 'browser' ? t('Total response time') : t('Total page generation time'), 'field' => 'total_time')
  );

  $query = db_select('accesslog', 'a', array('target' => 'slave'))->extend('PagerDefault')->extend('TableSort');
  $query->addExpression('COUNT(path)', 'hits');
  // MAX(title) avoids having empty node titles which otherwise causes
  // duplicates in the top pages list.
  $query->addExpression('MAX(title)', 'title');
  $query->addExpression('AVG(timer)', 'average_time');
  $query->addExpression('SUM(timer)', 'total_time');

  $query
    ->fields('a', array('path'))
    ->groupBy('path')
    ->limit(30)
    ->orderByHeader($header);

  $count_query = db_select('accesslog', 'a', array('target' => 'slave'));
  $count_query->addExpression('COUNT(DISTINCT path)');
  $query->setCountQuery($count_query);

  $result = $query->execute();
  $rows = array();
  foreach ($result as $page) {
    $rows[] = array($page->hits, _statistics_format_item($page->title, $page->path), t('%time ms', array('%time' => round($page->average_time))), format_interval(round($page->total_time / 1000)));
  }

  $timer = config_get('statistics.settings', 'flush_accesslog_timer');
  // With "Never" the report covers the whole log, not "the past 0 sec".
  backdrop_set_title($timer ? t('Top pages in the past %interval', array('%interval' => format_interval($timer))) : t('Top pages'), PASS_THROUGH);
  $build['statistics_top_pages_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No statistics available.'),
  );
  $build['statistics_top_pages_pager'] = array('#theme' => 'pager');
  return $build;
}

/**
 * Page callback: Displays the "top visitors" page.
 *
 * This displays the pages with the top number of visitors in a given time
 * interval that haven't been flushed yet. The flush interval is set on the
 * statistics settings form, but is dependent on cron running.
 *
 * @return
 *   A render array containing the top visitors information.
 */
function statistics_top_visitors() {

  $header = array(
    array('data' => t('Hits'), 'field' => 'hits', 'sort' => 'desc'),
    array('data' => t('Visitor'), 'field' => 'u.name'),
    array('data' => statistics_record_method() == 'browser' ? t('Total response time') : t('Total page generation time'), 'field' => 'total'),
  );
  $query = db_select('accesslog', 'a', array('target' => 'slave'))->extend('PagerDefault')->extend('TableSort');
  $query->leftJoin('users', 'u', 'a.uid = u.uid');

  $query->addExpression('COUNT(a.uid)', 'hits');
  $query->addExpression('SUM(a.timer)', 'total');
  $query
    ->fields('a', array('uid', 'hostname'))
    ->fields('u', array('name'))
    ->groupBy('a.hostname')
    ->groupBy('a.uid')
    ->groupBy('u.name')
    ->limit(30)
    ->orderByHeader($header)
    ->orderBy('a.hostname');

  $uniques_query = db_select('accesslog')->distinct();
  $uniques_query->fields('accesslog', array('uid', 'hostname'));
  $count_query = db_select($uniques_query);
  $count_query->addExpression('COUNT(*)');
  $query->setCountQuery($count_query);

  $result = $query->execute();
  $rows = array();
  $destination = backdrop_get_destination();
  foreach ($result as $account) {
    $rows[] = array(
      $account->hits,
      ($account->uid ? theme('username', array('account' => $account)) : check_plain($account->hostname)),
      format_interval(round($account->total / 1000)),
    );
  }

  $timer = config_get('statistics.settings', 'flush_accesslog_timer');
  // With "Never" the report covers the whole log, not "the past 0 sec".
  backdrop_set_title($timer ? t('Top visitors in the past %interval', array('%interval' => format_interval($timer))) : t('Top visitors'), PASS_THROUGH);
  $build['statistics_top_visitors_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No statistics available.'),
  );
  $build['statistics_top_visitors_pager'] = array('#theme' => 'pager');
  return $build;
}

/**
 * Page callback: Displays the "top referrers" in the access logs.
 *
 * This displays the pages with the top referrers in a given time interval that
 * haven't been flushed yet. The flush interval is set on the statistics
 * settings form, but is dependent on cron running.
 *
 * @return
 *   A render array containing the top referrers information.
 */
function statistics_top_referrers() {
  $timer = config_get('statistics.settings', 'flush_accesslog_timer');
  // With "Never" the report covers the whole log, not "the past 0 sec".
  backdrop_set_title($timer ? t('Top referrers in the past %interval', array('%interval' => format_interval($timer))) : t('Top referrers'), PASS_THROUGH);

  $header = array(
    array('data' => t('Hits'), 'field' => 'hits', 'sort' => 'desc'),
    array('data' => t('Url'), 'field' => 'url'),
    array('data' => t('Last visit'), 'field' => 'last'),
  );
  $query = db_select('accesslog', 'a')->extend('PagerDefault')->extend('TableSort');

  $query->addExpression('COUNT(url)', 'hits');
  $query->addExpression('MAX(timestamp)', 'last');
  $query
    ->fields('a', array('url'))
    ->condition('url', '%' . db_like($_SERVER['HTTP_HOST']) . '%', 'NOT LIKE')
    ->condition('url', '', '<>')
    ->groupBy('url')
    ->limit(30)
    ->orderByHeader($header);

  $count_query = db_select('accesslog', 'a', array('target' => 'slave'));
  $count_query->addExpression('COUNT(DISTINCT url)');
  $count_query
    ->condition('url', '%' . db_like($_SERVER['HTTP_HOST']) . '%', 'NOT LIKE')
    ->condition('url', '', '<>');
  $query->setCountQuery($count_query);

  $result = $query->execute();
  $rows = array();
  foreach ($result as $referrer) {
    $rows[] = array($referrer->hits, _statistics_link($referrer->url), t('@time ago', array('@time' => format_interval(REQUEST_TIME - $referrer->last))));
  }

  $build['statistics_top_referrers_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No statistics available.'),
  );
  $build['statistics_top_referrers_pager'] = array('#theme' => 'pager');
  return $build;
}

/**
 * Page callback: Gathers page access statistics suitable for rendering.
 *
 * @param $aid
 *   The unique accesslog ID.
 *
 * @return
 *   A render array containing page access statistics. If information for the
 *   page was not found, backdrop_not_found() is called.
 */
function statistics_access_log($aid) {
  $access = db_query('SELECT a.*, u.name FROM {accesslog} a LEFT JOIN {users} u ON a.uid = u.uid WHERE aid = :aid', array(':aid' => $aid))->fetch();
  if ($access) {
    $rows[] = array(
      array('data' => t('URL'), 'header' => TRUE),
      l(url($access->path, array('absolute' => TRUE)), $access->path)
    );
    // It is safe to avoid filtering $access->title through check_plain because
    // it comes from backdrop_get_title().
    $rows[] = array(
      array('data' => t('Title'), 'header' => TRUE),
      $access->title
    );
    $rows[] = array(
      array('data' => t('Referrer'), 'header' => TRUE),
      ($access->url ? l($access->url, $access->url) : '')
    );
    $rows[] = array(
      array('data' => t('Date'), 'header' => TRUE),
      format_date($access->timestamp, 'long')
    );
    $rows[] = array(
      array('data' => t('User'), 'header' => TRUE),
      theme('username', array('account' => $access))
    );
    $rows[] = array(
      array('data' => t('Hostname'), 'header' => TRUE),
      check_plain($access->hostname)
    );

    $build['statistics_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
    );
    return $build;
  }
  return MENU_NOT_FOUND;
}

/**
 * Form constructor for the statistics administration form.
 *
 * Written for site owners rather than developers: each section says what it
 * records, what uses it, and how much it currently holds.
 *
 * @ingroup forms
 * @see statistics_settings_form_submit()
 */
function statistics_settings_form() {
  $config = config('statistics.settings');

  $form['intro'] = array(
    '#markup' => '<p>' . t('Statistics shows which pages and content are viewed, and where visitors came from. It is for recent activity and popular content on this site; for detailed visitor analytics, use a dedicated analytics service.') . '</p>',
  );

  // How visits are recorded.
  $form['method'] = array(
    '#type' => 'fieldset',
    '#title' => t('How visits are recorded'),
  );
  $form['method']['record_method'] = array(
    '#type' => 'radios',
    '#title' => t('Record visits'),
    '#title_display' => 'invisible',
    '#default_value' => statistics_record_method(),
    '#options' => array(
      'browser' => t('In the visitor\'s browser (recommended)'),
      'server' => t('On the server'),
    ),
    'browser' => array(
      '#description' => t('Each page reports its own visit once it has loaded. This counts pages served from the page cache, and skips most search engines and bots, which do not run JavaScript. Visitors with JavaScript turned off are not counted.'),
    ),
    'server' => array(
      '#description' => t('Visits are recorded while the page is built. Pages served from the page cache are NOT recorded, and bots are counted as visitors. Records the time taken to build each page.'),
    ),
  );
  if (statistics_record_method() == 'server' && config_get('system.core', 'cache')) {
    $form['method']['cache_warning'] = array(
      '#markup' => '<div class="messages warning">' . t('Page caching is enabled, so most anonymous visits are served from the cache and are not being recorded. Choose "In the visitor\'s browser" for accurate figures.') . '</div>',
    );
  }

  // Access log settings.
  $log_count = db_query('SELECT COUNT(*) FROM {accesslog}')->fetchField();
  $log_oldest = db_query('SELECT MIN(timestamp) FROM {accesslog}')->fetchField();
  $form['access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page visit log'),
    '#description' => t('The page visit log (access log) records every page visit, of any page, as a separate entry. The reports under <a href="@reports">Reports &gt; Statistics</a> and the Track tabs are built from it. It is separate from the content view counts below.', array('@reports' => url('admin/reports/statistics'))) . ' ' . ($log_count ? t('It currently holds @count visits, the oldest from @date.', array('@count' => number_format($log_count), '@date' => format_date($log_oldest, 'medium'))) : t('It is currently empty.')),
  );
  $form['access']['enable_access_log'] = array(
    '#type' => 'checkbox',
    '#title' => t('Record every page visit (access log)'),
    '#default_value' => $config->get('enable_access_log'),
    '#description' => t('Record the page and its title, the referring page, the visitor\'s IP address and user account, their browser, and the time. Adds one database write per page.'),
  );
  $form['access']['flush_accesslog_timer'] = array(
    '#type' => 'select',
    '#title' => t('Keep individual page visits for'),
    '#default_value' => $config->get('flush_accesslog_timer'),
    '#options' => array(0 => t('Never delete')) + backdrop_map_assoc(array(10, 3600, 10800, 21600, 32400, 43200, 86400, 172800, 259200, 604800, 1209600, 2419200, 4838400, 9676800), 'format_interval'),
    '#description' => t('Older entries are deleted when <a href="@cron">cron</a> runs, so the reports only cover this period. Content view counts and the daily history are not affected. "Never delete" keeps every entry, and the log grows with every page visit. "10 sec" empties the log at each cron run, which is mainly useful for testing.', array('@cron' => url('admin/reports/status'))),
  );

  // Content counter settings.
  $counted = db_query('SELECT COUNT(*) FROM {node_counter}')->fetchField();
  $form['content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content view counts'),
    '#description' => t('Content view counts are a running total kept for each piece of content, separate from the page visit log. They are shown by the view counter on content, the Popular content block, Views and tokens.') . ' ' . format_plural($counted, '1 content item has been viewed so far.', '@count content items have been viewed so far.'),
  );
  $form['content']['count_content_views'] = array(
    '#type' => 'checkbox',
    '#title' => t('Count how often content is viewed'),
    '#default_value' => $config->get('count_content_views'),
    '#description' => t('Count each view of content of the types selected below. Totals are kept for today, this week, this month and this year, which cron resets to zero at the end of each period, and for all time, which is never reset.'),
  );

  $types = node_type_get_types();
  $options = array();
  foreach ($types as $type => $info) {
    $options[$type] = check_plain(t($info->name));
  }

  $form['content']['count_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#options' => $options,
    '#default_value' => (array) $config->get('count_node_types'),
    '#description' => t('Choose one or more content types to count page views.'),
    '#states' => array(
      'visible' => array(
        ':input[name="count_content_views"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Daily history is not a setting of its own: it follows the two above.
  $history_days = db_query('SELECT COUNT(DISTINCT day) FROM {statistics_daily}')->fetchField();
  $form['history'] = array(
    '#type' => 'fieldset',
    '#title' => t('Daily history'),
    '#description' => t('A total of views per page per day, kept permanently and not affected by deleting old page visits. It covers every page while the page visit log is on, and counted content regardless. Use it in Views ("Statistics history") for charts and trends, and for the Trending list in the Popular content block.') . ' ' . format_plural($history_days, 'It currently covers 1 day.', 'It currently covers @count days.'),
  );

  // Excluded roles apply to the access log and the view counts alike, so they
  // have their own fieldset rather than depending on either checkbox.
  $form['excluded'] = array(
    '#type' => 'fieldset',
    '#title' => t('Excluded roles'),
  );
  $form['excluded']['node_count_excluded_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Exclude roles'),
    '#options' => array_map('check_plain', user_roles()),
    '#default_value' => (array) $config->get('node_count_excluded_roles'),
    '#description' => t('Visits by users with any selected role are neither recorded nor counted. If the administrator role is selected, user 1 is excluded too.'),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form submission handler for statistics_settings_form().
 *
 * Clears the page cache when recording changes, because cached pages carry
 * (or lack) the script that reports visits from the browser.
 */
function statistics_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $config = config('statistics.settings');
  $before = array($config->get('record_method'), $config->get('enable_access_log'), $config->get('count_content_views'));
  $config->set('record_method', $values['record_method'] == 'server' ? 'server' : 'browser');
  $config->set('enable_access_log', $values['enable_access_log']);
  $config->set('flush_accesslog_timer', $values['flush_accesslog_timer']);
  $config->set('count_content_views', $values['count_content_views']);
  $config->set('count_node_types', array_filter($values['count_node_types']));
  $config->set('node_count_excluded_roles', array_filter($values['node_count_excluded_roles']));
  $config->save();
  if ($before != array($config->get('record_method'), $config->get('enable_access_log'), $config->get('count_content_views'))) {
    cache('page')->flush();
  }
  backdrop_set_message(t('The configuration options have been saved.'));
}

<?php

/**
 * @file
 * Definition of statistics_views_handler_field_numeric.
 */

/**
 * Field handler to present numeric values from the statistics module.
 *
 * @ingroup views_field_handlers
 */
class statistics_views_handler_field_numeric extends views_handler_field_numeric {

  /**
   * {@inheritdoc}
   */
  public function access() {
    // Needs permission to see total page views.
    return user_access('view post access counter');
  }

}

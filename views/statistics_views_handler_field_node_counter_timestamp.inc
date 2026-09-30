<?php

/**
 * @file
 * Definition of statistics_views_handler_field_node_counter_timestamp.
 */

/**
 * Field handler to present the most recent time the node has been viewed.
 *
 * @ingroup views_field_handlers
 */
class statistics_views_handler_field_node_counter_timestamp extends views_handler_field_date {

  /**
   * {@inheritdoc}
   */
  public function access() {
    // Needs permission to see total page views.
    return user_access('view post access counter');
  }

}

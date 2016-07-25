<?php

/**
 * @file
 * Definition of statistics_views_handler_field_numeric.
 */

class statistics_views_handler_field_daycount_numeric extends views_handler_field_numeric {
  function option_definition() {
    $options = parent::option_definition();

    $options['separator'] = array('default' => ',', 'translatable' => TRUE);
    $options['suffix'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    if (!empty($this->definition['float'])) {
      $form['separator'] = array(
        '#type' => 'select',
        '#title' => t('Thousands marker'),
        '#options' => array(
          '' => t('- None -'),
          ',' => t('Comma'),
          ' ' => t('Space'),
          '.' => t('Decimal'),
          '\'' => t('Apostrophe'),
        ),
        '#default_value' => $this->options['separator'],
        '#description' => t('What single character to use as the thousands separator.'),
        '#size' => 2,
      );
    }
    $form['suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Suffix'),
      '#default_value' => $this->options['suffix'],
      '#description' => t('Text to put after the number, such as currency symbol.'),
    );
    unset($form['set_precision']);
    unset($form['precision']);
    unset($form['decimal']);
    unset($form['format_plural']);
    unset($form['format_plural_singular']);
    unset($form['format_plural_plural']);
    unset($form['prefix']);
  }

  function render($values) {
    $value = $this->get_value($values);
    $value = statistics_get($value);
    $value = number_format($value['daycount'], 0, '', $this->options['separator']);

    // Check to see if hiding should happen before adding prefix and suffix.
    if ($this->options['hide_empty'] && empty($value) && ($value !== 0)) {
      return '';
    }

    return $this->sanitize_value($value)
      . $this->sanitize_value($this->options['suffix'], 'xss');
  }
}

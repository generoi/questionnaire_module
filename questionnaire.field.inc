<?php

/**
 * Implements hook_field_info().
 */
function questionnaire_field_info() {
  return array(
    'question' => array(
      'label' => t('Question'),
      'description' => t('A question to answer.'),
      'settings' => array('max_length' => 255),
      'default_widget' => 'questionnaire_widget',
      'default_formatter' => 'questionnaire_formatter',
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function questionnaire_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];
  $form = array();

  if ($field['type'] == 'question') {
    $form['max_length'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum length'),
      '#default_value' => $settings['max_length'],
      '#required' => TRUE,
      '#description' => t('The maximum length of the field in characters.'),
      '#element_validate' => array('element_validate_integer_positive'),
      // @todo: If $has_data, add a validate handler that only allows
      // max_length to increase.
      '#disabled' => $has_data,
    );
  }

  return $form;
}

/**
 * Implements hook_field_formatter_info().
 */
function questionnaire_field_formatter_info() {
  return array(
    'questionnaire_formatter' => array(
      'label' => t('Question'),
      'field types' => array('question'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function questionnaire_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $elements = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('questionnaire'),
    ),
    '#attached' => array(
      'js' => array(
        array(
          'data' => drupal_get_path('module', 'questionnaire') . '/js/questionnaire.js',
          'type' => 'file',
        ),
      ),
    ),
  );

  // Display a form with questions.
  $elements[0] = drupal_get_form('questionnaire_question_form', $items, $entity, $entity_type);

  return $elements;
}

/**
 * Implements hook_field_widget_info().
 */
function questionnaire_field_widget_info() {
  return array(
    'questionnaire_widget' => array(
      'label' => t('Question'),
      'field types' => array('question'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function questionnaire_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $base = $element;

  if ($instance['widget']['type'] == 'questionnaire_widget') {
    $max_length = $field['settings']['max_length'];
    // Question text area.
    $element['question'] = array(
      '#type' => 'textarea',
      '#title' => t('Question'),
      '#default_value' => isset($items[$delta]['question']) ? $items[$delta]['question'] : NULL,
      '#description' => t('The question that you want to ask. Please note that this is limited to %limit characters.', array('%limit'=>$max_length)),
      '#attributes' => array(
        'class' => array('question-textarea'),
        'maxlength' => $max_length,
      ),
      '#attached' => array(
        'js' => array(
          array(
            'data' => drupal_get_path('module', 'questionnaire') . '/js/questionnaire.js',
            'type' => 'file',
          ),
          array(
            'data' => array('questionnaire' => array('maxLength' => $max_length)),
            'type' => 'setting',
          ),
        ),
      ),
    );
    // Answer options.
    $element['answer'] = array(
      '#type' => 'radios',
      '#title' => t('Your answer'),
      '#default_value' => isset($items[$delta]['answer']) ? $items[$delta]['answer'] : NULL,
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
      ),
    ) + $base;

    // Create field for adding website URL.
    if (isset($form['#entity'])) {
      $website = questionnaire_website_load($instance['entity_type'], $form['#entity']);
      $form['website'] = array(
        '#type' => 'textfield',
        '#title' => t('Website'),
        '#description' => t('An URL to your website.'),
        '#default_value' => !empty($website['website']) ? $website['website'] : '',
        '#required' => TRUE,
      );
      $form['motivation'] = array(
        '#type' => 'textfield',
        '#title' => t('Motivation'),
        '#description' => t('An URL to your reasonings.'),
        '#default_value' => !empty($website['motivation']) ? $website['motivation'] : '',
      );
      // Add extra validation for the website field.
      $form['#validate'][] = 'questionnaire_field_widget_form_validate';
    }
  };

  return $element;
}

/**
 * Implements hook_field_validate().
 */
function questionnaire_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    if (!empty($item['question'])) {
      // Check that the max length is not exceeded.
      if (!empty($field['settings']['max_length']) && drupal_strlen($item['question']) > $field['settings']['max_length']) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'questionnaire_max_length',
          'message' => t('%name: the value may not be longer than %max characters.', array('%name' => $instance['label'], '%max' => $field['settings']['max_length'])),
        );
      }
      // Answer can not be empty if question has a value.
      if (!isset($item['answer'])) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'questionnaire_required',
          'message' => t('%name: an answer is required.', array('%name' => $instance['label'])),
        );
      }
    }
  }
}

/**
 * Website field validation.
 */
function questionnaire_field_widget_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if (!empty($values['website'])) {
    if (!valid_url($values['website'], TRUE)) {
      form_set_error('website', t('The website URL is incorrect.'));
    }
  }
  if (!empty($values['motivation'])) {
    if (!valid_url($values['motivation'], TRUE)) {
      form_set_error('motivation', t('The motivation URL is incorrect.'));
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function questionnaire_field_is_empty($item, $field) {
  return empty($item['question']);
}

/**
 * Implements hook_field_insert().
 */
function questionnaire_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  if (!empty($entity->website)) {
    questionnaire_website_insert($entity_type, $entity);
  }
}

/**
 * Implements hook_field_update().
 */
function questionnaire_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  if (!empty($entity->website)) {
    questionnaire_website_insert($entity_type, $entity);
  }
}

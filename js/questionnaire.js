(function ($) {
  Drupal.behaviors.questionnaire = {
    attach: function (context, settings) {

      $(document).ready(function() {

        var $questionsForm = $('#questionnaire-question-form')
          , $questions     = $questionsForm.find('.form-type-radios')
          , $submit        = $questionsForm.find('.form-submit')
          , $result        = $questionsForm.find('.result')
          , $progress      = $questionsForm.find('.progress')
          , count          = $questions.length;

        $submit.hide();
        $result.fadeIn('100');

        // Hide all but first question.
        $questions.each(function(index, el) {
          if (index != 0) {
            $(this).hide();
          }
        });

        // Show next question when radio changes.
        $questions.find('input:radio').click(function() {
          var $this = $(this).closest('.form-type-radios')
            , index = $this.index()
            , $next = $questions.eq(index + 1);

          if ($next.length > 0) {
            $this.delay('200').fadeOut('100', function() {
              $next.fadeIn('100');
              $progress.find('.current').text(index + 2);
            });
          }
          else {
            $questionsForm.submit();
          }

        });

        // Count character length in textarea and limit to max length.
        var $textArea = $('.question-textarea');
        $textArea.keyup(function(event) {
          var $this        = $(this)
            , $wrapper     = $this.closest('.form-type-textarea')
            , $placeholder = $wrapper.find('.description').find('.placeholder')
            , maxLength    = settings.questionnaire.maxLength
            , currLength   = $this.val().length;

          $placeholder.text(maxLength - currLength);

          if (maxLength <= currLength) {
            event.preventDefault();
          }

        });

        $('.share-btn').click(function(event) {
          event.preventDefault();
          $this = $(this);
          postToFeed($this.attr('href'));
        });

        function postToFeed(url) {
          FB.ui({
            method: 'share',
            href: url,
          }, function(response) {});
        }

      });

    }
  };
})(jQuery);

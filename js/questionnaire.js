(function ($) {
  Drupal.questionnaire = Drupal.questionnaire || {};

  Drupal.behaviors.questionnaire = {
    attach: function (context, settings) {

      $(document).ready(function() {
        $("body").once(function() {


        var $questionsForm = $('#questionnaire-question-form')
          , $questions     = $questionsForm.find('.form-type-radios')
          , $submit        = $questionsForm.find('.form-submit')
          , $result        = $questionsForm.find('.result')
          , $progress      = $questionsForm.find('.progress')
          , count          = $questions.length
          , $preView       = $('#questionnaire-pre-view')
          , previewEnabled = $preView.length > 0
          , i
          , len;

        // if we haven't submitted yet and we have a "pre-view" to show before the quiz starts...
        if($questionsForm.find('#questionnaire-result').length == 0 && previewEnabled &&  $('#questionnaire-pre-view .start-button').length > 0) {
          $questionsForm.hide();

          $('#questionnaire-pre-view .start-button').click(function() {
            //alert("foo");
            $preView.hide();
            $questionsForm.show();
            callFunctions(afterStartFunctions);
          });

        // if we have submitted and then want to run the afterEndFunctions we make available...
        } else {
          $preView.hide();
          callFunctions(afterEndFunctions);
        }

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
            if (callFunctions(preSubmitFunctions)) {
              $questionsForm.submit();
            }
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

        // expose it...
        Drupal.questionnaire.postToFeed = postToFeed;

      });
     });

    }
  };


  var afterStartFunctions = [];
  var preSubmitFunctions = [];
  var afterEndFunctions = [];

  Drupal.questionnaire.afterStart = function(fn) {
    afterStartFunctions.push(fn);
  };

  // if one of these return false, we will not submit the form...
  Drupal.questionnaire.preSubmit = function(fn) {
    preSubmitFunctions.push(fn);
  };

  Drupal.questionnaire.afterEnd = function(fn) {
    afterEndFunctions.push(fn);
  };

  function callFunctions(fns_arr) {
    var i,
        len,
        retval = true; // we have a boolean here if we want one of the fns
                       // to return false, we can also return false for this loop

    if (fns_arr && fns_arr.length > 0) {
      for (i = 0, len = fns_arr.length; i < len; i++) {
        if (fns_arr[i]() === false) {
          retval = false;
        }
      }
    }
    return retval;
  }


})(jQuery);

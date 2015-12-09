/**
 * @file
 * Handle checkbox before sending mail logic.
 */

(function($){
  $(document).ready(function() {
    var send_btn = $('.js-aakb-survey-send');
    var check = $('.js-aakb-survey-sure');

    // Ensure that it's always unchecked after screen reload.
    check.prop('checked', false);

    // Listen to check event.
    check.click(function() {
      send_btn.toggleClass('form-button-disabled');
      send_btn.toggleClass('form-button-enabled');
    });

    send_btn.click(function() {
      if (send_btn.hasClass('form-button-enabled')) {
        $.getJSON( "/admin/config/people/survey/send/now", function(data) {
          console.log(data);
        });
      }
    });
  });
})(jQuery);

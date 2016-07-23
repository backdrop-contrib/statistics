(function ($) {
  $(document).ready(function() {
    $.ajax({
      type: "POST",
      cache: false,
      url: Backdrop.settings.statistics.url,
      data: Backdrop.settings.statistics.data
    });
  });
})(jQuery);

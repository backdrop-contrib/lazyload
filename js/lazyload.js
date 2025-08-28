/**
 * @file
 * Initiates Lazy Load plugin on a page.
 */
(function($) {
  Backdrop.behaviors.lazyload = {
    attach: function(context, settings) {
      //alert('lazy!');
      // Initiate LazyLoad plugin. Set unique selector and data_src
      // so it doesn't accidentally mess with other plugins.
      var lazyLoad = new LazyLoad({
        threshold: settings.lazyload.threshold,
        elements_selector: '.lazyload-item',
        data_src: 'lazyload-src',
        //callback_loaded: onLoad,
      });
      console.log('Lazyload js test message!');
    }
  };
})(jQuery);

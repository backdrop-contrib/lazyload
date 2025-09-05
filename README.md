Lazy Load
=========

This module improves image loading performance by delaying the loading of images
outside the viewport and by temporarily showing lightweight versions of large
images while they are being fetched. This speeds up the initial page rendering.

It is particularly useful for websites with heavy visual content, such as 
photography or art galleries. Faster rendering enhances user experience and may
also contribute to better search engine ranking.

Features
--------

- Defers image loading until they appear in the visible part of the browser
  window (lazy loading).
- Displays a smaller version of an image until the full-size version has
  finished loading.
- Allows restricting the module’s behavior to specific pages.
- Allows restricting the module’s behavior to images above a minimum file size.
- Optionally can show a blank image instead of a small preview image.

Known issues
------------

- Does not apply to background images (not using the _img_ tag), such as those
  in a hero block.
- May not work with images located outside the default public file directory as
  defined in Administration > Structure > File types > File settings. 

Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://docs.backdropcms.org/documentation/extend-with-modules.

- Visit the configuration page under Administration > Configuration > Media >
  Lazy Load (admin/config/media/lazyload) and adjust settings as you need.

Issues
------

Bugs and Feature Requests should be reported in the Issue Queue:
https://github.com/backdrop-contrib/mymodule/issues.


Current Maintainers
-------------------

- [Pavel](https://github.com/korontari)
- Additional maintainers are welcome.

Credits
-------

- Written for Backdrop CMS by [Pavel](https://github.com/korontari).
- [LazyLoad](https://github.com/verlok/vanilla-lazyload) javascript plugin (MIT license) by [Andrea Verlicchi](https://github.com/verlok).


License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.

/**
 * @file
 * Reports this page visit to statistics.php.
 *
 * This runs for every visitor, including those served a copy from the page
 * cache, which the server never sees. Most bots do not run JavaScript, so they
 * are not recorded.
 */
(function () {
  'use strict';

  /**
   * Sends the visit once, when the page has loaded.
   */
  function reportVisit() {
    var settings = window.Backdrop && Backdrop.settings && Backdrop.settings.statistics;
    if (!settings || !settings.url) {
      return;
    }
    var data = new URLSearchParams();
    var visit = settings.data || {};
    Object.keys(visit).forEach(function (key) {
      data.append(key, visit[key]);
    });
    data.append('referrer', document.referrer || '');

    // Server response time, in place of the page generation time a cached
    // page does not have.
    if (window.performance && performance.getEntriesByType) {
      var navigation = performance.getEntriesByType('navigation')[0];
      if (navigation && navigation.responseStart > 0) {
        data.append('timer', Math.round(navigation.responseStart - navigation.requestStart));
      }
    }

    if (navigator.sendBeacon && navigator.sendBeacon(settings.url, data)) {
      return;
    }
    var request = new XMLHttpRequest();
    request.open('POST', settings.url, true);
    request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    request.send(data.toString());
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', reportVisit);
  }
  else {
    reportVisit();
  }
})();

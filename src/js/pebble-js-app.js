Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
  var url = 'https://wrbrooks.github.io/bigagain/config.html';
  console.log('Showing configuration page: ' + url);

  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));

  var backgroundColor = configData.background_color;
  
  var dict = {};
  dict.KEY_INVERT_COLORS = configData.invert_colors;
  dict.KEY_COLOR_RED = parseInt(backgroundColor.substring(2, 4), 16);
  dict.KEY_COLOR_GREEN = parseInt(backgroundColor.substring(4, 6), 16);
  dict.KEY_COLOR_BLUE = parseInt(backgroundColor.substring(6), 16);

  
  console.log(JSON.stringify(dict));
  
  // Send to watchapp
  Pebble.sendAppMessage(dict, function() {
    console.log('Send successful: ' + JSON.stringify(dict));
  }, function() {
    console.log('Send failed!');
  });
});
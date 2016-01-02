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

  var color_a = configData.color_a;
  var color_b = configData.color_b;
  
  var dict = {};
  dict.KEY_COLOR_A_RED = parseInt(color_a.substring(2, 4), 16);
  dict.KEY_COLOR_A_GREEN = parseInt(color_a.substring(4, 6), 16);
  dict.KEY_COLOR_A_BLUE = parseInt(color_a.substring(6), 16);
  
  dict.KEY_COLOR_B_RED = parseInt(color_b.substring(2, 4), 16);
  dict.KEY_COLOR_B_GREEN = parseInt(color_b.substring(4, 6), 16);
  dict.KEY_COLOR_B_BLUE = parseInt(color_b.substring(6), 16);
  
  console.log(JSON.stringify(dict));
  
  // Send to watchapp
  Pebble.sendAppMessage(dict, function() {
    console.log('Send successful: ' + JSON.stringify(dict));
  }, function(d, e) {
    console.log("Send failed!");
    console.log(e);
    console.log(JSON.stringify(d));
  });
});
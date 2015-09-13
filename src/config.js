function getStorageValue(item, default_value){
    var retVal = localStorage.getItem(item);
    if (retVal === null || retVal == 'undefined' || retVal == 'null'){
        retVal = default_value;
    }
    return retVal;
}


Pebble.addEventListener('showConfiguration', function(e) {
  // Show config page
  //load storage values for seconds and date settings
  var useSeconds = parseInt((getStorageValue('KEY_USESECONDS','1')));
  var useBluetooth = parseInt((getStorageValue('KEY_USEBLUETOOTH','1')));
  var dateFormat = getStorageValue('KEY_DATEFORMAT','1');
  var settingsURI = "useSeconds=" + encodeURIComponent(parseInt(useSeconds))+"&showBluetooth=" +parseInt(useBluetooth) + "&dateFormat="+ encodeURIComponent(parseInt(dateFormat));
  Pebble.openURL('http://marks68.github.io/VideoGeek/settings_1_0_6.html?'+settingsURI);
});

Pebble.addEventListener('webviewclosed', function(e) {
  // Decode and parse config data as JSON
  var config_data = JSON.parse(decodeURIComponent(e.response));
  console.log('Config window returned: ', JSON.stringify(config_data));

  // Prepare AppMessage payload
  var dict = {
    'KEY_USESECONDS': parseInt(config_data.useSeconds),
    'KEY_USEBLUETOOTH': parseInt(config_data.showBluetooth),
    'KEY_DATEFORMAT': parseInt(config_data.dateFormat)
  };
  // save settings to Pebble watchap
  localStorage.setItem('KEY_USESECONDS',parseInt(config_data.useSeconds));
  localStorage.setItem('KEY_USEBLUETOOTH',parseInt(config_data.showBluetooth));
  localStorage.setItem('KEY_DATEFORMAT',parseInt(config_data.dateFormat));
  // Send settings to Pebble watchapp
  Pebble.sendAppMessage(dict, function(){
    console.log('Sent config data to Pebble');  
  }, function() {
    console.log('Failed to send config data!');
  });
});
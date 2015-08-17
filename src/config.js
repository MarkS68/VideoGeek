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
  console.log('value: '+getStorageValue('KEY_USESECONDS','1'));
  var temp = parseInt((getStorageValue('KEY_USESECONDS','1')));
  console.log(getStorageValue('KEY_USESECONDS','1'));
  console.log("temp:"+temp);
  var useSeconds = temp;
  var dateFormat = getStorageValue('KEY_DATEFORMAT','1');
  var settingsURI = "useSeconds=" + encodeURIComponent(parseInt(useSeconds)) + "&dateFormat="+ encodeURIComponent(parseInt(dateFormat));
  Pebble.openURL('https://people.pharmacy.purdue.edu/~sharpm/videogeek/index.html?'+settingsURI);
});

Pebble.addEventListener('webviewclosed', function(e) {
  // Decode and parse config data as JSON
  var config_data = JSON.parse(decodeURIComponent(e.response));
  console.log('Config window returned: ', JSON.stringify(config_data));

  // Prepare AppMessage payload
  var dict = {
    'KEY_USESECONDS': parseInt(config_data.useSeconds),
    'KEY_DATEFORMAT': parseInt(config_data.dateFormat)
  };
  // save settings to Pebble watchap
  localStorage.setItem('KEY_USESECONDS',parseInt(config_data.useSeconds));
  localStorage.setItem('KEY_DATEFORMAT',parseInt(config_data.dateFormat));
  // Send settings to Pebble watchapp
  Pebble.sendAppMessage(dict, function(){
    console.log('Sent config data to Pebble');  
  }, function() {
    console.log('Failed to send config data!');
  });
});
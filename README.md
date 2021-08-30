# arduino-water-level-measurement

The whole project was inspired by the work described in the follwoing posts:

- (https://arduino-projekte.info/zisterne-fuellstandsanzeige)[https://arduino-projekte.info/zisterne-fuellstandsanzeige]
- (https://arduino-projekte.info/zisterne-3-0-web-server-spiffs)[https://arduino-projekte.info/zisterne-3-0-web-server-spiffs]

## Requirements

The following libraries are required:

- (ESPAsyncWebServer)[https://github.com/me-no-dev/ESPAsyncWebServer]
- (ESPAsyncTCP)[https://github.com/me-no-dev/ESPAsyncTCP]

Just clone them into the `Arduino/libraries` folder.  
Also add the `ArduinoJson` Library to the project.

## Upload Files

 -  Navigate to the `Sketch` folder an create a now folder called `data`. In this folder, all website-related stuff like images, `.css` or `.html` files are stored.
 - The flash size of the corresponding _ESP8266 Board_ should be set to `4M (FS:1MB)` or `4M (1M SPIFFS)`.
- `.html`, `.css` files and icons are uploaded using the (Arduino ESP8266 filesystem uploader)[https://github.com/esp8266/arduino-esp8266fs-plugin].
- Download a release and put the resulting folder in the `tools` folder of your _Sketchbook_ folder.


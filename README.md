# mqtt-event-fade

Arduino MQTT event visualisation.

Expects an RGB LED to be wired to pins 23, 3, and 18.  Connects to the given WiFi network and MQTT broker and subscribes to the `dinky/motion` topic.  When it receives a "1" message it turns the RGB LED on red, and then slowly fades it through yellow and blue and green over the subsequent ten minutes.  After that the LED is turned off.

Ideallly it would fade gradually through the colours and get dimmer near the end, but it's currently deployed (see photos below) on an ESP32, which doesn't implement `analogWrite`, so fading the LEDs is tricky.

![Prototype with the LED lit red](https://pbs.twimg.com/media/EBoGnlvWkAI9n34.jpg)

![Prototype with the LED off](https://pbs.twimg.com/media/EBoGnErWsAEkYhN.jpg)

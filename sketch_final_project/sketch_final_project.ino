#include <Adafruit_MCP9808.h>

#include <Wire.h>
#include "Adafruit_MCP9808.h"

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

//int led = 8;
//int ledblue = 7;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      10

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

int pressurePin = A0;
int pressureValue = 0;

void setup() {

  pixels.begin(); // This initializes the NeoPixel library.
  pinMode(pressurePin, INPUT);

  Serial.begin(9600);


  while (!Serial); //waits for serial terminal to be open, necessary in newer arduino boards.
  Serial.println("MCP9808 demo");

  //pinMode(led, OUTPUT);
  //pinMode(ledblue, OUTPUT);

  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }

  Serial.println("Found MCP9808!");

  tempsensor.setResolution(3); // sets the resolution mode of reading, the modes are defined in the table bellow:
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms
}


void loop() {

  pressureValue = analogRead(pressurePin);

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  if (pressureValue < 200) {
    pixels.setBrightness(10);
    for (int i = 0; i < NUMPIXELS; i++) {

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Moderately bright green color.
      // pixels.setPixelColor(8, pixels.Color(0,0,255)); // Moderately bright green color.


      pixels.show(); // This sends the updated pixel color to the hardware.

      //delay(delayval); // Delay for a period of time (in milliseconds).

    }
  }

  if (pressureValue > 200 && pressureValue < 900) {
    pixels.setBrightness(100);
    for (int i = 0; i < NUMPIXELS; i++) {

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255

      pixels.setPixelColor(i, pixels.Color(0, 0, 255)); // Moderately bright green color.


      pixels.show(); // This sends the updated pixel color to the hardware.

      //delay(delayval); // Delay for a period of time (in milliseconds).

    }
  }

  if (pressureValue > 900) {
    pixels.setBrightness(250);
    for (int i = 0; i < NUMPIXELS; i++) {

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255

      pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Moderately bright green color.


      pixels.show(); // This sends the updated pixel color to the hardware.

      //delay(delayval); // Delay for a period of time (in milliseconds).

    }
  }
  Serial.println(pressureValue);

  Serial.println("wake up MCP9808.... "); // wake up MCP9808 - power consumption ~200 mikro Ampere
  tempsensor.wake();   // wake up, ready to read!

  // Read and print out the temperature, also shows the resolution mode used for reading.
  Serial.print("Resolution in mode: ");
  Serial.println (tempsensor.getResolution());
  float c = tempsensor.readTempC();
  float f = tempsensor.readTempF();


  //this is to turn off the lights when the temp goes down
  if (f > 83) {
    for (int i = 0; i < NUMPIXELS; i++) {

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255

      pixels.setPixelColor(i, pixels.Color(255, 0, 255)); // Moderately bright green color.this new one is purple


      pixels.show(); // This sends the updated pixel color to the hardware.

      //delay(delayval); // Delay for a period of time (in milliseconds).

    }
  }

  Serial.print("Temp: ");
  Serial.print(c, 4); Serial.print("*C\t and ");
  Serial.print(f, 4); Serial.println("*F.");

  delay(2000);
  Serial.println("Shutdown MCP9808.... ");
  tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
  Serial.println("");
  //delay(200);
}

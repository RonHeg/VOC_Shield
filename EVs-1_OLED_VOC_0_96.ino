/*evs
* EVs-1 Environmental Sensor PCB
* This sketch initializes the SHT40, SGP40, and ICP-10111 sensors, reads data from them, 
* and prints the values to the Serial Monitor. 
* It also displays environmental data on an attached 0.96" OLED Display.
* Configured for the 0.96" OLED display. Example Hosyond 0.96 Inch OLED I2C Display Module 128x64 Pixel available in Amazon
* Also tested with a 1.54" OLED display !!BE SURE TO VERIFY PINOUT OF DISPLAY!!
*
* EVs-1 PCB is designed to connect to the Seeed Studio XIAO ESP32C3
* This sketch is meant to be a quick starting point to developing your project
*
* Make sure you have the necessary libraries installed:
* Adafruit SHT4x Library
* Adafruit SGP40 Library
* Adafruit ICM20X Library
* Oliver U8g2 Library
* Install these libraries via the Arduino Library Manager.
*/

#include <Wire.h>
#include <Adafruit_SHT4x.h>
#include <Adafruit_SGP40.h>
#include <DFRobot_ICP10111.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); //U8g2lib has 'drivers' for quite a few other displays 

// Create sensor objects
Adafruit_SHT4x sht4 = Adafruit_SHT4x();
Adafruit_SGP40 sgp40 = Adafruit_SGP40();
DFRobot_ICP10111 icp;

void setup() {
  u8g2.begin();
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for Serial to be ready

  // Initialize I2C communication
  Wire.begin();

  // Initialize SHT40 - Temperature and Humidity
  if (!sht4.begin()) {
    Serial.println("Couldn't find SHT40 sensor!");
  }
  else {
    Serial.println("SHT40 sensor found!");
  }

  // Initialize SGP40 - VOC
  if (!sgp40.begin()) {
    Serial.println("Couldn't find SGP40 sensor!");
  }
  Serial.println("SGP40 sensor found!");

  // Initialize ICP-10111 - barometric pressure sensor 
  if (icp.begin() != 0) {
    Serial.println("Failed to initialize ICP-10111 sensor");
  }
  Serial.println("ICP-10111 sensor found!");
  /**
      * @brief Set work mode
      * |------------------|--------------|-------------------|----------------------|
      * |       api        |   mode       |Conversion Time(ms)|Pressure RMS Noise(Pa)|
      * |icp.eLowPower     |  Low Power   |      1.8          |        3.2           |
      * |icp.eNormal       |  Normal      |      6.3          |        1.6           |
      * |icp.eLowNoise     |  Low Noise   |      23.8         |        0.8           |
      * |icp.eUltraLowNoise|  Ultra-low Noise |      94.5         |        0.4           |
      */

  icp.setWorkPattern(icp.eNormal);
  u8g2.enableUTF8Print();

}

void loop() {

  uint16_t sraw;
  int32_t voc_index;

  // Read SHT40 data
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_7x14B_tf);
  //u8g2.setFont(u8g2_font_t0_11_tf);
  u8g2.drawStr(0,12, "SGP40 VOC Sensor");
  sensors_event_t humidity, temp;
  sht4.getEvent(&humidity, &temp);
  Serial.print("Temperature: ");
  float t = temp.temperature;
  Serial.print((t * 9/5) + 32);
  t = (t * 9/5) + 32;
  Serial.print(" °F, Humidity: ");
  float h = humidity.relative_humidity;
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");

  u8g2.setCursor(0,28);
  u8g2.print(String(t) + "°F, RH " + String(h) + "%");

  // Read SGP40 data
  uint16_t raw_index = sgp40.measureRaw(t, h);
  Serial.print("RAW Index: ");
  Serial.println(raw_index);

  voc_index = sgp40.measureVocIndex(t, h);
  Serial.print("Voc Index: ");
  Serial.println(voc_index);


  u8g2.setCursor(0,43);
  u8g2.print("VOC = " + String(voc_index));

  // Read ICP-10111 data
  Serial.print("Read air pressure:");
  
  float ap = icp.getAirPressure();
  Serial.print(String(ap));
  Serial.println("Pa");
  float Hg = ap/3386;
  Serial.print(String(Hg));
  Serial.println("Hg");
  Serial.print("Read temperature:");
  Serial.print(icp.getTemperature());
  Serial.println("℃");
  Serial.print("Read altitude:");
  float alt = icp.getElevation();
  Serial.print(String(alt));
  Serial.println("m");
  Serial.println("------------------------------");

  u8g2.setCursor(0,58);
  u8g2.print(String(Hg) + "inHg");
  u8g2.sendBuffer();					// transfer internal memory to the display

  delay(2000); // Wait for 2 seconds before next reading
}


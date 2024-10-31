/*This sketch initializes the SHT40, SGP40, and ICP-10111 sensors, reads data from them, 
* and prints the values to the Serial Monitor. 
* PCB is designed to connect to the Seeed Studio XIAO ESP32C3
* Make sure you have the necessary libraries installed:*
*
* Adafruit SHT4x Library
* Adafruit SGP40 Library
* Adafruit ICM20X Library
* Install these libraries via the Arduino Library Manager.
* Configured for the 0.96" OLED display

*/

#include <Wire.h>
#include <Adafruit_SHT4x.h>
#include <Adafruit_SGP40.h>
//#include <Adafruit_ICM20X.h>
#include <DFRobot_ICP10111.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Create sensor objects
Adafruit_SHT4x sht4 = Adafruit_SHT4x();
Adafruit_SGP40 sgp40 = Adafruit_SGP40();
//Adafruit_ICM20X icp10111 = Adafruit_ICM20X();
DFRobot_ICP10111 icp;

void setup() {
  u8g2.begin();
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait for Serial to be ready

  // Initialize I2C communication
  Wire.begin();

  // Initialize SHT40
  if (!sht4.begin()) {
    Serial.println("Couldn't find SHT40 sensor!");
    //while (1) delay(10);
  }
  else {
    Serial.println("SHT40 sensor found!");
  }

  // Initialize SGP40
  if (!sgp40.begin()) {
    Serial.println("Couldn't find SGP40 sensor!");
  //  while (1) delay(10);
  }
  Serial.println("SGP40 sensor found!");

  // Initialize ICP-10111
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
  u8g2.drawStr(0,12, "WeatherShield");
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
  uint16_t raw_index = sgp40.measureRaw();
  Serial.print("RAW Index: ");
  Serial.println(raw_index);

  voc_index = sgp40.measureVocIndex(t, h);
  Serial.print("Voc Index: ");
  Serial.println(voc_index);

  u8g2.setCursor(0,43);
  u8g2.print("VOC = " + String(voc_index));

  // Read ICP-10111 data
  Serial.print("Read air pressure:");
  
  uint16_t ap = icp.getAirPressure();
  Serial.print(ap);
  Serial.println("Pa");
  Serial.print("Read temperature:");
  Serial.print(icp.getTemperature());
  Serial.println("℃");
  Serial.print("Read altitude:");
  uint16_t alt = icp.getElevation();
  Serial.print(icp.getElevation());
  Serial.println("m");
  Serial.println("------------------------------");

  u8g2.setCursor(0,58);
  u8g2.print(String(ap) + "Pa");
  u8g2.sendBuffer();					// transfer internal memory to the display

  delay(2000); // Wait for 2 seconds before next reading
}


VOC Shield is a small PCB containing Temperature, Humidity, Pressure and VOC sensors meant to connect to the XIAO series of micros. 
The sensors are: 
Sensirion AG SHT40-AD1B Temperature and Humidity.
Sensirion AG SGP40-D-R4 VOC.
TDK ICP-10111 Barometric Pressure and Temperature.
The board uses a 3.3VDC supply and communicates via I2C.
Code examples are for a Seeed Studio XIAO ESP32C3.

WeatherShield.ino just sends data thru the serial port at a baud rate of 115,200.
OLEDWeatherShield0_96 also displays 4 lines on a 0.96" OLED display (please verify pinout).
The display reads:
	WeatherShield
	78.23F, RH 48.00%
	VOC = 94
	32555Pa
(Numbers in the data are an example, your readings should be the actual for your conditions)




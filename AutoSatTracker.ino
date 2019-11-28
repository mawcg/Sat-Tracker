#include <EthernetClient.h>
#include <Ethernet.h>
#include <Dhcp.h>
#include <EthernetServer.h>
#include <Dns.h>
#include <EthernetUdp.h>

#include <Ethernet.h>

/* Autonomous Satellite Tracker.
 *
 * Connections on Arduino Mega 2560:
 *   Servo controller connects to SDA-20 and SCL-21
 *   WiFi shield uses IRQ-3, VBEN-5, CS-10 plus 5V, Gnd, Reset, MOSI, MISO and CLK via ISP
 *   GPS uses hardware Serial1 port TX1-18 and RX1-17
 *
 */


#include <EEPROM.h>
#include <SPI.h>
#include <Wire.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_GPS.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "NV.h"
#include "Sensor.h"
#include "Circum.h"
#include "Gimbal.h"
#include "Target.h"
#include "Webpage.h"

NV *nv;
Sensor *sensor;
Circum *circum;
Gimbal *gimbal;
Target *target;
Webpage *webpage;

/* get current amount of free RAM.
 * lifted from Adafruit CC3000 library.
 */
int
getFreeRam(void)
{
  extern int  __bss_end;
  extern int  *__brkval;
  int free_memory;
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval);
  }

  return free_memory;
}

/* called first and once
 */
void
setup()
{
    // init serial monitor
    Serial.begin (115200);

    // instantiate each module
    Serial.println (F("making NV"));
    nv = new NV();
    Serial.println (F("making Sensor"));
    sensor = new Sensor();
    Serial.println (F("making Circum"));
    circum = new Circum();
    Serial.println (F("making Gimbal"));
    gimbal = new Gimbal();
    Serial.println (F("making Target"));
    target = new Target();
    Serial.println (F("making Webpage"));
    webpage = new Webpage();

    // display available RAM
    Serial.print(F("Free RAM now: ")); Serial.println(getFreeRam(), DEC);

    // all set to go.. see you in loop()

}

/* called repeatedly forever
 */
void
loop()
{
    // check for ethernet activity
    webpage->checkEthernet();

    // check for new GPS info
    circum->checkGPS();

    // follow the target
    target->track();
}

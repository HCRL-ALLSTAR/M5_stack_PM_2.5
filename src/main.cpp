#include <Arduino.h>
#include <AM2320.h>

AM2320 th(&Wire);

/**
 * For example sps30 & esp32
 * Mod by Sonthaya Boonchan @HONEYLab
 * 05/02/2019
 * 
 * Hardware test
 * -> esp32 lite (mcu)
 * -> sps30 (sensor)
 * 
 * Communication uart
 * rx -> 16
 * tx -> 17
 * 
 * Please edit file sensirion_uart.cpp for change pin or port communication
*/
#include "sensirion_uart.h"
#include "sps30.h"
#include "Display.h" 
#include "utility\Power.h"
#include "RGBStrip\RGBStrip.h"

int last_1 = -1,last_2 = -1,last_3 = -1;
int last_11 = -1, last_12 = -1, last_13 = -1;
int last_21 = -1, last_22 = -1;
int last_31 = -1, last_32 = -1;
int last_bt = -1;
void PM2p5(int value);
void PM10(int value);
void TEMP(int value);
void HUMI(int value);

void setAllStrip(uint8_t r, uint8_t g, uint8_t b);

struct sps30_measurement measurement;
s16 ret;
uint8_t thR;

uint16_t ringColor;

Display Lcd = Display();
POWER power = POWER();
RGBStrip strip = RGBStrip();

void setup()
{
  Lcd.begin();
  SPIFFS.begin();
  Serial.begin(115200);

  strip.begin();
  strip.setBrightness(10);
  for(int i = 0; i<STRIP_NUM; i++){
    strip.set_status(i,1);
  }

  sensirion_uart_open();
  Wire.begin();
  while (sps30_probe() != 0)
  {
    Serial.println("probe failed");
    delay(1000);
  }
  //sps30_set_fan_auto_cleaning_interval(900);

  /* start measurement and wait for 10s to ensure the sensor has a
     * stable flow and possible remaining particles are cleaned out */
  if (sps30_start_measurement() != 0)
  {
    Serial.println("error starting measurement");
  }

  Lcd.drawPngFile(SPIFFS, "/template.png", 0, 0);
}

void loop()
{
  thR = th.Read();
  ret = sps30_read_measurement(&measurement);
  if(thR == 0) //2 = CRC Failed 1 = Sensor Offline 0 = OK
  {
    TEMP(th.cTemp);
    HUMI(th.Humidity);
  }
  if (ret >= 0) //Read Successful
  {
    if (!SPS_IS_ERR_STATE(ret)) //Accurate
    {
      float pm2p5 = measurement.mc_2p5;
      PM2p5(pm2p5);
      PM10(measurement.mc_10p0);
      if(pm2p5 <= 12) 
      {
        ringColor = 0x377B;
        setAllStrip(52, 239, 227);
      }
      else if (pm2p5 <= 35)
      {
        ringColor = GREEN;
        setAllStrip(0, 255, 0);
      }
      else if (pm2p5 <= 55)
      {
        ringColor = YELLOW;
        setAllStrip(255, 255, 0);
      }
      else if (pm2p5 <= 150)
      {
        ringColor = ORANGE;
        setAllStrip(255, 166, 0);
      }
      else if (pm2p5 <= 250)
      {
        ringColor = RED;
        setAllStrip(255, 0, 0);
      }
      else
      {
        ringColor = 0x4008;
        setAllStrip(64, 0, 64);
      }
    }
  }
  int current_bt = power.getBatteryLevel();
  if(last_bt != current_bt)
  {
    Lcd.setTextColor(BLACK);
    Lcd.setCursor(5, 5);
    Lcd.printf("%d", last_bt);
    Lcd.setTextColor(WHITE);
    Lcd.setCursor(5,5);
    Lcd.printf("%d",current_bt);
    last_bt = current_bt;
  }
  Lcd.drawCircle(160, 120, 158, ringColor);
  Lcd.drawCircle(160, 120, 157, ringColor);
  Lcd.drawCircle(160, 120, 156, ringColor);
  strip.update();
  delay(300);
  // sps30_stop_measurement();
  // sensirion_uart_close();
}

void PM2p5(int value)
{
  int first = value/100;
  int second = (value%100)/10;
  int third = value%10;
  if(first != last_1){
    Lcd.fillRect(83, 72, 46, 72, BLACK);
    switch (first)
    {
    case 0:
      Lcd.drawPngFile(SPIFFS, "/0b.png", 83, 72);
      break;
    case 1:
      Lcd.drawPngFile(SPIFFS, "/1b.png", 83, 72);
      break;
    case 2:
      Lcd.drawPngFile(SPIFFS, "/2b.png", 83, 72);
      break;
    case 3:
      Lcd.drawPngFile(SPIFFS, "/3b.png", 83, 72);
      break;
    case 4:
      Lcd.drawPngFile(SPIFFS, "/4b.png", 83, 72);
      break;
    case 5:
      Lcd.drawPngFile(SPIFFS, "/5b.png", 83, 72);
      break;
    case 6:
      Lcd.drawPngFile(SPIFFS, "/6b.png", 83, 72);
      break;
    case 7:
      Lcd.drawPngFile(SPIFFS, "/7b.png", 83, 72);
      break;
    case 8:
      Lcd.drawPngFile(SPIFFS, "/8b.png", 83, 72);
      break;
    case 9:
      Lcd.drawPngFile(SPIFFS, "/9b.png", 83, 72);
      break;
    }
    last_1 = first;
  }

  if (second != last_2)
  {
    Lcd.fillRect(83 + 46 + 8, 72, 46, 72, BLACK);
    switch (second)
    {
    case 0:
      Lcd.drawPngFile(SPIFFS, "/0b.png", 83 + 46 + 8, 72);
      break;
    case 1:
      Lcd.drawPngFile(SPIFFS, "/1b.png", 83 + 46 + 8, 72);
      break;
    case 2:
      Lcd.drawPngFile(SPIFFS, "/2b.png", 83 + 46 + 8, 72);
      break;
    case 3:
      Lcd.drawPngFile(SPIFFS, "/3b.png", 83 + 46 + 8, 72);
      break;
    case 4:
      Lcd.drawPngFile(SPIFFS, "/4b.png", 83 + 46 + 8, 72);
      break;
    case 5:
      Lcd.drawPngFile(SPIFFS, "/5b.png", 83 + 46 + 8, 72);
      break;
    case 6:
      Lcd.drawPngFile(SPIFFS, "/6b.png", 83 + 46 + 8, 72);
      break;
    case 7:
      Lcd.drawPngFile(SPIFFS, "/7b.png", 83 + 46 + 8, 72);
      break;
    case 8:
      Lcd.drawPngFile(SPIFFS, "/8b.png", 83 + 46 + 8, 72);
      break;
    case 9:
      Lcd.drawPngFile(SPIFFS, "/9b.png", 83 + 46 + 8, 72);
      break;
    }
    last_2 = second;
  }
  if (third != last_3)
  {
    Lcd.fillRect(83 + 46 + 8 + 46 + 8, 72, 46, 72, BLACK);
    switch (third)
    {
    case 0:
      Lcd.drawPngFile(SPIFFS, "/0b.png", 83 + 46 + 8 + 46 + 8, 72);
      break;
    case 1:
      Lcd.drawPngFile(SPIFFS, "/1b.png", 83 + 46 + 8 + 46 + 8, 72);
      break;
    case 2:
      Lcd.drawPngFile(SPIFFS, "/2b.png", 83 + 46 + 8 + 46 + 8, 72);
      break;
    case 3:
      Lcd.drawPngFile(SPIFFS, "/3b.png", 83 + 46 + 8 + 46 + 8, 72);
      break;
    case 4:
      Lcd.drawPngFile(SPIFFS, "/4b.png", 83 + 46 + 8 + 46 + 8, 72);
      break;
    case 5:
      Lcd.drawPngFile(SPIFFS, "/5b.png", 83 + 46 + 8 + 46 + 8, 72);
      break;
    case 6:
      Lcd.drawPngFile(SPIFFS, "/6b.png", 83 + 46 + 8 + 46 + 8, 72);
      break;
    case 7:
      Lcd.drawPngFile(SPIFFS, "/7b.png", 83 + 46 + 8 + 46 + 8, 72);
      break;
    case 8:
      Lcd.drawPngFile(SPIFFS, "/8b.png", 83 + 46 + 8 + 46 + 8, 72);
      break;
    case 9:
      Lcd.drawPngFile(SPIFFS, "/9b.png", 83 + 46 + 8 + 46 + 8, 72);
      break;
    }
    last_3 = third;
  }
}

void PM10(int value)
{
  int first = value / 100;
  int second = (value % 100) / 10;
  int third = value % 10;
  if (first != last_11)
  {
    Lcd.fillRect(187, 159, 15, 23, BLACK);
    switch (first)
    {
    case 0:
      Lcd.drawPngFile(SPIFFS, "/0s.png", 187, 159);
      break;
    case 1:
      Lcd.drawPngFile(SPIFFS, "/1s.png", 187, 159);
      break;
    case 2:
      Lcd.drawPngFile(SPIFFS, "/2s.png", 187, 159);
      break;
    case 3:
      Lcd.drawPngFile(SPIFFS, "/3s.png", 187, 159);
      break;
    case 4:
      Lcd.drawPngFile(SPIFFS, "/4s.png", 187, 159);
      break;
    case 5:
      Lcd.drawPngFile(SPIFFS, "/5s.png", 187, 159);
      break;
    case 6:
      Lcd.drawPngFile(SPIFFS, "/6s.png", 187, 159);
      break;
    case 7:
      Lcd.drawPngFile(SPIFFS, "/7s.png", 187, 159);
      break;
    case 8:
      Lcd.drawPngFile(SPIFFS, "/8s.png", 187, 159);
      break;
    case 9:
      Lcd.drawPngFile(SPIFFS, "/9s.png", 187, 159);
      break;
    }
    last_11 = first;
  }

  if (second != last_12)
  {
    Lcd.fillRect(187 + 15 + 2, 159, 15, 23, BLACK);
    switch (second)
    {
    case 0:
      Lcd.drawPngFile(SPIFFS, "/0s.png", 187 + 15 + 2, 159);
      break;
    case 1:
      Lcd.drawPngFile(SPIFFS, "/1s.png", 187 + 15 + 2, 159);
      break;
    case 2:
      Lcd.drawPngFile(SPIFFS, "/2s.png", 187 + 15 + 2, 159);
      break;
    case 3:
      Lcd.drawPngFile(SPIFFS, "/3s.png", 187 + 15 + 2, 159);
      break;
    case 4:
      Lcd.drawPngFile(SPIFFS, "/4s.png", 187 + 15 + 2, 159);
      break;
    case 5:
      Lcd.drawPngFile(SPIFFS, "/5s.png", 187 + 15 + 2, 159);
      break;
    case 6:
      Lcd.drawPngFile(SPIFFS, "/6s.png", 187 + 15 + 2, 159);
      break;
    case 7:
      Lcd.drawPngFile(SPIFFS, "/7s.png", 187 + 15 + 2, 159);
      break;
    case 8:
      Lcd.drawPngFile(SPIFFS, "/8s.png", 187 + 15 + 2, 159);
      break;
    case 9:
      Lcd.drawPngFile(SPIFFS, "/9s.png", 187 + 15 + 2, 159);
      break;
    }
    last_12 = second;
  }

  if (third != last_13)
  {
    Lcd.fillRect(187 + 15 + 2 + 15 + 2, 159, 15, 23, BLACK);
    switch (third)
    {
    case 0:
      Lcd.drawPngFile(SPIFFS, "/0s.png", 187 + 15 + 2 + 15 + 2, 159);
      break;
    case 1:
      Lcd.drawPngFile(SPIFFS, "/1s.png", 187 + 15 + 2 + 15 + 2, 159);
      break;
    case 2:
      Lcd.drawPngFile(SPIFFS, "/2s.png", 187 + 15 + 2 + 15 + 2, 159);
      break;
    case 3:
      Lcd.drawPngFile(SPIFFS, "/3s.png", 187 + 15 + 2 + 15 + 2, 159);
      break;
    case 4:
      Lcd.drawPngFile(SPIFFS, "/4s.png", 187 + 15 + 2 + 15 + 2, 159);
      break;
    case 5:
      Lcd.drawPngFile(SPIFFS, "/5s.png", 187 + 15 + 2 + 15 + 2, 159);
      break;
    case 6:
      Lcd.drawPngFile(SPIFFS, "/6s.png", 187 + 15 + 2 + 15 + 2, 159);
      break;
    case 7:
      Lcd.drawPngFile(SPIFFS, "/7s.png", 187 + 15 + 2 + 15 + 2, 159);
      break;
    case 8:
      Lcd.drawPngFile(SPIFFS, "/8s.png", 187 + 15 + 2 + 15 + 2, 159);
      break;
    case 9:
      Lcd.drawPngFile(SPIFFS, "/9s.png", 187 + 15 + 2 + 15 + 2, 159);
      break;
    }
    last_13 = third;
  }
}

void TEMP(int value)
{
  int first = value / 10;
  int second = value % 10;
  if (first != last_21)
  {
    Lcd.fillRect(83, 159, 15, 23, BLACK);
    switch (first)
    {
    case 0:
      Lcd.drawPngFile(SPIFFS, "/0s.png", 83, 159);
      break;
    case 1:
      Lcd.drawPngFile(SPIFFS, "/1s.png", 83, 159);
      break;
    case 2:
      Lcd.drawPngFile(SPIFFS, "/2s.png", 83, 159);
      break;
    case 3:
      Lcd.drawPngFile(SPIFFS, "/3s.png", 83, 159);
      break;
    case 4:
      Lcd.drawPngFile(SPIFFS, "/4s.png", 83, 159);
      break;
    case 5:
      Lcd.drawPngFile(SPIFFS, "/5s.png", 83, 159);
      break;
    case 6:
      Lcd.drawPngFile(SPIFFS, "/6s.png", 83, 159);
      break;
    case 7:
      Lcd.drawPngFile(SPIFFS, "/7s.png", 83, 159);
      break;
    case 8:
      Lcd.drawPngFile(SPIFFS, "/8s.png", 83, 159);
      break;
    case 9:
      Lcd.drawPngFile(SPIFFS, "/9s.png", 83, 159);
      break;
    }
    last_21 = first;
  }

  if (second != last_22)
  {
    Lcd.fillRect(83 + 15 + 2, 159, 15, 23, BLACK);
    switch (second)
    {
    case 0:
      Lcd.drawPngFile(SPIFFS, "/0s.png", 83 + 15 + 2, 159);
      break;
    case 1:
      Lcd.drawPngFile(SPIFFS, "/1s.png", 83 + 15 + 2, 159);
      break;
    case 2:
      Lcd.drawPngFile(SPIFFS, "/2s.png", 83 + 15 + 2, 159);
      break;
    case 3:
      Lcd.drawPngFile(SPIFFS, "/3s.png", 83 + 15 + 2, 159);
      break;
    case 4:
      Lcd.drawPngFile(SPIFFS, "/4s.png", 83 + 15 + 2, 159);
      break;
    case 5:
      Lcd.drawPngFile(SPIFFS, "/5s.png", 83 + 15 + 2, 159);
      break;
    case 6:
      Lcd.drawPngFile(SPIFFS, "/6s.png", 83 + 15 + 2, 159);
      break;
    case 7:
      Lcd.drawPngFile(SPIFFS, "/7s.png", 83 + 15 + 2, 159);
      break;
    case 8:
      Lcd.drawPngFile(SPIFFS, "/8s.png", 83 + 15 + 2, 159);
      break;
    case 9:
      Lcd.drawPngFile(SPIFFS, "/9s.png", 83 + 15 + 2, 159);
      break;
    }
    last_22 = second;
  }
}

void HUMI(int value)
{
  int first = value / 10;
  int second = value % 10;
  if (first != last_31)
  {
    Lcd.fillRect(137, 159, 15, 23, BLACK);
    switch (first)
    {
    case 0:
      Lcd.drawPngFile(SPIFFS, "/0s.png", 137, 159);
      break;
    case 1:
      Lcd.drawPngFile(SPIFFS, "/1s.png", 137, 159);
      break;
    case 2:
      Lcd.drawPngFile(SPIFFS, "/2s.png", 137, 159);
      break;
    case 3:
      Lcd.drawPngFile(SPIFFS, "/3s.png", 137, 159);
      break;
    case 4:
      Lcd.drawPngFile(SPIFFS, "/4s.png", 137, 159);
      break;
    case 5:
      Lcd.drawPngFile(SPIFFS, "/5s.png", 137, 159);
      break;
    case 6:
      Lcd.drawPngFile(SPIFFS, "/6s.png", 137, 159);
      break;
    case 7:
      Lcd.drawPngFile(SPIFFS, "/7s.png", 137, 159);
      break;
    case 8:
      Lcd.drawPngFile(SPIFFS, "/8s.png", 137, 159);
      break;
    case 9:
      Lcd.drawPngFile(SPIFFS, "/9s.png", 137, 159);
      break;
    }
    last_31 = first;
  }

  if (second != last_32)
  {
    Lcd.fillRect(137 + 15 + 2, 159, 15, 23, BLACK);
    switch (second)
    {
    case 0:
      Lcd.drawPngFile(SPIFFS, "/0s.png", 137 + 15 + 2, 159);
      break;
    case 1:
      Lcd.drawPngFile(SPIFFS, "/1s.png", 137 + 15 + 2, 159);
      break;
    case 2:
      Lcd.drawPngFile(SPIFFS, "/2s.png", 137 + 15 + 2, 159);
      break;
    case 3:
      Lcd.drawPngFile(SPIFFS, "/3s.png", 137 + 15 + 2, 159);
      break;
    case 4:
      Lcd.drawPngFile(SPIFFS, "/4s.png", 137 + 15 + 2, 159);
      break;
    case 5:
      Lcd.drawPngFile(SPIFFS, "/5s.png", 137 + 15 + 2, 159);
      break;
    case 6:
      Lcd.drawPngFile(SPIFFS, "/6s.png", 137 + 15 + 2, 159);
      break;
    case 7:
      Lcd.drawPngFile(SPIFFS, "/7s.png", 137 + 15 + 2, 159);
      break;
    case 8:
      Lcd.drawPngFile(SPIFFS, "/8s.png", 137 + 15 + 2, 159);
      break;
    case 9:
      Lcd.drawPngFile(SPIFFS, "/9s.png", 137 + 15 + 2, 159);
      break;
    }
    last_32 = second;
  }
}

void setAllStrip(uint8_t r, uint8_t g, uint8_t b)
{
  for (int i = 0; i < STRIP_NUM; i++)
  {
    strip.set_RGB(i, r, g, b);
  }
}
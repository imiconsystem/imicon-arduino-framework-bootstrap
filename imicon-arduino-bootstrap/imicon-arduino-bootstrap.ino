// Arduino framework bootstrap
// Copyright (c) 2022, iMiconSystem.com
// Version 1.0.1
// Date: 2022-09-14
// Author: Somkid Sukchot (contact@imiconsystem.com)
//
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the MIT License
// which accompanies this distribution, and is available at
// https://opensource.org/licenses/mit-license.php
//

// ----------------------------------------------------------------------------------------
#define R1 13
#define R2 12
#define R3 15
#define R4 27

// กำหนดตัวแปรสำหรับการสื่อสารผ่านโปรโตคอล Modbus RTU
#define RXD2 16 // กำหนดหมายเลข GPIO 16 สำหรับขา Rx
#define TXD2 17 // กำหนดหมายเลข GPIO 17 สำหรับขา Tx

// Set these to your desired credentials.
const char *APssid = "iMicon-AP";
const char *APpassword = "dddddddd";

bool SETMODE = false;

// Set these to your desired credentials.
String WIFIssid = "ssid";
String WIFIpassword = "password";

const char *www_realm = "Custom Auth Realm";
const char *www_username = "wwwuser";
const char *www_password = "wwwpass";

// Max value
int32_t maxTemp, maxHumi, maxWindSpeed, maxWindDirec;

int32_t TEMP, HUMI, WINSPD, WINDIR;

void setup()
{
  Serial.begin(115200); // For debug

  //serialSetup();
  oledSetup();
  oledPrint("Start!", "");
  //loraSetup();
  relaySetup();
  resetbuttonSetup();
  storageSetup();

  if (resetbuttonispress()) // SET
  {

    SETMODE = true;
    Serial.println("SET MODE!");
    Serial.println();
    wifiapSetup();
  }
  else // RUN MODE
  {
    SETMODE = false;
    Serial.println("RUN MODE!");
    Serial.println("");
    //webserverSetup();
    oledPrint("Run MODE!", "");
  }
}

void controlRelay()
{

  relayHi(R1);
  relayHi(R2);
  relayHi(R3);
  relayHi(R4);
  delay(1000);
  relayLo(R1);
  relayLo(R2);
  relayLo(R3);
  relayLo(R4);
}

void loop()
{
  webserverLoop();
  if (!SETMODE)
  {
    //loraLoop();
    //serialLoop();
  }
  else
  {

    // Setmode loop
  }
}

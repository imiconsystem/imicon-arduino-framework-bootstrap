#include <LoRa.h> // นำเข้าไลบรารี่สำหรับการรับส่งข้อมูล LoRa radios ด้วยบอร์ด Esp32
// https://github.com/sandeepmistry/arduino-LoRa

// LoRa
#define ss 5
#define rst 14
#define dio0 2

void loraSetup()
{

    LoRa.setPins(ss, rst, dio0); // setup LoRa transceiver module

    while (!LoRa.begin(433E6)) // 433E6 - Asia, 866E6 - Europe, 915E6 - North America
    {
        Serial.println(".");
        delay(500);
    }
    LoRa.setSyncWord(0xA5); // A receiver using one syncword will not (mostly) receive packets from a transmitter using a different Syncword.
    Serial.println("LoRa Initializing OK!");
    
}

void loraLoop()
{
    // parse for a packet, and call onReceive with the result:
    onReceive(LoRa.parsePacket());
}



void onReceive(int packetSize)
{

  if (packetSize == 0)
    return;
  String incomingMsg = "";

  while (LoRa.available())
  {
    incomingMsg += (char)LoRa.read();
  }
  LoraProcessData(incomingMsg);
}

void LoraProcessData(String msg)
{
  Serial.println("LoRa Receive"); // 3:0.50:22.50

  int idx1 = msg.indexOf(":");     // 3[:]0.50:22.50 = 1
  int idx2 = msg.lastIndexOf(":"); // 3:0.50[:]22.50 = 6

  // Serial.print(idx1);
  // Serial.print("-");
  // Serial.print(idx2);

  String id = msg.substring(0, idx1);          // 3
  String sub1 = msg.substring(idx1 + 1, idx2); // 0.50
  String sub2 = msg.substring(idx2 + 1);       // 22.50

  int val1 = sub1.toInt();
  int val2 = sub2.toInt();

  // Serial.println();
  // Serial.print("ID ");
  // Serial.println(id);
  // Serial.print("Val1 ");
  // Serial.println(val1);
  // Serial.print("Val2 ");
  // Serial.println(val2);

  String _msg = "";
  _msg.concat("SI:");
  _msg.concat(LoRa.packetRssi());
  _msg.concat(" NR:");
  _msg.concat(LoRa.packetSnr());
  oledPrint(_msg,"");
  delay(500);

  String result = "";

  if (id == "1")
  {
    TEMP = round(val2 * 0.1);
    HUMI = round(val1 * 0.1);

    result.concat("T:");
    result.concat(TEMP);
    result.concat(" H:");
    result.concat(HUMI);
  }
  else if (id == "2")
  {
    WINSPD = round(val1 * 0.1);

    result.concat("SPEED:");
    result.concat(WINSPD);
  }
  else if (id == "3")
  {
    WINDIR = val1;

    result.concat("DIREC:");
    result.concat(WINDIR);
  }

  controlRelay();
  oledPrint(result,"");
}

const int numBytes = 8;
const int loopCount = 3;
const byte modbusQuery1[numBytes] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B}; // BGT-WSD2 humi & temp
const byte modbusQuery2[numBytes] = {0x02, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x38}; // RS FS wind speed
const byte modbusQuery3[numBytes] = {0x03, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC5, 0xE9}; // RS FX wind direction
byte buffQuery[numBytes];

byte ByteArray[9];
int ByteData[9];


void serialSetup(){
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); //
}



void serialLoop() {
  for(int i = 1 ; i <= loopCount ; i++){

    switch(i){
      case 1:
        Serial2.updateBaudRate(9600);
        memcpy(buffQuery, modbusQuery1, numBytes);
        break;
      case 2:
        Serial2.updateBaudRate(9600);
        memcpy(buffQuery, modbusQuery2, numBytes);
        break;
      case 3:
        Serial2.updateBaudRate(4800);
        memcpy(buffQuery, modbusQuery3, numBytes);
        break;
      default:
        break;
    }
    SerialProcessData(buffQuery);
  
    delay(8000);
  }
  
}

void SerialProcessData(byte buffQuery[8]){

  
  for (int j = 0; j < numBytes; j++)
    {
      Serial2.write(buffQuery[j]);
    }
    int a = 0;
    while(Serial2.available()>0){
      ByteArray[a] = Serial2.read();
      // Serial.print(ByteArray[a]);
      // Serial.print(" ");
      a++;
    }
    //Serial.println();
    
    ByteData[0] = ByteArray[3] * 256 + ByteArray[4];
    ByteData[1] = ByteArray[5] * 256 + ByteArray[6];

     float Data1, Data2;
     Data1 = ByteData[0] * 0.1;
     Data2 = ByteData[1] * 0.1;

    // Serial.print(ByteArray[0]);
    // Serial.print(" ");
    // Serial.print(Data1);
    // Serial.print(" ");
    // Serial.print(Data2);
    // Serial.print(" ");
    
    String result = "";
    
    result.concat(String(ByteArray[0]));
    result.concat(":");
    result.concat(ByteData[0]);
    result.concat(":");
    result.concat(ByteData[1]);

    
    if(ByteData[0] > 0 && ByteData[1] > 0) displayData(result);
}

void displayData(String msg)
{
//   Serial.println(msg); // 3:0.50:22.50

  int idx1 = msg.indexOf(":");     // 3[:]0.50:22.50 = 1
  int idx2 = msg.lastIndexOf(":"); // 3:0.50[:]22.50 = 6

//   Serial.print(idx1);
//   Serial.print("-");
//   Serial.print(idx2);

  String id = msg.substring(0, idx1);          // 3
  String sub1 = msg.substring(idx1 + 1, idx2); // 0.50
  String sub2 = msg.substring(idx2 + 1);       // 22.50

  int val1 = sub1.toInt();
  int val2 = sub2.toInt();

//   Serial.println();
//   Serial.print("ID ");
//   Serial.println(id);
//   Serial.print("Val1 ");
//   Serial.println(val1);
//   Serial.print("Val2 ");
//   Serial.println(val2);


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

    result.concat("SP:");
    result.concat(WINSPD);
  }
  else if (id == "3")
  {
    WINDIR = val1;

    result.concat("DI:");
    result.concat(WINDIR);
  }

 // displayMessage(result);
  controlRelay();
  oledPrint(result,"");
}

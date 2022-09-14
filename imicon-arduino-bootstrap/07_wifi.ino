#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <ESPmDNS.h>

// Set these to your desired credentials.
// const char *espssid = "E32x485";
// const char *esppassword = "dddddddd";
// WiFiServer wifiserver(80);

// Set these to your desired credentials.
// const char *ssid = "i2";
// const char *password = "pks5687013";
WebServer server(80);

// const char *www_realm = "Custom Auth Realm";
// const char *www_username = "imicon";
// const char *www_password = "0099";
// the Content of the HTML response in case of Unautherized Access Default:empty
String authFailResponse = "Authentication Failed";

void wifiapSetup()
{
    Serial.println("Configuring access point...");
    Serial.println("APssid");
    Serial.println(storageGet("APssid"));

    Serial.println("APpassword");
    Serial.println(storageGet("APpassword"));

    WiFi.softAP(storageGet("APssid").c_str(), storageGet("APpassword").c_str());

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    String dis = "";

    dis.concat(myIP.toString());
    dis.concat(" ID:");
    dis.concat(storageGet("APssid"));
    dis.concat(" PW:");
    dis.concat(storageGet("APpassword"));
    oledPrint(dis,"");


    server.on("/", configForm);
    server.on("/saveConfig", saveConfig);

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("Wifi Server started");
}

void webserverSetup()
{

    Serial.println("WIFIssid");
    Serial.println(storageGet("WIFIssid"));
    Serial.println("WIFIpassword");
    Serial.println(storageGet("WIFIpassword"));

    WiFi.mode(WIFI_STA);
    WiFi.begin(storageGet("WIFIssid").c_str(), storageGet("WIFIpassword").c_str());
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.print("Connected to ");
    Serial.println(WIFIssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("esp32")) // esp32.local
    {
        Serial.println("MDNS responder started");
    }

    server.on("/", handleRoot);
    //server.on("/setvalueform", hadleSetValueForm);
    //server.on("/setvalue", hadleSetValue);

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}
void webserverLoop()
{
    server.handleClient();
    delay(2); // allow the cpu to switch to other tasks
}
void handleRoot()
{

    char html[1200];
    int sec = millis() / 1000;
    int min = sec / 60;
    int hr = min / 60;
    String maxTempClass, maxHumiClass, maxWinSClass, maxWinDClass;
    if (TEMP > maxTemp)
        maxTempClass = "alert";
    else
        maxTempClass = "normal";

    if (HUMI > maxHumi)
        maxHumiClass = "alert";
    else
        maxHumiClass = "normal";

    if (WINSPD > maxWindSpeed)
        maxWinSClass = "alert";
    else
        maxWinSClass = "normal";

    if (WINDIR > maxWindDirec)
        maxWinDClass = "alert";
    else
        maxWinDClass = "normal";

    snprintf(html, 1200,

             "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP32 Weather Station IOT Development Kits</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
      .alert{color:red; font-weight:bold;}\
    </style>\
  </head>\
  <body>\
   <div style=\"width:500px;margin:auto;\">\
    <h1>ESP32 Weather Station IOT Development Kits - Receiver</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <div style=\"float:right;\">Max:%2d</div>\
    <h3 class=\"%s\">Temp: %2d<sup>&#xb0;C</sup></h3>\
    <hr>\
    <div style=\"float:right;\">Max:%2d</div>\
    <h3 class=\"%s\">Humi: %2d<sup>&percnt;</sup></h3>\
    <hr>\
    <div style=\"float:right;\">Max:%2d</div>\
    <h3 class=\"%s\">Wind Speed: %2d<sup>m/s</sup></h3>\
    <hr>\
    <div style=\"float:right;\">Max:%d</div>\
    <h3 class=\"%s\">Wind Direction: %d<sup>&#xb0;</sup></h3>\
    <hr>\
    </div>\
  </body>\
</html>",

             hr, min % 60, sec % 60, maxTemp, maxTempClass, TEMP, maxHumi, maxHumiClass, HUMI, maxWindSpeed, maxWinSClass, WINSPD, maxWindDirec, maxWinDClass, WINDIR);
    server.send(200, "text/html", html);
}

void handleNotFound()
{

    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void hadleSetValueForm()
{
    if (!server.authenticate(www_username, www_password))
    // Basic Auth Method with Custom realm and Failure Response
    // return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
    // Digest Auth Method with realm="Login Required" and empty Failure Response
    // return server.requestAuthentication(DIGEST_AUTH);
    // Digest Auth Method with Custom realm and empty Failure Response
    // return server.requestAuthentication(DIGEST_AUTH, www_realm);
    // Digest Auth Method with Custom realm and Failure Response
    {
        return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }

    char html[1200];
    snprintf(html, 1200,

             "<html>\
  <head>\
    <meta/>\
    <title>ESP32 Weather Station IOT Development Kits</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088;}\
    </style>\
  </head>\
  <body>\
  <div style=\"width:500px;margin:auto;\">\
  <form action=\"/setvalue\" method=\"POST\">\
  <fieldset>\
    <legend><h1>Set max values</h1></legend>\
    <div style=\"float:right;\"><label>Max </label><input type=\"number\" min=\"0\" name=\"temp\" value=\"%1d\" size=\"5\">&#xb0;C</div>\
    <h3>Temp:</h3>\
    <hr>\
    <div style=\"float:right;\"><label>Max </label><input type=\"number\" min=\"0\" name=\"humi\" value=\"%1d\" size=\"5\">&percnt;</div>\
    <h3>Humi:</h3>\
    <hr>\
    <div style=\"float:right;\"><label>Max </label><input type=\"number\" min=\"0\" name=\"speed\" value=\"%1d\" size=\"5\">m/s</div>\
    <h3>Wind Speed:</h3>\
    <hr>\
    <div style=\"float:right;\"><label>Max </label><input type=\"number\" min=\"0\" max=\"7\" name=\"dir\" value=\"%1d\" size=\"5\">&#xb0;(0-7)</div>\
    <h3>Wind Direction:</h3>\
    </fieldset>\
    <div style=\"float:right;\"><input type=\"submit\" value=\"SAVE\"></div>\
    </form>\
    <a href=\"http://192.168.4.1\">Back to monitor</a>\
    </div>\
  </body>\
</html>",

             maxTemp, maxHumi, maxWindSpeed, maxWindDirec);
    server.send(200, "text/html", html);
}

void hadleSetValue()
{

    if (!server.authenticate(www_username, www_password))
    // Basic Auth Method with Custom realm and Failure Response
    // return server.requestAuthentication(BASIC_AUTH, www_realm, authFailResponse);
    // Digest Auth Method with realm="Login Required" and empty Failure Response
    // return server.requestAuthentication(DIGEST_AUTH);
    // Digest Auth Method with Custom realm and empty Failure Response
    // return server.requestAuthentication(DIGEST_AUTH, www_realm);
    // Digest Auth Method with Custom realm and Failure Response
    {
        return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }

    char temp, humi, speed, dir;
    String message = "Data changed.<br>";
    // message += (server.method() == HTTP_GET) ? "GET" : "POST";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "<br>\n";
        if (server.argName(i) == "temp")
        {
            temp = server.arg(i).toInt();
        }
        else if (server.argName(i) == "humi")
        {
            humi = server.arg(i).toInt();
        }
        else if (server.argName(i) == "speed")
        {
            speed = server.arg(i).toInt();
        }
        else if (server.argName(i) == "dir")
        {
            dir = server.arg(i).toInt();
        }
    }
    message += "<html><head></head><body><script>alert('Saving data please wait a few seconds then click Back to main');</script><br><br><a href='http://192.168.4.1'>Back to main</a></body></html>";
    server.send(200, "text/html", message);

    storagePutInt("maxTemp", temp);
    storagePutInt("maxHumi", humi);
    storagePutInt("maxWindSpeed", speed);
    storagePutInt("maxWindDirec", dir);
    ESP.restart();
}

void configForm()
{

    char html[2500];
    snprintf(html, 2500,

             "<html>\
  <head>\
    <meta/>\
    <title>ESP32 WIFI configuration.</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088;}\
    </style>\
  </head>\
  <body>\
  <div style=\"width:500px;margin:auto;\">\
 <form action=\"/saveConfig\" method=\"POST\">\
    <fieldset>\
    <legend><h1>WiFi credential:</h1></legend>\
    <div style=\"float:right;\"><label> </label><input type=\"text\" name=\"APssid\" value=\"%s\" size=\"10\"></div>\
    <h3>SSID:</h3>\
    <hr>\
    <div style=\"float:right;\"><label> </label><input type=\"text\"  name=\"APpassword\" value=\"%s\" size=\"10\"></div>\
    <h3>PASSWORD:</h3>\
    <hr>\
    </fieldset>\
    <fieldset>\
    <legend><h1>Set max values</h1></legend>\
    <div style=\"float:right;\"><label>Max </label><input type=\"number\" min=\"0\" name=\"temp\" value=\"%d\" size=\"5\">&#xb0;C</div>\
    <h3>Temp:</h3>\
    <hr>\
    <div style=\"float:right;\"><label>Max </label><input type=\"number\" min=\"0\" name=\"humi\" value=\"%d\" size=\"5\">&percnt;</div>\
    <h3>Humi:</h3>\
    <hr>\
    <div style=\"float:right;\"><label>Max </label><input type=\"number\" min=\"0\" name=\"speed\" value=\"%d\" size=\"5\">m/s</div>\
    <h3>Wind Speed:</h3>\
    <hr>\
    <div style=\"float:right;\"><label>Max </label><input type=\"number\" min=\"0\" max=\"7\" name=\"dir\" value=\"%d\" size=\"5\">&#xb0;(0-7)</div>\
    <h3>Wind Direction:</h3>\
    </fieldset>\
    <div style=\"float:right;\"><input type=\"submit\" value=\"SAVE\"></div>\
    </form>\
    <a href=\"http://192.168.4.1\">Back to monitor</a>\
    </div>\
  </body>\
</html>",

    
    
    
             storageGet("APssid"), storageGet("APpassword"), maxTemp, maxHumi, maxWindSpeed, maxWindDirec);
    server.send(200, "text/html", html);
}

void saveConfig()
{

    char temp, humi, speed, dir;
    String message = "Data changed.<br>";
    String WIFIssid, WIFIpassword, APssid, APpassword;
    // message += (server.method() == HTTP_GET) ? "GET" : "POST";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "<br>\n";
        if (server.argName(i) == "APssid")
        {
            APssid = server.arg(i);
        }
        else if (server.argName(i) == "APpassword")
        {
            APpassword = server.arg(i);
        }
        else if (server.argName(i) == "temp")
        {
            temp = server.arg(i).toInt();
        }
        else if (server.argName(i) == "humi")
        {
            humi = server.arg(i).toInt();
        }
        else if (server.argName(i) == "speed")
        {
            speed = server.arg(i).toInt();
        }
        else if (server.argName(i) == "dir")
        {
            dir = server.arg(i).toInt();
        }
    }
    message += "<html><head></head><body><script>alert('Saving data please wait a few seconds then click Back to main');</script><br><br><a href='http://192.168.4.1'>Back to main</a></body></html>";
    server.send(200, "text/html", message);

    // storagePut("WIFIssid", WIFIssid);
    // storagePut("WIFIpassword", WIFIpassword);
    storagePut("APssid", APssid);
    storagePut("APpassword", APpassword);
    storagePutInt("maxTemp", temp);
    storagePutInt("maxHumi", humi);
    storagePutInt("maxWindSpeed", speed);
    storagePutInt("maxWindDirec", dir);
    ESP.restart();
}
#include <Preferences.h>
#include <nvs_flash.h>

Preferences preferences;
const char *prekey = "APcredentials";

void storageSetup()
{
    String WIssid, WIpass, Assid, Apass, www_realm, www_username,www_password;
    preferences.begin(prekey, false);

    maxTemp = preferences.getInt("maxTemp", 0);
    maxHumi = preferences.getInt("maxHumi", 0);
    maxWindSpeed = preferences.getInt("maxWindSpeed", 0);
    maxWindDirec = preferences.getInt("maxWindDirec", 0);
    Serial.println(preferences.getInt("maxTemp", 0));
    Serial.println(preferences.getInt("maxHumi", 0));
    Serial.println(preferences.getInt("maxWindSpeed", 0));
    Serial.println(preferences.getInt("maxWindDirec", 0));

    WIssid = preferences.getString("WIFIssid", "");
    WIpass = preferences.getString("WIFIpassword", "");

    Assid = preferences.getString("APssid", "");
    Apass = preferences.getString("APpassword", "");

    www_realm = preferences.getString("www_realm", "");
    www_username = preferences.getString("www_username", "");
    www_password = preferences.getString("www_password", "");

    if (Assid == "" || Apass == "")
    {
        preferences.putString("APssid", APssid);
        preferences.putString("APpassword", APpassword);
        Serial.println("No values saved for APssid or APpassword");
        Serial.println("AP Credentials Saved using default value.");
    }

    if (WIssid == "" || WIpass == "")
    {
        preferences.putString("WIFIssid", WIFIssid);
        preferences.putString("WIFIpassword", WIFIpassword);
        Serial.println("No values saved for WIFssid or WIFIpassword");
        Serial.println("WIFI Credentials Saved using default value.");
        
    }

    // if (www_realm == "")
    // {
    //     preferences.putString("www_realm", www_realm);
    //     Serial.println("No values saved for www_realm");
    //     Serial.println("Saved using default value.");
        
    // }

    // if (www_username == "")
    // {
    //     preferences.putString("www_username", www_username);
    //     Serial.println("No values saved for www_username");
    //     Serial.println("Saved using default value.");
        
    // }

    // if (www_password == "")
    // {
    //     preferences.putString("www_password", www_password);
    //     Serial.println("No values saved for www_password");
    //     Serial.println("Saved using default value.");
        
    // }


    

    preferences.end();
}

void storagePut(const char *key, String value)
{
    preferences.begin(prekey, false);
    preferences.putString(key, value);
    preferences.end();
}

void storagePutInt(const char *key, int32_t value)
{
    preferences.begin(prekey, false);
    preferences.putInt(key, value);
    preferences.end();
}

String storageGet(const char *key)
{
    String result;
    preferences.begin(prekey, false);
    result = preferences.getString(key, "");
    preferences.end();
    return result;
}

String storageGetInt(const char *key)
{
    String result;
    preferences.begin(prekey, false);
    result = preferences.getInt(key, 0);
    preferences.end();
    return result;
}

void storageClear()
{
    nvs_flash_erase(); // erase the NVS partition and...
    nvs_flash_init();  // initialize the NVS partition.
    while (true)
        ;
}

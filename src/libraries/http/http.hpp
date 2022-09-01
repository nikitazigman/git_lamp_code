
#ifndef _HTTP_H
#define _HTTP_H

#include <Thread.h>

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>

ESP8266WiFiMulti WiFiMulti;
std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
HTTPClient https;

class HttpThread : public Thread
{
private:
    // ESP8266WiFiMulti WiFiMulti;
    // HTTPClient https;
    const char *ssid;
    const char *password;

public:
    HttpThread(const char *ssid, const char *passwrod) : Thread()
    {
        this->ssid = ssid;
        this->password = passwrod;
    }
    void init(void)
    {
        WiFi.mode(WIFI_STA);
        WiFiMulti.addAP(this->ssid, this->password);

        while (WiFiMulti.run() != WL_CONNECTED)
        {
            delay(1000);
        }
        client->setInsecure();

        Serial.println("WiFi is connected");
    }

    void run()
    {
        static int httpCode;
        static String payload;

        https.begin(*client, "https://reqbin.com/echo/get/json");

        httpCode = https.GET();

        if (httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            // file found at server
            if (httpCode == HTTP_CODE_OK)
            {
                payload = https.getString();
                Serial.println(payload);
            }
        }
        else
        {
            Serial.printf("[HTTP] GET... failed, error");
        }

        this->runned();
    }
};

#endif // _HTTP_H
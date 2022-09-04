
#ifndef _HTTP_H
#define _HTTP_H

#include <Thread.h>

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>

#include <ArduinoJson.h>

ESP8266WiFiMulti WiFiMulti;
std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
HTTPClient https;
StaticJsonDocument<2200> doc;

class HttpThread : public Thread
{
private:
    // ESP8266WiFiMulti WiFiMulti;
    // HTTPClient https;
    const char **pull;
    const char *auth_value;
    const char *ssid;
    const char *password;
    const char *auth_key = "Authorization";
    const char *accept_key = "Accept";
    const char *accept_value = "application/vnd.github.v3+json";
    const char *url_pattern = "https://api.github.com/repos/%s/events?per_page=1";
    char url_buf[100];
    int pull_size;
    uint64_t id = 0;
    bool *event;

public:
    HttpThread(const char *ssid, const char *passwrod, const char *auth_value, const char **pull, int pull_size, bool *event) : Thread()
    {
        this->ssid = ssid;
        this->password = passwrod;
        this->auth_value = auth_value;
        this->pull = pull;
        this->pull_size = pull_size;
        this->event = event;
    }
    void print_settings()
    {
        Serial.printf("wifi ssid: %s\n", this->ssid);
        Serial.printf("wifi password: %s\n", this->password);
        Serial.printf("auth_value: %s\n", this->auth_value);
        Serial.printf("pull_size: %d\n", this->pull_size);
        for (int i = 0; i < pull_size; i++)
        {
            Serial.printf("\trepo %d: %s\n", i, this->pull[i]);
        }
    }
    void init(void)
    {
        this->print_settings();

        WiFi.mode(WIFI_STA);
        WiFiMulti.addAP(this->ssid, this->password);

        while (WiFiMulti.run() != WL_CONNECTED)
        {
            delay(1000);
        }
        delay(1000);

        client->setInsecure();
        Serial.println("WiFi is connected");
    }

    void request_repo(const char *repo)
    {
        static int httpCode;

        sprintf(this->url_buf, this->url_pattern, repo);

        Serial.printf("request repo: %s\n", this->url_buf);
        Serial.printf("header: %s:%s\n", this->accept_key, this->accept_value);
        Serial.printf("header: %s:%s\n", this->auth_key, this->auth_value);

        https.begin(*client, this->url_buf);
        https.addHeader(this->accept_key, this->accept_value);
        https.addHeader(this->auth_key, this->auth_value);

        httpCode = https.GET();

        if (httpCode > 0)
        {
            if (httpCode == HTTP_CODE_OK)
            {
                DeserializationError error = deserializeJson(doc, https.getString());
                if (error)
                {
                    Serial.println("serialization error");
                    return;
                }
                String type = doc[0]["type"];
                if (type == "PushEvent")
                {
                    uint64_t current_id = doc[0]["id"];
                    if (current_id != this->id)
                    {
                        Serial.println("EVENT!");
                        *this->event = true;
                        this->id = current_id;
                    }
                }
            }
        }
        else
        {
            Serial.printf("[HTTP] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
    }

    void run()
    {
        for (int i = 0; i < pull_size; i++)
        {
            this->request_repo(this->pull[i]);
        }

        this->runned();
    }
};

#endif // _HTTP_H
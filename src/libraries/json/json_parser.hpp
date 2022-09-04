#ifndef _JSON_PARSER
#define _JSON_PARSER

#include "JsonListener.h"
#define ID_PULL_SIZE 100

class ExampleListener : public JsonListener
{
private:
    String id_pull[ID_PULL_SIZE];
    int deep = 0;
    String current_key;
    String current_id;
    bool event = false;

public:
    void key(String key)
    {
        this->current_key = key;
    }
    bool is_new_id(String id)
    {
        for (int i = 0; i < ID_PULL_SIZE; i++)
        {
            if (id == this->id_pull[i])
            {
                return false;
            }
        }

        return true;
    }
    void add_id(String id)
    {
        // shift buf
        for (int i = ID_PULL_SIZE - 1; i >= 0; i--)
        {
            this->id_pull[i] = this->id_pull[i + 1];
        }
        this->id_pull[0] = id;
    }
    void value(String value)
    {
        if (this->deep != 1)
            return;

        if (this->current_key == "id")
            this->current_id = value;

        if (this->current_key == "type")
        {
            if (this->current_id.length())
            {
                Serial.println("id is empty");
                return;
            }

            if (value == "PushEvent")
                if (this->is_new_id(this->current_id))
                {
                    this->add_id(this->current_id);
                    this->event = true;
                }
        }
    }
    bool get_event()
    {
        bool event = this->event;
        this->event = false;
        return event;
    }

    virtual void endObject()
    {
        this->deep--;
    }

    virtual void startObject()
    {
        this->deep++;
    }
};

#endif // _JSON_PARSER
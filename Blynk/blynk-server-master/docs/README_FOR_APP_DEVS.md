# Protocol messages

Every message consists of 2 parts.

+ Header :
    + Protocol command (1 byte);
    + MessageId (2 bytes);
    + Body message length (2 bytes);

+ Body : string (could be up to 2^15 bytes).

Blynk transfers binary messages with the following structure:

| Command       | Message Id    | Length/Status   | Body     |
|:-------------:|:-------------:|:---------------:|:--------:|
| 1 byte        | 2 bytes       | 2 bytes         | Variable |

So, message is always "1 byte + 2 bytes + 2 bytes + messageBody.length".

### Command field
Unsigned byte.
This is 1 byte field responsible for storing command code from client, like login, ping, etc. Codes:

    RESPONSE = 0;  
    REGISTER = 1;              
    LOGIN = 2;                  
    GET_TOKEN = 5; 
    PING = 6;      
    ACTIVATE_DASHBOARD = 7;
    DEACTIVATE_DASHBOARD = 8;
    REFRESH_TOKEN = 9;
    HARDWARE = 20;
    
    CREATE_DASH = 21;
    SAVE_DASH = 22;
    DELETE_DASH = 23;
    
    LOAD_PROFILE_GZIPPED = 24;

    CREATE_WIDGET = 33;
    UPDATE_WIDGET = 34;
    DELETE_WIDGET = 35;

[Full list](https://github.com/blynkkk/blynk-server/blob/master/server/core/src/main/java/cc/blynk/server/core/protocol/enums/Command.java#L11) 

### Message Id field
Unsigned short.
Message Id field is a 2 bytes field for defining unique message identifier. It’s used in order to distinguish 
how to manage responses from hardware on mobile client. Message ID field should be generated on client’s side.

### Length field
Unsigned short.
Length field is a 2 bytes field for defining body length. Could be 0 if body is empty.

## Response Codes

    OK = 200;
    QUOTA_LIMIT_EXCEPTION = 1;
    ILLEGAL_COMMAND = 2;
    USER_NOT_REGISTERED = 3;
    USER_ALREADY_REGISTERED = 4;
    USER_NOT_AUTHENTICATED = 5;
    NOT_ALLOWED = 6;
    DEVICE_NOT_IN_NETWORK = 7;
    NO_ACTIVE_DASHBOARD = 8;
    INVALID_TOKEN = 9;
    ILLEGAL_COMMAND_BODY = 11;
    GET_GRAPH_DATA_EXCEPTION = 12;
    
    NOTIFICATION_INVALID_BODY_EXCEPTION = 13;
    NOTIFICATION_NOT_AUTHORIZED_EXCEPTION = 14;
    NOTIFICATION_EXCEPTION = 15;
    
    //reserved
     BLYNK_TIMEOUT_EXCEPTION = 16;
     
    NO_DATA_EXCEPTION = 17;
    DEVICE_WENT_OFFLINE = 18;
    SERVER_EXCEPTION = 19;
    NOT_SUPPORTED_VERSION = 20;

Client sends commands to the server and gets response for every command sent.
For commands (register, login, saveProfile, hardware) that doesn't request any data back - 'response' (command field 0x00) message is returned.
For commands (loadProfile, getToken) that request data back - message will be returned with same command code. In case you sent 'loadProfile' you will receive 'loadProfile' command back with filled body.

[Here is the class with all of the codes](https://github.com/blynkkk/blynk-server/blob/master/server/core/src/main/java/cc/blynk/server/core/protocol/enums/Response.java#L12).
Response message structure:

| Command       | Message Id    | Response code   |
|:-------------:|:-------------:|:---------------:|
| 1 byte        | 2 bytes       | 2 bytes         |


## Widget types

    //controls
    BUTTON,
    SLIDER,
    VERTICAL_SLIDER,
    KNOB,
    TIMER,
    ROTARY_KNOB,
    RGB,
    TWO_WAY_ARROW,
    FOUR_WAY_ARROW,
    ONE_AXIS_JOYSTICK,
    TWO_AXIS_JOYSTICK,
    GAMEPAD,
    KEYPAD,

    //outputs
    LED,
    LOGGER, //history_graph
    DIGIT4_DISPLAY, //same as NUMERICAL_DISPLAY
    GAUGE,
    LCD_DISPLAY,
    GRAPH,
    LEVEL_DISPLAY,
    TERMINAL,

    //inputs
    MICROPHONE,
    GYROSCOPE,
    ACCELEROMETER,
    GPS,

    //notifications
    TWITTER,
    EMAIL,
    NOTIFICATION,

    //other
    SD_CARD,
    EVENTOR,
    RCT,
    BRIDGE,
    BLUETOOTH,

    //UI
    MENU


[List is here](https://github.com/blynkkk/blynk-server/blob/master/server/core/src/main/java/cc/blynk/server/core/model/enums/WidgetType.java#L8).

## JSON structure

    {
        "dashBoards" :
            [
                {
                 "id":1,
                 "name":"My Dashboard",
                 "isActive" : true,
                 "isShared" : true,
                 "widgets"  : [
                    {"id":1, "x":1, "y":1, "label":"Some Text", "type":"BUTTON",         "pinType":"DIGITAL", "pin":1, "value":"1"},
                    {"id":2, "x":1, "y":1, "label":"Some Text", "type":"SLIDER",  "pinType":"DIGITAL", "pin":2, "value":"1", "state":"ON"},
                    {"id":3, "x":1, "y":1, "label":"Some Text", "type":"SLIDER",  "pinType":"ANALOG", "pin":3, "value":"0", "state":"OFF"},
                    {"id":4, "x":1, "y":1, "label":"Some Text", "type":"SLIDER",         "pinType":"VIRTUAL", "pin":4, "value":"244" },
                    {"id":5, "x":1, "y":1, "label":"Some Text", "type":"TIMER",          "pinType":"DIGITAL", "pin":5, "value":"1", "startTime":0},
                    {"id":6, "x":1, "y":1, "label":"Some Text", "type":"LED",            "pinType":"ANALOG", "pin":6, "frequency" : 100},
                    {"id":7, "x":1, "y":1, "label":"Some Text", "type":"DIGIT4_DISPLAY", "pinType":"ANALOG", "pin":7, "frequency" : 1000},
                    {"id":8, "x":1, "y":1, "label":"Some Text", "type":"GRAPH",          "pinType":"ANALOG", "pin":8},
                    {"id":9, "x":1, "y":1, "type":"NOTIFICATION", "notifyWhenOffline":true, "androidTokens":{"uid":"token"}},
                    {"id":10, "x":1, "y":1, "token":"token", "secret":"secret", "type":"TWITTER"},
                    {"id":11, "x":1, "y":1, "type":"RTC", "pinType":"VIRTUAL", "pin":9},
                    {"id":12, "x":0, "y":0, "color":-1, "width":8, "height":2,
                        "type":"LCD",
                        "pins": [
                                    {
                                        "pin":0,
                                        "pwmMode":false,
                                        "rangeMappingOn":false,
                                        "pinType":"VIRTUAL",
                                        "value":"89.888037459418",
                                        "min":-100,
                                        "max":100
                                    },
                                    {   "pin":1,
                                        "pwmMode":false,
                                        "rangeMappingOn":false,
                                        "pinType":"VIRTUAL",
                                        "value":"-58.74774244674501",
                                        "min":-100,
                                        "max":100
                                    }
                                ],
                        "advancedMode":false,
                        "textFormatLine1":"pin1 : /pin0/",
                        "textFormatLine2":"pin2 : /pin1/",
                        "textLight":false,
                        "frequency":1000
                    }
                 ],
                 "boardType":"UNO"
                }
            ]
    }
    
## Hardware command description
    
Could be found [here](https://github.com/blynkkk/blynk-library/blob/master/docs/Implementing.md#hardwarebridge-command-body).
    
## Command workflow

For better understanding of how commands should be processed, please have a look in integration test 
it is easy understand what is going on there. You may start from [this](https://github.com/blynkkk/blynk-server/blob/master/integration-tests/src/test/java/cc/blynk/integration/tcp/MainWorkflowTest.java#L78) test.
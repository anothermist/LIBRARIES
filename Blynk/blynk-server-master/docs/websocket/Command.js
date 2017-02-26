var MsgType = {
    RESPONSE      :  0,
    LOGIN         :  2,
    PING          :  6,
    TWEET         :  12,
    EMAIL         :  13,
    NOTIFY        :  14,
    BRIDGE        :  15,
    HW_SYNC       :  16,
    HW_INFO       :  17,
    HARDWARE      :  20
};

var MsgStatus = {
    OK                    :  200,
    ILLEGAL_COMMAND       :  2,
    NO_ACTIVE_DASHBOARD   :  8,
    INVALID_TOKEN         :  9,
    ILLEGAL_COMMAND_BODY  : 11
};

function getCommandByString(cmdString) {
    switch (cmdString) {
        case "ping" :
            return MsgType.PING;
        case "login" :
            return MsgType.LOGIN;
        case "hardware" :
            return MsgType.HARDWARE;
    }
}

function getStringByCommandCode(cmd) {
    switch (cmd) {
        case 0 :
            return "RESPONSE";
        case 20 :
            return "HARDWARE";
    }
}

function getStatusByCode(statusCode) {
    switch (statusCode) {
        case 200 :
            return "OK";
        case 2 :
            return "ILLEGAL_COMMAND";
        case 8 :
            return "NO_ACTIVE_DASHBOARD";
        case 9 :
            return "INVALID_TOKEN";
        case 11 :
            return "ILLEGAL_COMMAND_BODY";
    }
}
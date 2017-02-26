package cc.blynk.server.core.model.widgets;

import cc.blynk.server.core.model.Pin;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.controls.*;
import cc.blynk.server.core.model.widgets.notifications.Mail;
import cc.blynk.server.core.model.widgets.notifications.Notification;
import cc.blynk.server.core.model.widgets.notifications.SMS;
import cc.blynk.server.core.model.widgets.notifications.Twitter;
import cc.blynk.server.core.model.widgets.others.*;
import cc.blynk.server.core.model.widgets.others.eventor.Eventor;
import cc.blynk.server.core.model.widgets.others.rtc.RTC;
import cc.blynk.server.core.model.widgets.others.webhook.WebHook;
import cc.blynk.server.core.model.widgets.outputs.*;
import cc.blynk.server.core.model.widgets.sensors.*;
import cc.blynk.server.core.model.widgets.ui.*;
import cc.blynk.server.core.model.widgets.ui.table.Table;
import cc.blynk.utils.ByteUtils;
import cc.blynk.utils.StringUtils;
import com.fasterxml.jackson.annotation.JsonSubTypes;
import com.fasterxml.jackson.annotation.JsonTypeInfo;

/**
 * User: ddumanskiy
 * Date: 21.11.13
 * Time: 13:08
 */
@JsonTypeInfo(
        use = JsonTypeInfo.Id.NAME,
        include = JsonTypeInfo.As.PROPERTY,
        property = "type")
@JsonSubTypes({

        //controls
        @JsonSubTypes.Type(value = Button.class, name = "BUTTON"),
        @JsonSubTypes.Type(value = FieldInput.class, name = "FIELD_INPUT"),
        @JsonSubTypes.Type(value = Slider.class, name = "SLIDER"),
        @JsonSubTypes.Type(value = VerticalSlider.class, name = "VERTICAL_SLIDER"),
        @JsonSubTypes.Type(value = RGB.class, name = "RGB"),
        @JsonSubTypes.Type(value = Timer.class, name = "TIMER"),
        @JsonSubTypes.Type(value = OneAxisJoystick.class, name = "ONE_AXIS_JOYSTICK"),
        @JsonSubTypes.Type(value = TwoAxisJoystick.class, name = "TWO_AXIS_JOYSTICK"),
        @JsonSubTypes.Type(value = Terminal.class, name = "TERMINAL"),
        @JsonSubTypes.Type(value = Step.class, name = "STEP"),
        @JsonSubTypes.Type(value = VerticalStep.class, name = "VERTICAL_STEP"),
        @JsonSubTypes.Type(value = QR.class, name = "QR"),
        @JsonSubTypes.Type(value = TimeInput.class, name = "TIME_INPUT"),

        //outputs
        @JsonSubTypes.Type(value = LED.class, name = "LED"),
        @JsonSubTypes.Type(value = ValueDisplay.class, name = "DIGIT4_DISPLAY"),
        @JsonSubTypes.Type(value = LabeledValueDisplay.class, name = "LABELED_VALUE_DISPLAY"),
        @JsonSubTypes.Type(value = Gauge.class, name = "GAUGE"),
        @JsonSubTypes.Type(value = LCD.class, name = "LCD"),
        @JsonSubTypes.Type(value = Graph.class, name = "GRAPH"),
        @JsonSubTypes.Type(value = LevelDisplay.class, name = "LEVEL_DISPLAY"),
        @JsonSubTypes.Type(value = VerticalLevelDisplay.class, name = "VERTICAL_LEVEL_DISPLAY"),
        @JsonSubTypes.Type(value = Video.class, name = "VIDEO"),
        @JsonSubTypes.Type(value = HistoryGraph.class, name = "LOGGER"),

        //sensors
        @JsonSubTypes.Type(value = GPSTrigger.class, name = "GPS_TRIGGER"),
        @JsonSubTypes.Type(value = GPSStreaming.class, name = "GPS_STREAMING"),
        @JsonSubTypes.Type(value = Light.class, name = "LIGHT"),
        @JsonSubTypes.Type(value = Proximity.class, name = "PROXIMITY"),
        @JsonSubTypes.Type(value = Temperature.class, name = "TEMPERATURE"),
        @JsonSubTypes.Type(value = Accelerometer.class, name = "ACCELEROMETER"),
        @JsonSubTypes.Type(value = Gravity.class, name = "GRAVITY"),
        @JsonSubTypes.Type(value = Barometer.class, name = "BAROMETER"),
        @JsonSubTypes.Type(value = Humidity.class, name = "HUMIDITY"),

        //notifications
        @JsonSubTypes.Type(value = Twitter.class, name = "TWITTER"),
        @JsonSubTypes.Type(value = Mail.class, name = "EMAIL"),
        @JsonSubTypes.Type(value = Notification.class, name = "NOTIFICATION"),
        @JsonSubTypes.Type(value = SMS.class, name = "SMS"),

        //interface
        @JsonSubTypes.Type(value = Menu.class, name = "MENU"),
        @JsonSubTypes.Type(value = Tabs.class, name = "TABS"),
        @JsonSubTypes.Type(value = Player.class, name = "PLAYER"),
        @JsonSubTypes.Type(value = Table.class, name = "TABLE"),
        @JsonSubTypes.Type(value = Image.class, name = "IMAGE"),

        //others
        @JsonSubTypes.Type(value = RTC.class, name = "RTC"),
        @JsonSubTypes.Type(value = Bridge.class, name = "BRIDGE"),
        @JsonSubTypes.Type(value = Bluetooth.class, name = "BLUETOOTH"),
        @JsonSubTypes.Type(value = BluetoothSerial.class, name = "BLUETOOTH_SERIAL"),
        @JsonSubTypes.Type(value = Eventor.class, name = "EVENTOR"),
        @JsonSubTypes.Type(value = Map.class, name = "MAP"),
        @JsonSubTypes.Type(value = DeviceSelector.class, name = "DEVICE_SELECTOR"),

        @JsonSubTypes.Type(value = WebHook.class, name = "WEBHOOK")

})
public abstract class Widget {

    public long id;

    public int x;

    public int y;

    public volatile int color;

    public int width;

    public int height;

    public int tabId = 0;

    public volatile String label;

    protected static void append(StringBuilder sb, byte pin, PinType pinType, String pinMode) {
        if (pin == Pin.NO_PIN || pinMode == null || pinType == PinType.VIRTUAL) {
            return;
        }
        sb.append(StringUtils.BODY_SEPARATOR)
                .append(pin)
                .append(StringUtils.BODY_SEPARATOR)
                .append(pinMode);
    }

    public abstract boolean updateIfSame(int deviceId, byte pin, PinType type, String value);

    public abstract boolean isSame(int deviceId, byte pin, PinType type);

    public abstract String getJsonValue();

    public abstract String getModeType();

    public abstract String getValue(byte pin, PinType type);

    public abstract boolean hasValue(String searchValue);

    public abstract int getPrice();

    public abstract void append(StringBuilder sb, int deviceId);

    public void setProperty(String property, String propertyValue) {
        switch (property) {
            case "label" :
                this.label = propertyValue;
                break;
            case "color" :
                this.color = ByteUtils.parseColor(propertyValue);
                break;
            default:
                throw new RuntimeException("Error setting widget property.");
        }
    }
}

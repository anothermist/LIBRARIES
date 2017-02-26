package cc.blynk.server.core.model;

import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.controls.Button;
import cc.blynk.server.core.model.widgets.controls.RGB;
import cc.blynk.utils.JsonParser;
import cc.blynk.utils.StringUtils;
import com.fasterxml.jackson.databind.ObjectReader;
import org.apache.commons.lang3.ArrayUtils;
import org.junit.Test;

import java.io.InputStream;

import static org.junit.Assert.assertEquals;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 02.11.15.
 */
public class PinValuesUpdateCorrectTest {

    private static final ObjectReader profileReader = JsonParser.init().readerFor(Profile.class);

    public static Profile parseProfile(InputStream reader) {
        try {
            return profileReader.readValue(reader);
        } catch (Exception e) {
            throw new RuntimeException("Error parsing profile");
        }
    }

    @Test
    public void testHas1Pin() {
        InputStream is = this.getClass().getResourceAsStream("/json_test/user_profile_json.txt");

        Profile profile = parseProfile(is);
        DashBoard dash = profile.dashBoards[0];
        dash.isActive = true;

        Button button = dash.getWidgetByType(Button.class);
        assertEquals(1, button.pin);
        assertEquals(PinType.DIGITAL, button.pinType);
        assertEquals("1", button.value);

        dash.update(0, "dw 1 0".replaceAll(" ", StringUtils.BODY_SEPARATOR_STRING));
        assertEquals("0", button.value);

        dash.update(0, "aw 1 1".replaceAll(" ", StringUtils.BODY_SEPARATOR_STRING));
        assertEquals("0", button.value);

        dash.update(0, "dw 1 1".replaceAll(" ", StringUtils.BODY_SEPARATOR_STRING));
        assertEquals("1", button.value);

        RGB rgb = new RGB();
        rgb.pins = new Pin[3];
        rgb.pins[0] = new Pin();
        rgb.pins[0].pin = 0;
        rgb.pins[0].pinType = PinType.VIRTUAL;
        rgb.pins[1] = new Pin();
        rgb.pins[1].pin = 1;
        rgb.pins[1].pinType = PinType.VIRTUAL;
        rgb.pins[2] = new Pin();
        rgb.pins[2].pin = 2;
        rgb.pins[2].pinType = PinType.VIRTUAL;


        dash.widgets = ArrayUtils.add(dash.widgets, rgb);

        dash.update(0, "vw 0 100".replaceAll(" ", StringUtils.BODY_SEPARATOR_STRING));
        dash.update(0, "vw 1 101".replaceAll(" ", StringUtils.BODY_SEPARATOR_STRING));
        dash.update(0, "vw 2 102".replaceAll(" ", StringUtils.BODY_SEPARATOR_STRING));

        for (int i = 0; i < rgb.pins.length; i++) {
            assertEquals("10" + i, rgb.pins[i].value);
        }

        rgb = new RGB();
        rgb.pins = new Pin[3];
        rgb.pins[0] = new Pin();
        rgb.pins[0].pin = 4;
        rgb.pins[0].pinType = PinType.VIRTUAL;
        rgb.pins[1] = new Pin();
        rgb.pins[1].pin = 4;
        rgb.pins[1].pinType = PinType.VIRTUAL;
        rgb.pins[2] = new Pin();
        rgb.pins[2].pin = 4;
        rgb.pins[2].pinType = PinType.VIRTUAL;

        dash.widgets = ArrayUtils.add(dash.widgets, rgb);

        dash.update(0, "vw 4 100 101 102".replaceAll(" ", StringUtils.BODY_SEPARATOR_STRING));

        assertEquals("100 101 102".replaceAll(" ", StringUtils.BODY_SEPARATOR_STRING), rgb.pins[0].value);
    }


}

package cc.blynk.server.api.http.pojo.business;

import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.OnePinWidget;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.04.16.
 */
public class BusinessProject {

    public int id;

    public String name;

    public BusinessPin[] pins;

    public BusinessProject(DashBoard dashBoard) {
        this.id = dashBoard.id;
        this.name = dashBoard.name;
        this.pins = new BusinessPin[dashBoard.widgets.length];

        for (int i = 0; i < dashBoard.widgets.length; i++) {
            //for now working only with one pin widget.
            if (dashBoard.widgets[i] instanceof OnePinWidget) {
                this.pins[i] = new BusinessPin((OnePinWidget) dashBoard.widgets[i]);
            }
        }
    }

    private static class BusinessPin {
        public String value;
        public byte pin;
        public PinType pinType;

        public BusinessPin(OnePinWidget widget) {
            this.pin = widget.pin;
            this.pinType = widget.pinType;
            this.value = widget.value;
        }
    }
}

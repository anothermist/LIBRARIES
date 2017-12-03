#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0x8410

void setup()
{
    Serial.begin(9600);
    if (!Serial) delay(5000);           //allow some time for Leonardo
    uint16_t ID = tft.readID(); //
    Serial.println("Diagnose whether this controller is supported");
    Serial.println("There are FAQs in extras/mcufriend_how_to.txt");
    Serial.println("");
    Serial.print("tft.readID() finds: ID = 0x");
    Serial.println(ID, HEX);
    Serial.println("");
    if (ID == 0x0404) {
        Serial.println("Probably a write-only Mega2560 Shield");
        Serial.println("#define USE_SPECIAL in mcufriend_shield.h");
        Serial.println("#define appropriate SPECIAL in mcufriend_special.h");
        Serial.println("e.g. USE_MEGA_16BIT_SHIELD");
        Serial.println("e.g. USE_MEGA_8BIT_SHIELD");
        Serial.println("Hint.  A Mega2560 Shield has a 18x2 male header");
        Serial.println("Often a row of resistor-packs near the 18x2");
        Serial.println("RP1-RP7 implies 16-bit but it might be 8-bit");
        Serial.println("RP1-RP4 or RP1-RP5 can only be 8-bit");
    }
    if (ID == 0xD3D3) {
        uint16_t guess_ID = 0x9481; // write-only shield
        Serial.println("Probably a write-only Mega2560 Shield");
        Serial.print("Try to force ID = 0x");
        Serial.println(guess_ID, HEX);
        tft.begin(guess_ID);
    }
    else tft.begin(ID);
    Serial.println("");
    if (tft.width() == 0) {
        Serial.println("This ID is not supported");
        Serial.println("look up ID in extras/mcufriend_how_to.txt");
        Serial.println("you may need to edit MCUFRIEND_kbv.cpp");
        Serial.println("to enable support for this ID");
        Serial.println("e.g. #define SUPPORT_8347D");
        Serial.println("");
        Serial.println("New controllers appear on Ebay often");
        Serial.println("If your ID is not supported");
        Serial.println("run LCD_ID_readreg.ino from examples/");
        Serial.println("Copy-Paste the output from the Serial Terminal");
        Serial.println("to a message in Displays topic on Arduino Forum");
        Serial.println("or to Issues on GitHub");
        while (1);    //just die
    } else {
        Serial.print("PORTRAIT is ");
        Serial.print(tft.width());
        Serial.print(" x ");
        Serial.println(tft.height());
        Serial.println("");
        Serial.println("Run the examples/graphictest_kbv sketch");
        Serial.println("All colours, text, directions, rotations, scrolls");
        Serial.println("should work.  If there is a problem,  make notes on paper");
        Serial.println("Post accurate description of problem to Forum");
        Serial.println("Or post a link to a video (or photos)");
        Serial.println("");
        Serial.println("I rely on good information from remote users");
    }
}

void loop()
{
    static uint8_t aspect = 0;
    const char *aspectname[] = {
        "PORTRAIT", "LANDSCAPE", "PORTRAIT_REV", "LANDSCAPE_REV"
    };
    const char *colorname[] = { "BLUE", "GREEN", "RED", "GRAY" };
    uint16_t colormask[] = { BLUE, GREEN, RED, GRAY };
    uint16_t ID = tft.readID(); //
    tft.setRotation(aspect);
    int width = tft.width();
    int height = tft.height();
    tft.fillScreen(colormask[aspect]);
    tft.drawRect(0, 0, width, height, WHITE);
    tft.drawRect(32, 32, width - 64, height - 64, WHITE);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(40, 40);
    tft.print("ID=0x");
    tft.print(ID, HEX);
    if (ID == 0xD3D3) tft.print(" w/o");
    tft.setCursor(40, 70);
    tft.print(aspectname[aspect]);
    tft.setCursor(40, 100);
    tft.print(width);
    tft.print(" x ");
    tft.print(height);
    tft.setTextColor(WHITE);
    tft.setCursor(40, 130);
    tft.print(colorname[aspect]);
    if (++aspect > 3) aspect = 0;
    delay(5000);
}

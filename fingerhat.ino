#include <M5StickC.h>
#include "fingerhat.h"

FingerHat finger;

void setup() {
  M5.begin();
  finger.setup();
  M5.Lcd.println("FingerHat");
}

void loop() {
  uint8_t ret;

  if (M5.BtnA.wasPressed()) {
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.println("Sleep");
    ret = finger.sleep();
    if (ret >= ERR_IO_ERROR) {
      M5.Lcd.setTextColor(RED);
      M5.Lcd.printf("Error: %02x", ret);
    } else {
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.printf("ACK: %02x", ret);
    }
  }

  M5.update();
}

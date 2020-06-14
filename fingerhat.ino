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
    M5.Lcd.fillRect(0, 20, 160, 60, BLACK);
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.println("Number of Users");
    ret = finger.getUserCount();
    if (ret >= ERR_IO_ERROR) {
      M5.Lcd.setTextColor(RED);
      M5.Lcd.printf("Error: %02x", ret);
    } else {
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.printf("ACK: %02x\n", ret);
      M5.Lcd.printf("Count: %d", finger.res[IDX_Q2]);
    }
  }

  M5.update();
}

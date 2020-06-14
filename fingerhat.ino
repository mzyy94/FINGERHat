#include <M5StickC.h>
#include "fingerhat.h"

FingerHat finger;
uint8_t count;

void setup() {
  M5.begin();
  finger.setup();
  finger.getUserCount();
  count = finger.res[IDX_Q2];
  M5.Lcd.print("FingerHat ");
  M5.Lcd.setTextColor(RED);
  M5.Lcd.println(count);
  M5.Lcd.setTextColor(WHITE);
}

void loop() {
  uint8_t ret;

  if (M5.BtnA.wasPressed()) {
    M5.Lcd.fillRect(0, 20, 160, 60, BLACK);
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.println("User 3?");
    ret = finger.identifyUser(3);
    if (ret >= ERR_IO_ERROR) {
      M5.Lcd.setTextColor(RED);
      M5.Lcd.printf("Error: %02x", ret);
    } else {
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.printf("ACK: %02x\n", ret);
    }
  }

  M5.update();
}

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
  uint8_t data[4096];
  uint16_t len;

  if (M5.BtnA.wasPressed()) {
    M5.Lcd.fillRect(0, 20, 160, 60, BLACK);
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.println("Capture Finger");
    ret = finger.captureImage(data, &len);
    if (ret >= ERR_IO_ERROR) {
      M5.Lcd.setTextColor(RED);
      M5.Lcd.printf("Error: %02x", ret);
    } else {
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.printf("ACK: %02x\n", ret);
      if (ret == ACK_SUCCESS) {
        M5.Lcd.printf("LEN: %d\n", len);
        uint16_t i;
        uint8_t x = 0, y = 80;
        for (i = 0; i < len; i++) {
          uint8_t buf = *(data + i);
          uint16_t color;
          uint8_t pixel;
          pixel = (buf & 0x0f) << 4;
          color = M5.Lcd.color565(pixel, pixel, pixel);
          M5.Lcd.drawPixel(x++, y, color);
          pixel = (buf >> 4) << 4;
          color = M5.Lcd.color565(pixel, pixel, pixel);
          M5.Lcd.drawPixel(x++, y, color);
          if (x >= 80) {
            x = 0;
            y++;
          }
        }
      }
    }
  }

  M5.update();
}

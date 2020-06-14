#include "fingerhat.h"

#define CHECK_ERROR(exp, value) \
  do {                          \
    if (exp)                    \
      return value;             \
  } while (0)

void FingerHat::setup() {
  Serial2.begin(19200, SERIAL_8N1, HAT_TX, HAT_RX);
  Serial2.setRxBufferSize(4096);
}

uint8_t FingerHat::sleep() {
  return send(CMD_SLEEP);
}

uint8_t FingerHat::send(uint8_t cmd) {
  uint8_t buf[8] = {0};
  uint8_t i;

  buf[0] = buf[7] = 0xF5;
  buf[IDX_CMD] = cmd;
  for (i = 1; i < 6; i++) {
    buf[IDX_CHK] ^= buf[i];
  }

  CHECK_ERROR(Serial2.write(buf, 8) != 8, ERR_IO_ERROR);
  delay(300);
  CHECK_ERROR(!Serial2.available(), ERR_NO_DATA);
  CHECK_ERROR(Serial2.readBytes(buf, 8) != 8, ERR_IO_ERROR);
  CHECK_ERROR(buf[0] != 0xF5 || buf[7] != 0xF5, ERR_INVALID_DATA);
  for (i = 1; i < 6; i++) {
    buf[IDX_CHK] ^= buf[i];
  };
  CHECK_ERROR(buf[IDX_CHK] != 0, ERR_INVALID_CHK);
  return buf[IDX_Q3];
}
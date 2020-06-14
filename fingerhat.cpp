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

uint8_t FingerHat::getUserCount() {
  return send(CMD_USER_COUNT);
}

uint8_t FingerHat::send(uint8_t cmd) {
  uint8_t req[8] = {0};
  uint8_t i;

  req[0] = req[7] = 0xF5;
  req[IDX_CMD] = cmd;
  for (i = 1; i < 6; i++) {
    req[IDX_CHK] ^= req[i];
  }

  CHECK_ERROR(Serial2.write(req, 8) != 8, ERR_IO_ERROR);
  delay(300);
  CHECK_ERROR(!Serial2.available(), ERR_NO_DATA);
  CHECK_ERROR(Serial2.readBytes(res, 8) != 8, ERR_IO_ERROR);
  CHECK_ERROR(res[0] != 0xF5 || res[7] != 0xF5, ERR_INVALID_DATA);
  for (i = 1; i < 6; i++) {
    res[IDX_CHK] ^= res[i];
  };
  CHECK_ERROR(res[IDX_CHK] != 0, ERR_INVALID_CHK);
  return res[IDX_Q3];
}
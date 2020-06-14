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

uint8_t FingerHat::addUser(uint8_t id, uint8_t permission) {
  uint8_t ret;
  ret = send(CMD_USER_ADD_1, 0, id, permission, 3000);
  CHECK_ERROR(ret != ACK_SUCCESS, ret);
  ret = send(CMD_USER_ADD_2, 0, id, permission, 3000);
  CHECK_ERROR(ret != ACK_SUCCESS, ret);
  ret = send(CMD_USER_ADD_3, 0, id, permission, 3000);
  return ret;
}

uint8_t FingerHat::deleteUser(uint8_t id) {
  return send(CMD_DELETE_UESR, 0, id, 0, 1000);
}

uint8_t FingerHat::deleteAll() {
  return send(CMD_DELETE_ALL, 1000);
}

uint8_t FingerHat::getPermission(uint8_t id) {
  return send(CMD_GET_PERMISSION, 0, id, 0, 1000);
}

uint8_t FingerHat::searchUser() {
  return send(CMD_SEARCH, 3000);
}

uint8_t FingerHat::identifyUser(uint8_t id) {
  return send(CMD_IDENTIFY, 0, id, 0, 3000);
}

uint8_t FingerHat::send(uint8_t cmd) {
  return send(cmd, 300);
}

uint8_t FingerHat::send(uint8_t cmd, uint16_t timeout) {
  return send(cmd, 0, 0, 0, timeout);
}

uint8_t FingerHat::send(uint8_t cmd,
                        uint8_t p1,
                        uint8_t p2,
                        uint8_t p3,
                        uint16_t timeout) {
  uint8_t req[8] = {0};
  uint16_t i;

  req[0] = req[7] = 0xF5;
  req[IDX_CMD] = cmd;
  req[IDX_P1] = p1;
  req[IDX_P2] = p2;
  req[IDX_P3] = p3;

  for (i = 1; i < 6; i++) {
    req[IDX_CHK] ^= req[i];
  }

  CHECK_ERROR(Serial2.write(req, 8) != 8, ERR_IO_ERROR);
  for (i = 0; i < timeout; i++) {
    if (Serial2.available()) {
      break;
    }
    delay(1);
  }
  CHECK_ERROR(i == timeout, ERR_NO_DATA);
  CHECK_ERROR(Serial2.readBytes(res, 8) != 8, ERR_IO_ERROR);
  CHECK_ERROR(res[0] != 0xF5 || res[7] != 0xF5, ERR_INVALID_DATA);
  for (i = 1; i < 6; i++) {
    res[IDX_CHK] ^= res[i];
  };
  CHECK_ERROR(res[IDX_CHK] != 0, ERR_INVALID_CHK);
  return res[IDX_Q3];
}
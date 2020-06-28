/*
 *  Copyright (c) 2020 Yuki MIZUNO
 *  This file is part of FINGERHat - https://github.com/mzyy94/FINGERHat
 *
 *  SPDX-License-Identifier: MIT
 *  See LICENSE for more information.
 */

#include "fingerhat.h"

#define CHECK_ERROR(exp, value) \
  do {                          \
    if (exp)                    \
      return value;             \
  } while (0)

CFingerHat FingerHat;

void CFingerHat::setup() {
  setup(HAT_TX, HAT_RX);
}

void CFingerHat::setup(uint8_t pin_tx, uint8_t pin_rx) {
  Serial2.begin(19200, SERIAL_8N1, pin_tx, pin_rx);
  Serial2.setRxBufferSize(4096);
}

uint8_t CFingerHat::sleep() {
  return send(CMD_SLEEP);
}

uint8_t CFingerHat::getUserCount() {
  uint8_t ret = send(CMD_USER_COUNT);
  CHECK_ERROR(ret != ACK_SUCCESS, 0xFF);
  return res.rx.q2;
}

uint8_t CFingerHat::addUser(uint8_t id, uint8_t permission) {
  uint8_t ret;
  ret = send(CMD_USER_ADD_1, 0, id, permission, 3000);
  CHECK_ERROR(ret != ACK_SUCCESS, ret);
  ret = send(CMD_USER_ADD_2, 0, id, permission, 3000);
  CHECK_ERROR(ret != ACK_SUCCESS, ret);
  ret = send(CMD_USER_ADD_3, 0, id, permission, 3000);
  return ret;
}

uint8_t CFingerHat::deleteUser(uint8_t id) {
  return send(CMD_DELETE_UESR, 0, id, 0, 1000);
}

uint8_t CFingerHat::deleteAll() {
  return send(CMD_DELETE_ALL, 1000);
}

uint8_t CFingerHat::getPermission(uint8_t id) {
  uint8_t ret = send(CMD_GET_PERMISSION, 0, id, 0, 1000);
  CHECK_ERROR(ret == ACK_NOUSER, 0xFF);
  return ret;
}

uint8_t CFingerHat::captureImage(uint8_t* data, uint16_t* len) {
  uint8_t ret;
  uint16_t i, timeout = 3000;
  uint8_t head[1], tail[2];

  ret = send(CMD_IMAGE_CAPTURE, 3000);
  CHECK_ERROR(ret != ACK_SUCCESS, ret);
  *len = (((uint16_t)res.rx.q1) * 256) + ((uint16_t)res.rx.q2);
  for (i = 0; i < timeout; i++) {
    if (Serial2.available()) {
      break;
    }
    delay(1);
  }
  CHECK_ERROR(i == timeout, ERR_NO_DATA | MASK_ERROR_PAYLOAD);
  CHECK_ERROR(Serial2.readBytes(head, 1) != 1,
              ERR_IO_ERROR | MASK_ERROR_PAYLOAD);
  CHECK_ERROR(Serial2.readBytes(data, *len) != *len,
              ERR_IO_ERROR | MASK_ERROR_PAYLOAD);
  CHECK_ERROR(Serial2.readBytes(tail, 2) != 2,
              ERR_IO_ERROR | MASK_ERROR_PAYLOAD);
  CHECK_ERROR(head[0] != MARKER || tail[1] != MARKER,
              ERR_INVALID_DATA | MASK_ERROR_PAYLOAD);
  for (i = 0; i < *len; i++) {
    tail[0] ^= data[i];
  }
  CHECK_ERROR(tail[0] != 0, ERR_INVALID_CHK);

  return ret;
}

uint8_t CFingerHat::searchUser() {
  uint8_t ret = send(CMD_SEARCH, 3000);
  CHECK_ERROR(ret == ACK_NOUSER || ret == ACK_TIMEOUT, 0xFF);
  return res.rx.q2;
}

uint8_t CFingerHat::identifyUser(uint8_t id) {
  return send(CMD_IDENTIFY, 0, id, 0, 3000);
}

uint8_t CFingerHat::send(uint8_t cmd) {
  return send(cmd, 300);
}

uint8_t CFingerHat::send(uint8_t cmd, uint16_t timeout) {
  return send(cmd, 0, 0, 0, timeout);
}

uint8_t CFingerHat::send(uint8_t cmd,
                         uint8_t p1,
                         uint8_t p2,
                         uint8_t p3,
                         uint16_t timeout) {
  uint16_t i;
  uint8_t chk = 0;

  req.tx.begin = req.tx.end = MARKER;
  req.tx.cmd = cmd;
  req.tx.p1 = p1;
  req.tx.p2 = p2;
  req.tx.p3 = p3;
  req.tx.zero = req.tx.chk = 0;

  for (i = 1; i < 6; i++) {
    req.tx.chk ^= req.buffer[i];
  }
  CHECK_ERROR(Serial2.write(req.buffer, 8) != 8, ERR_IO_ERROR);

  for (i = 0; i < timeout; i++) {
    if (Serial2.available()) {
      break;
    }
    delay(1);
  }
  CHECK_ERROR(i == timeout, ERR_NO_DATA);

  CHECK_ERROR(Serial2.readBytes(res.buffer, 8) != 8, ERR_IO_ERROR);
  CHECK_ERROR(res.rx.begin != MARKER || res.rx.end != MARKER, ERR_INVALID_DATA);
  for (i = 1; i < 6; i++) {
    chk ^= res.buffer[i];
  };
  CHECK_ERROR(res.rx.chk != chk, ERR_INVALID_CHK);
  return res.rx.q3;
}
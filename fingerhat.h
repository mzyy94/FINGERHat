#ifndef __FINGERHAT_H_
#define __FINGERHAT_H_

#include <M5StickC.h>

#define HAT_TX 26
#define HAT_RX 0

#define CMD_SLEEP 0x2C

#define ACK_SUCCESS 0x00
#define ACK_FAIL 0x01
#define ACK_FULL 0x04
#define ACK_NOUSER 0x05
#define ACK_USER_OCCUPIED 0x06　　
#define ACK_USER_EXIST 0x07
#define ACK_TIMEOUT 0x08

#define ERR_IO_ERROR 0x10
#define ERR_NO_DATA 0x11
#define ERR_INVALID_DATA 0x12
#define ERR_INVALID_CHK 0x13

#define IDX_CMD 1
#define IDX_Q1 2
#define IDX_Q2 3
#define IDX_Q3 4
#define IDX_CHK 6

class FingerHat {
 public:
  void setup();
  uint8_t sleep();
  uint8_t send(uint8_t cmd);

 private:
};

#endif
#ifndef __FINGERHAT_H_
#define __FINGERHAT_H_

#include <M5StickC.h>

#define HAT_TX 26
#define HAT_RX 0

#define CMD_SLEEP 0x2C
#define CMD_USER_COUNT 0x09
#define CMD_USER_ADD_1 0x01
#define CMD_USER_ADD_2 0x02
#define CMD_USER_ADD_3 0x03
#define CMD_DELETE_UESR 0x04
#define CMD_DELETE_ALL 0x05
#define CMD_GET_PERMISSION 0x0A
#define CMD_IDENTIFY 0x0B
#define CMD_SEARCH 0x0C
#define CMD_IMAGE_CAPTURE 0x24

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

#define MASK_ERROR 0x10
#define MASK_ERROR_PAYLOAD 0x20

#define MARKER 0xF5

union command_t {
  uint8_t buffer[8];
  struct {
    uint8_t begin;
    uint8_t cmd;
    uint8_t p1;
    uint8_t p2;
    uint8_t p3;
    uint8_t zero;
    uint8_t chk;
    uint8_t end;
  } tx;

  struct {
    uint8_t begin;
    uint8_t cmd;
    uint8_t q1;
    uint8_t q2;
    uint8_t q3;
    uint8_t zero;
    uint8_t chk;
    uint8_t end;
  } rx;
};

class CFingerHat {
 public:
  command_t req;
  command_t res;

  void setup();
  uint8_t sleep();
  uint8_t getUserCount();
  uint8_t addUser(uint8_t id, uint8_t permission);
  uint8_t deleteUser(uint8_t id);
  uint8_t deleteAll();
  uint8_t getPermission(uint8_t id);
  uint8_t captureImage(uint8_t* data, uint16_t* len);
  uint8_t searchUser();
  uint8_t identifyUser(uint8_t id);

  uint8_t send(uint8_t cmd);
  uint8_t send(uint8_t cmd, uint16_t timeout);
  uint8_t send(uint8_t cmd,
               uint8_t p1,
               uint8_t p2,
               uint8_t p3,
               uint16_t timeout);

 private:
};

extern CFingerHat FingerHat;

#endif
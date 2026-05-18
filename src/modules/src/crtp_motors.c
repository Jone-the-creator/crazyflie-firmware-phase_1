#include "crtp.h"
#include "motors.h"
#include <string.h>

#define CRTP_PORT_MOTORS 0x09   // unused port

static void motorsCrtpHandler(CRTPPacket *pk)
{
    // Expect 4 uint16 motor values
    if (pk->size < 8) {
        return;
    }

    uint16_t m[4];
    memcpy(m, pk->data, sizeof(m));

    // Clamp to safe range
    for (int i = 0; i < 4; i++) {
        if (m[i] > 60000) {
            m[i] = 60000;
        }
    }

    // Send directly to motors
    motorsSetRatio(MOTOR_M1, m[0]);
    motorsSetRatio(MOTOR_M2, m[1]);
    motorsSetRatio(MOTOR_M3, m[2]);
    motorsSetRatio(MOTOR_M4, m[3]);
}

static CrtpHandler motorsHandler = {
    .port = CRTP_PORT_MOTORS,
    .channel = 0,
    .cb = motorsCrtpHandler
};

void motorsCrtpInit(void)
{
    crtpRegisterHandler(&motorsHandler);
}
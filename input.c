#include "input.h"

#include "display/dispdrv.h"
#include "settings.h"

static volatile int8_t encCnt = 0;
static volatile CmdBtn cmdBuf = BTN_NO;

static int8_t encRes = 0;

void inputInit(void)
{
    encRes = (int8_t)settingsRead(PARAM_SYSTEM_ENC_RES);
}

void inputSetEncRes(int8_t value)
{
    encRes = value;
}

int8_t inputGetEncRes(void)
{
    return encRes;
}

void inputPoll(void)
{
    // Antibounce counter
    static int16_t btnCntPress = 0;
    static int16_t btnCntRelease = 0;

    // Previous state
    static uint8_t btnPrev = BTN_NO;
    static uint8_t encPrev = ENC_NO;

    // Current state
    uint8_t btnNow = dispdrvGetBus();

    // If encoder event has happened, inc/dec encoder counter
    if (encRes) {
        uint8_t encNow = btnNow & ENC_AB;
        btnNow &= ~ENC_AB;

        if ((encPrev == ENC_NO && encNow == ENC_A) ||
            (encPrev == ENC_A && encNow == ENC_AB) ||
            (encPrev == ENC_AB && encNow == ENC_B) ||
            (encPrev == ENC_B && encNow == ENC_NO))
            encCnt++;
        if ((encPrev == ENC_NO && encNow == ENC_B) ||
            (encPrev == ENC_B && encNow == ENC_AB) ||
            (encPrev == ENC_AB && encNow == ENC_A) ||
            (encPrev == ENC_A && encNow == ENC_NO))
            encCnt--;
        encPrev = encNow;
    }

    // On button event place it to command buffer
    if (btnNow) {
        if (btnNow == btnPrev) {
            btnCntPress++;
            if (btnCntPress == LONG_PRESS) {
                cmdBuf = (btnPrev << 8) & 0x0000FF00;
                if (btnNow & (BTN_D3 | BTN_D4 | ENC_A | ENC_B)) {
                    btnCntPress -= AUTOREPEAT;
                }
            }
        } else {
            if (btnCntRelease == SHORT_PRESS) {
                cmdBuf |= (btnNow << 16) & 0x00FF0000;
            }
        }
        btnCntRelease = 0;
    } else {
        if (btnNow == btnPrev) {
            if (btnCntRelease < SHORT_PRESS) {
                btnCntRelease++;
            }
        }
        if ((btnCntPress > SHORT_PRESS) && (btnCntPress < LONG_PRESS - AUTOREPEAT)) {
            cmdBuf = btnPrev;
        }
        btnCntPress = 0;
    }
    btnPrev = btnNow;
}

int8_t getEncoder(void)
{
    int8_t ret = 0;

    if (encRes) {
        ret = encCnt / encRes;
        encCnt -= (ret * encRes);
    } else {
        ret = encCnt;
        encCnt = 0;
    }

    return ret;
}

CmdBtn getBtnCmd(void)
{
    CmdBtn ret = cmdBuf;
    cmdBuf = BTN_NO;

    return ret;
}

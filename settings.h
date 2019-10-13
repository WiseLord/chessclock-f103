#ifndef SETTINGS_H
#define SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define EE_EMPTY        (uint16_t)0xFFFF
#define EE_NOT_FOUND    (uint16_t)0x8000

#define GENERATE_PARAM_RC(CMD)  PARAM_RC_ ## CMD,

typedef struct {
    uint16_t cell;  // EE cell
    int16_t def;    // Default value
} EE_Map;

typedef uint8_t Param;
enum {
    PARAM_NULL = 0,

    PARAM_DISPLAY_ROTATE,
    PARAM_DISPLAY_PALETTE,

    PARAM_CHESS_GAME_H,
    PARAM_CHESS_GAME_M,
    PARAM_CHESS_MOVE_S,

    PARAM_SYSTEM_LANG,
    PARAM_SYSTEM_ENC_RES,

    PARAM_END
};

void settingsInit(void);
int16_t settingsGet(Param param);
void settingsSet(Param param, int16_t value);

int16_t settingsRead(Param param);
void settingsStore(Param param, int16_t value);

const EE_Map *eeMapGet(void);

#ifdef __cplusplus
}
#endif

#endif // SETTINGS_H

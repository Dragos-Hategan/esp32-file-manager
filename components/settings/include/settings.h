#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"

/**
 * @brief Perform one-time system bring-up (NVS, display, touch, SD) and UI calibration.
 *
 * Initializes NVS, display/LVGL (backlight + default Domine font theme), touch driver,
 * LVGL input registration, touch calibration (load from NVS and run dialog), and SD
 * card over SDSPI with retry scheduling. Ends by seeding in-memory settings state.
 *
 * @note Call once at startup before launching UI tasks.
 */
void starting_routine(void);

/**
 * @brief Rotate the display in 90-degree increments (0 -> 90 -> 180 -> 270 -> 0).
 */
void settings_rotate_screen(void);

#ifdef __cplusplus
}
#endif

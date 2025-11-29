#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"
#include "lvgl.h"

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
 * @brief Open the Settings UI, creating it on first call and loading it into LVGL.
 *
 * @param return_screen Screen to switch back to when closing settings (nullable).
 * @return ESP_OK on success, ESP_ERR_INVALID_ARG on bad return screen
 */
esp_err_t settings_open_settings(lv_obj_t *return_screen);

#ifdef __cplusplus
}
#endif

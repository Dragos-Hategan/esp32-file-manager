#include "settings.h"

#include <stddef.h>

#include "bsp/esp-bsp.h"
#include "Domine_14.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "lvgl.h"

#include "calibration_xpt2046.h"
#include "touch_xpt2046.h"
#include "sd_card.h"

typedef struct{
    int incr_90_deg;
}settings_t;

static settings_t s_settings;
static const char *TAG = "settings";

/**
 * @brief Initialize the Non-Volatile Storage (NVS) flash partition.
 *
 * This function initializes the NVS used for storing persistent configuration
 * and calibration data.  
 * If the NVS partition is full, corrupted, or created with an incompatible SDK version,
 * it will be erased and reinitialized automatically.
 *
 * @return
 * - ESP_OK on successful initialization  
 * - ESP_ERR_NVS_NO_FREE_PAGES if the partition had to be erased  
 * - ESP_ERR_NVS_NEW_VERSION_FOUND if a version mismatch was detected  
 * - Other error codes from @ref nvs_flash_init() if initialization fails
 *
 * @note This function should be called before performing any NVS read/write operations.
 */
static esp_err_t init_nvs(void);

/**
 * @brief Starts the BSP display subsystem and reports the initialization result.
 *
 * This function calls `bsp_display_start()` and converts its boolean return
 * value into an `esp_err_t`.  
 * 
 * @return ESP_OK      Display successfully initialized.
 * @return ESP_FAIL    Display failed to initialize.
 */
static esp_err_t bsp_display_start_result(void);

/**
 * @brief Apply the Domine 14 font as the app-wide default LVGL theme font.
 */
static void apply_default_font_theme(void);

/**
 * @brief Initialize runtime settings defaults.
 */
static void init_settings(void);

void starting_routine(void)
{
    /* ----- Init NSV ----- */
    ESP_LOGI(TAG, "Initializing NVS");
    ESP_ERROR_CHECK(init_nvs());

    /* ----- Init Display and LVGL ----- */
    ESP_LOGI(TAG, "Starting bsp for ILI9341 display");
    ESP_ERROR_CHECK(bsp_display_start_result()); 
    ESP_ERROR_CHECK(bsp_display_backlight_on()); 
    apply_default_font_theme();

    /* ----- Init XPT2046 Touch Driver ----- */
    ESP_LOGI(TAG, "Initializing XPT2046 touch driver");
    ESP_ERROR_CHECK(init_touch()); 

    /* ----- Register Touch Driver To LVGL ----- */
    ESP_LOGI(TAG, "Registering touch driver to LVGL");
    ESP_ERROR_CHECK(register_touch_to_lvgl());

    /* ----- Load XPT2046 Touch Driver Calibration ----- */
    bool calibration_found;
    ESP_LOGI(TAG, "Check for touch driver calibration data");
    load_nvs_calibration(&calibration_found);

    /* ----- Calibration Test ----- */
    ESP_LOGI(TAG, "Start calibration dialog");
    ESP_ERROR_CHECK(calibration_test(calibration_found));

    /* ----- Init SDSPI ----- */
    ESP_LOGI(TAG, "Initializing SDSPI");
    esp_err_t err = init_sdspi();
    if (err != ESP_OK){
        retry_init_sdspi();
    }

    init_settings();
}

void settings_rotate_screen(void)
{
    lv_display_t *display = lv_display_get_default();
    if (!display){
        return;
    }

    switch(s_settings.incr_90_deg){
        case 0:
            lv_display_set_rotation(display, LV_DISPLAY_ROTATION_270);
            s_settings.incr_90_deg = 1;
            break;
        case 1:
            lv_display_set_rotation(display, LV_DISPLAY_ROTATION_180);
            s_settings.incr_90_deg = 2;        
            break;
        case 2:
            lv_display_set_rotation(display, LV_DISPLAY_ROTATION_90);
            s_settings.incr_90_deg = 3;        
            break;
        case 3:
            lv_display_set_rotation(display, LV_DISPLAY_ROTATION_0);
            s_settings.incr_90_deg = 0;        
            break; 
        
        default:
            ESP_LOGE(TAG, "Wrong rotation setting");
            break;
    }
}

static esp_err_t init_nvs(void)
{
    esp_err_t nvs_err = nvs_flash_init();
    if (nvs_err == ESP_ERR_NVS_NO_FREE_PAGES || nvs_err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        nvs_err = nvs_flash_init();
    }

    return nvs_err;
}

static esp_err_t bsp_display_start_result(void)
{
    if (!bsp_display_start()){
        ESP_LOGE(TAG, "BSP failed to initialize display.");
        return ESP_FAIL;
    } 
    return ESP_OK;
}

static void apply_default_font_theme(void)
{
    lv_display_t *disp = lv_display_get_default();
    if (!disp) {
        ESP_LOGW(TAG, "No LVGL display available; cannot set theme font");
        return;
    }

    lv_theme_t *theme = lv_theme_default_init(
        disp,
        lv_palette_main(LV_PALETTE_BLUE),
        lv_palette_main(LV_PALETTE_RED),
        false,
        &Domine_14);

    if (!theme) {
        ESP_LOGW(TAG, "Failed to init LVGL default theme with Domine_14");
        return;
    }

    lv_display_set_theme(disp, theme);

    /* Ensure overlay/system layers also inherit the font (dialogs, prompts, etc.) */
    lv_obj_t *act_scr = lv_display_get_screen_active(disp);
    lv_obj_t *top_layer = lv_display_get_layer_top(disp);
    lv_obj_t *sys_layer = lv_display_get_layer_sys(disp);
    lv_obj_set_style_text_font(act_scr, &Domine_14, 0);
    lv_obj_set_style_text_font(top_layer, &Domine_14, 0);
    lv_obj_set_style_text_font(sys_layer, &Domine_14, 0);
}

static void init_settings(void)
{
    s_settings.incr_90_deg = 0;
}

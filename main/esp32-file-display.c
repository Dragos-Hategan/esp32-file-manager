#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "lv_demos.h"
#include "lv_examples.h"
#include "bsp/esp-bsp.h"

#include "calibration_ui.h"
#include "touch_xpt2046.h"

#include "file_browser.h"

static char *TAG = "app_main";

#define LOG_MEM_INFO    (0)

static void my_task_function(void *arg)
{
    ESP_LOGI(TAG, "LVGL File Display");

    UBaseType_t freeStack[10];
    freeStack[0] = uxTaskGetStackHighWaterMark(NULL);

    /* ----- Init NVS ----- */
    bool calibration_found;
    init_nvs(&calibration_found);
    freeStack[1] = uxTaskGetStackHighWaterMark(NULL);

    /* ----- Init display and LVGL ----- */
    bsp_display_start(); 
    bsp_display_backlight_on(); 
    freeStack[2] = uxTaskGetStackHighWaterMark(NULL);

    // /* ----- Init touch driver ----- */
    // //init_touch(); 
    // freeStack[3] = uxTaskGetStackHighWaterMark(NULL);

    // /* ----- Register Touch to LVGL ----- */
    // bsp_display_lock(0); 
    // lv_indev_t *indev = NULL;
    // indev = register_touch_with_lvgl(); 
    // if (indev == NULL) { bsp_display_unlock(); return; }
    // bsp_display_unlock(); 
    // ESP_LOGI(TAG, "XPT2046 touch registered to LVGL");
    // freeStack[4] = uxTaskGetStackHighWaterMark(NULL);
    
    // /* ----- Calibration Test ----- */
    // if (!calibration_found) {
    //     // No calibration saved: runs calibration directly
    //     run_5point_touch_calibration();
    //     freeStack[5] = uxTaskGetStackHighWaterMark(NULL);

    // } else {
    //     // Run calibration?
    //     bool run;
    //     run = ui_yes_no_dialog("Run Touch Screen Calibration?");
    //     freeStack[5] = uxTaskGetStackHighWaterMark(NULL);

    //     if (run) {
    //         run_5point_touch_calibration();
    //         freeStack[6] = uxTaskGetStackHighWaterMark(NULL);

    //     } else {
    //         // We keep s_cal from NVS; we just clear the screen
    //         bsp_display_lock(0); 
    //         lv_obj_clean(lv_screen_active());
    //         freeStack[6] = uxTaskGetStackHighWaterMark(NULL);
    //         bsp_display_unlock(); 
    //     }
    // }
    // freeStack[7] = uxTaskGetStackHighWaterMark(NULL);

    /* ----- Launch file browser ----- */
    init_sdspi();
    file_browser_config_t browser_cfg = {
        .root_path = SD_SPI_MOUNT_POINT,
        .max_entries = 512,
    };
    esp_err_t fb_err = file_browser_start(&browser_cfg);
    if (fb_err != ESP_OK) {
        ESP_LOGE(TAG, "File browser failed to start: %s", esp_err_to_name(fb_err));
    }
    freeStack[8] = uxTaskGetStackHighWaterMark(NULL);


    // /* ----- Draw Demo Widgets ----- */
    // bsp_display_lock(0); 
    // lv_demo_widgets(); 
    // //lv_demo_stress();
    // freeStack[9] = uxTaskGetStackHighWaterMark(NULL);
    // bsp_display_unlock(); 

    /* ----- Print Available Memory ----- */
    for (int i = 0; i < 10; i++){
        printf("Free stack[%d]: %d words (%d bytes)\n", i, freeStack[i], freeStack[i] * sizeof(StackType_t));
    }

    vTaskDelay(portMAX_DELAY);
}

void app_main(void)
{
    xTaskCreatePinnedToCore(my_task_function, "MyTask", 12 * 1024, NULL, 1, NULL, 0);

    //while (1){
        // size_t free_ram[5];
        // free_ram[0] = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
        // free_ram[1] = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
        // free_ram[2] = heap_caps_get_free_size(MALLOC_CAP_DMA);

        // heap_caps_print_heap_info(0);  // 0 = all caps
        // printf("\n Free SPIRAM= %u | Free SRAM= %u | Free DMA_RAM= %u\n\n\n\n", free_ram[0], free_ram[1], free_ram[2]);
        // vTaskDelay(pdMS_TO_TICKS(5000));
    //}
}

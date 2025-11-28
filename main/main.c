#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "file_browser.h"
#include "settings.h"

static char *TAG = "app_main";

#define LOG_MEM_INFO    (0)

static void main_task(void *arg)
{
    ESP_LOGI(TAG, "\n\n ********** LVGL File Display ********** \n");

    starting_routine();
    ESP_ERROR_CHECK(file_browser_start());

    vTaskDelete(NULL);
}

void app_main(void)
{
    xTaskCreatePinnedToCore(main_task, "MyTask", 8 * 1024, NULL, 1, NULL, 0);
    size_t last_free_heap = 0;
    size_t min_free_heap = UINT_MAX;
    size_t max_free_heap = 0;
    
    while (1){
        size_t free_heap = esp_get_free_heap_size();

        if (free_heap < min_free_heap){
            min_free_heap = free_heap;
        }

        if (free_heap > max_free_heap){
            max_free_heap = free_heap;
        }

        if (free_heap != last_free_heap){
            printf("----- HEAP INFO ----- free=%u  min_free_heap_ever=%u max_free_heap_ever=%u ----- HEAP INFO ----- \n", free_heap, min_free_heap, max_free_heap);
            last_free_heap = free_heap;
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

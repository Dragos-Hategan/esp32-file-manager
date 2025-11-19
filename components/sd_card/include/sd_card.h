#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "esp_err.h"

/**
 * @brief Initializes the SDSPI bus and mounts the SD card filesystem.
 *
 * Performs a one-time initialization of the SPI bus specified by
 * @c CONFIG_SDSPI_BUS_HOST, followed by mounting the FAT filesystem at
 * @c CONFIG_SDSPI_MOUNT_POINT using @c esp_vfs_fat_sdspi_mount.
 *
 * The function is safe to call multiple times. If the SPI bus is already
 * initialized or the SD card is already mounted, the function returns
 * @c ESP_OK without reinitializing or remounting resources.
 *
 * @return esp_err_t  
 *         - @c ESP_OK on success (bus initialized and/or filesystem mounted)  
 *         - ESP-IDF error code if SPI bus initialization or FAT mount fails  
 *
 * @note No fatal aborts are used; all failures are reported via the returned
 *       @c esp_err_t value.
 *
 * @post On success, the VFS mount point is available and @c sd_card_handle
 *       points to a valid @c sdmmc_card_t structure.
 */
esp_err_t init_sdspi(void);

/**
 * @brief Prompt the user then retry SD card initialization with UI feedback.
 *
 * @return true  if the bus/card recovered inside @ref SDSPI_MAX_RETRIES.
 * @return false once all retries are exhausted.
 */
bool retry_init_sdspi(void);

#ifdef __cplusplus
}
#endif

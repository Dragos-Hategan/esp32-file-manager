// main/fs_nav.h
#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <time.h>

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FS_NAV_MAX_PATH 256
#define FS_NAV_MAX_NAME 96

typedef enum {
    FS_NAV_SORT_NAME = 0,
    FS_NAV_SORT_DATE = 1,
    FS_NAV_SORT_SIZE = 2,
    FS_NAV_SORT_COUNT
} fs_nav_sort_mode_t;

typedef struct {
    char name[FS_NAV_MAX_NAME];
    bool is_dir;
    size_t size_bytes;
    time_t modified;
} fs_nav_entry_t;

typedef struct fs_nav {
    char root[FS_NAV_MAX_PATH];
    char current[FS_NAV_MAX_PATH];
    char relative[FS_NAV_MAX_PATH];
    fs_nav_entry_t *entries;
    size_t entry_count;
    size_t capacity;
    size_t max_entries;
    fs_nav_sort_mode_t sort_mode;
    bool ascending;
} fs_nav_t;

typedef struct {
    const char *root_path;
    size_t max_entries;
} fs_nav_config_t;

esp_err_t fs_nav_init(fs_nav_t *nav, const fs_nav_config_t *cfg);
void fs_nav_deinit(fs_nav_t *nav);
esp_err_t fs_nav_refresh(fs_nav_t *nav);
const fs_nav_entry_t *fs_nav_entries(const fs_nav_t *nav, size_t *count);
const char *fs_nav_current_path(const fs_nav_t *nav);
const char *fs_nav_relative_path(const fs_nav_t *nav);
bool fs_nav_can_go_parent(const fs_nav_t *nav);
esp_err_t fs_nav_enter(fs_nav_t *nav, size_t index);
esp_err_t fs_nav_go_parent(fs_nav_t *nav);
esp_err_t fs_nav_set_sort(fs_nav_t *nav, fs_nav_sort_mode_t mode, bool ascending);
fs_nav_sort_mode_t fs_nav_get_sort(const fs_nav_t *nav);
bool fs_nav_is_sort_ascending(const fs_nav_t *nav);

#ifdef __cplusplus
}
#endif

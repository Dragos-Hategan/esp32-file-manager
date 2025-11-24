#include "jpg.h"

#include <stdbool.h>
#include <string.h>

#include "bsp/esp-bsp.h"
#include "esp_err.h"
#include "esp_log.h"

#define TAG "jpg_viewer"
#define IMG_VIEWER_MAX_PATH 256

typedef struct {
    bool active;
    lv_obj_t *screen;
    lv_obj_t *image;
    lv_obj_t *close_btn;
    lv_obj_t *path_label;
    lv_obj_t *return_screen;
    lv_obj_t *previous_screen;
    lv_timer_t *dim_timer;
    char path[IMG_VIEWER_MAX_PATH];
} jpg_viewer_ctx_t;

static jpg_viewer_ctx_t s_jpg_viewer;

/**
 * @brief Set the LVGL image source to a JPEG on disk.
 *
 * Performs basic validation (non-empty path) before calling lv_image_set_src().
 *
 * @param img  LVGL image object (must be non-NULL).
 * @param path Path to JPEG file (drive-prefixed if using LVGL FS, e.g. "S:/...").
 * @return ESP_OK on success, ESP_ERR_INVALID_ARG on bad inputs.
 */
static esp_err_t jpg_handler_set_src(lv_obj_t *img, const char *path);

static void jpg_viewer_destroy_active(jpg_viewer_ctx_t *ctx);
static void jpg_viewer_build_ui(jpg_viewer_ctx_t *ctx, const char *path);
static void jpg_viewer_reset(jpg_viewer_ctx_t *ctx);
static void jpg_viewer_on_close(lv_event_t *e);
static void jpg_viewer_on_screen_tap(lv_event_t *e);
static void jpg_viewer_start_dim_timer(jpg_viewer_ctx_t *ctx);
static void jpg_viewer_dim_cb(lv_timer_t *timer);
static void jpg_viewer_set_close_opacity(jpg_viewer_ctx_t *ctx, lv_opa_t opa);
static void jpg_viewer_restore_close_opacity(jpg_viewer_ctx_t *ctx);
static void jpg_viewer_anim_set_btn_opa(void *obj, int32_t v);

esp_err_t jpg_viewer_open(const jpg_viewer_open_opts_t *opts)
{
    if (!opts || !opts->path || opts->path[0] == '\0') {
        return ESP_ERR_INVALID_ARG;
    }

    jpg_viewer_ctx_t *ctx = &s_jpg_viewer;

    if (ctx->active) {
        jpg_viewer_destroy_active(ctx);
    }

    ctx->return_screen = opts->return_screen;
    strlcpy(ctx->path, opts->path, sizeof(ctx->path));

    if (!bsp_display_lock(0)) {
        return ESP_ERR_TIMEOUT;
    }

    ctx->previous_screen = lv_screen_active();
    jpg_viewer_build_ui(ctx, opts->path);

    ESP_LOGW("", "Before jpg_handler_set_src");

    esp_err_t err = jpg_handler_set_src(ctx->image, opts->path);
    if (err != ESP_OK) {
        lv_obj_del(ctx->screen);
        ctx->screen = NULL;
        bsp_display_unlock();
        jpg_viewer_reset(ctx);
        return err;
    }

    ESP_LOGW("", "After jpg_handler_set_src");

    jpg_viewer_restore_close_opacity(ctx);

    lv_obj_center(ctx->image);
    lv_screen_load(ctx->screen);
    bsp_display_unlock();

    ctx->active = true;
    return ESP_OK;
}

static void jpg_viewer_destroy_active(jpg_viewer_ctx_t *ctx)
{
    if (!ctx || !ctx->active) {
        jpg_viewer_reset(ctx);
        return;
    }

    if (bsp_display_lock(0)) {
        if (ctx->screen) {
            lv_obj_del(ctx->screen);
        }
        bsp_display_unlock();
    }

    jpg_viewer_reset(ctx);
}

static void jpg_viewer_build_ui(jpg_viewer_ctx_t *ctx, const char *path)
{
    ctx->screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(ctx->screen, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(ctx->screen, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_all(ctx->screen, 8, 0);
    lv_obj_add_flag(ctx->screen, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ctx->screen, jpg_viewer_on_screen_tap, LV_EVENT_CLICKED, ctx);

    lv_obj_t *close_btn = lv_button_create(ctx->screen);
    ctx->close_btn = close_btn;
    lv_obj_set_size(close_btn, 32, 32);
    lv_obj_t *close_lbl = lv_label_create(close_btn);
    lv_label_set_text(close_lbl, LV_SYMBOL_CLOSE);
    lv_obj_center(close_lbl);
    lv_obj_align(close_btn, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_add_event_cb(close_btn, jpg_viewer_on_close, LV_EVENT_CLICKED, ctx);

    ctx->image = lv_image_create(ctx->screen);
    lv_obj_center(ctx->image);
}

static esp_err_t jpg_handler_set_src(lv_obj_t *img, const char *path)
{
    if (!img || !path || path[0] == '\0') {
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGW("", "Before lv_image_set_src");
    lv_image_set_src(img, path);
    ESP_LOGW("", "After lv_image_set_src");

    return ESP_OK;
}

static void jpg_viewer_reset(jpg_viewer_ctx_t *ctx)
{
    if (!ctx) {
        return;
    }
    if (ctx->dim_timer) {
        lv_timer_del(ctx->dim_timer);
        ctx->dim_timer = NULL;
    }
    memset(ctx, 0, sizeof(*ctx));
}

static void jpg_viewer_on_close(lv_event_t *e)
{
    jpg_viewer_ctx_t *ctx = lv_event_get_user_data(e);
    if (!ctx || !ctx->active) {
        return;
    }

    if (!bsp_display_lock(0)) {
        return;
    }

    lv_obj_t *old_screen = ctx->screen;
    lv_obj_t *target = ctx->return_screen ? ctx->return_screen : ctx->previous_screen;
    if (target) {
        lv_screen_load(target);
    }

    if (old_screen) {
        lv_obj_del(old_screen);
    }

    bsp_display_unlock();
    jpg_viewer_reset(ctx);
}

static void jpg_viewer_on_screen_tap(lv_event_t *e)
{
    jpg_viewer_ctx_t *ctx = lv_event_get_user_data(e);
    if (!ctx) {
        return;
    }
    jpg_viewer_restore_close_opacity(ctx);
}

static void jpg_viewer_start_dim_timer(jpg_viewer_ctx_t *ctx)
{
    if (!ctx) {
        return;
    }
    if (ctx->dim_timer) {
        lv_timer_reset(ctx->dim_timer);
    } else {
        ctx->dim_timer = lv_timer_create(jpg_viewer_dim_cb, 3000, ctx);
    }
}

static void jpg_viewer_dim_cb(lv_timer_t *timer)
{
    jpg_viewer_ctx_t *ctx = lv_timer_get_user_data(timer);
    if (!ctx) {
        return;
    }
    jpg_viewer_set_close_opacity(ctx, LV_OPA_20);
}

static void jpg_viewer_set_close_opacity(jpg_viewer_ctx_t *ctx, lv_opa_t opa)
{
    if (!ctx || !ctx->close_btn) {
        return;
    }
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, ctx->close_btn);
    lv_anim_set_values(&a, lv_obj_get_style_opa(ctx->close_btn, LV_PART_MAIN), opa);
    lv_anim_set_time(&a, 300);
    lv_anim_set_path_cb(&a, lv_anim_path_linear);
    lv_anim_set_exec_cb(&a, jpg_viewer_anim_set_btn_opa);
    lv_anim_start(&a);
}

static void jpg_viewer_restore_close_opacity(jpg_viewer_ctx_t *ctx)
{
    jpg_viewer_set_close_opacity(ctx, LV_OPA_COVER);
    jpg_viewer_start_dim_timer(ctx);
}

static void jpg_viewer_anim_set_btn_opa(void *obj, int32_t v)
{
    if (!obj) {
        return;
    }
    lv_obj_set_style_opa((lv_obj_t *)obj, (lv_opa_t)v, LV_PART_MAIN);
}

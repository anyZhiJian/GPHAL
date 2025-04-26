#ifndef __GPHAL_KEY_H
#define __GPHAL_KEY_H

#include "gphal.h"
#include "timer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    KEY_EVENT_PRESSED,
    KEY_EVENT_TIMEOUT,
    KEY_EVENT_POPUP,
} gphal_key_event_t;

typedef void (*gphal_key_cb_t)(void *self, gphal_key_event_t event, void *user_data);

typedef struct gphal_key_t
{
    struct gphal_key_t *self;
    gphal_key_cb_t cb;
    void *user_data;
    gphal_state_t state;
    uint32_t keep_ticks;
    uint32_t pressed_ticks;
    int pin;
    int pressed_level;
} gphal_key_t, *gphal_key_handle_t;

gphal_key_handle_t gphal_key_init(uint32_t keep_ticks, int pin, int pressed_level, gphal_key_cb_t cb, void *user_data);
void gphal_key_deinit(gphal_key_handle_t key);
void gphal_key_enable(gphal_key_handle_t key);
void gphal_key_disable(gphal_key_handle_t key);
void gphal_key_start(gphal_key_handle_t key);
void gphal_key_stop(gphal_key_handle_t key);
void gphal_key_handler(gphal_key_handle_t key);
void gphal_key_register_callback(gphal_key_handle_t key, gphal_key_cb_t cb, void *user_data);
void gphal_key_set_keep(gphal_key_handle_t key, uint32_t keep_ticks);
void gphal_key_set_pressed_level(gphal_key_handle_t key, int pressed_level);
void gphal_key_set_pin(gphal_key_handle_t key, int pin);

#ifdef __cplusplus
}
#endif

#endif
#include "key.h"

gphal_key_handle_t gphal_key_init(uint32_t hold_ticks, int pin, int pressed_level, gphal_key_cb_t cb, void *user_data)
{
    gphal_key_handle_t key = NULL;
    key = (gphal_key_handle_t)gphal_malloc(sizeof(gphal_key_t));
    if(key)
    {
        key->self = key;
        key->cb = cb;
        key->user_data = user_data;
        key->state = STATE_INIT;
        key->hold_ticks = hold_ticks;
        key->multi_press_interval_ticks = 0;
        key->pressed_ticks = 0;
        key->multi_press_count = 0;
        key->multi_press_ticks = 0;
        key->key_state = KEY_STATE_UNPRESSED;
        key->pin = pin;
        key->pressed_level = pressed_level;
    }
    return key;
}

void gphal_key_deinit(gphal_key_handle_t key)
{
    assert(key);
    if(key->state == STATE_RUNNUNG) return;
    gphal_free(key);
}

void gphal_key_enable(gphal_key_handle_t key)
{
    assert(key);
    if(key->state != STATE_INIT) return;
    key->pressed_ticks = 0;
    key->multi_press_count = 0;
    key->multi_press_ticks = 0;
    key->key_state = KEY_STATE_UNPRESSED;
    key->state = STATE_READY;
}

void gphal_key_disable(gphal_key_handle_t key)
{
    assert(key);
    if(key->state == STATE_INIT) return;
    key->state = STATE_INIT;
}

void gphal_key_start(gphal_key_handle_t key)
{
    assert(key);
    if(key->state != STATE_READY) return;
    key->state = STATE_RUNNUNG;
}

void gphal_key_stop(gphal_key_handle_t key)
{
    assert(key);
    if(key->state != STATE_RUNNUNG) return;
    key->state = STATE_READY;
}

void gphal_key_handler(gphal_key_handle_t key)
{
    assert(key);
    if(key->state != STATE_RUNNUNG) return;
    if(key->multi_press_count > 0)
    {
        key->multi_press_ticks++;
        if(key->multi_press_ticks > key->multi_press_interval_ticks)
        {
            key->multi_press_count = 0;
            key->multi_press_ticks = 0;
        }
    }
    if(key->key_state == KEY_STATE_UNPRESSED)
    {
        if(gphal_gpio_read(key->pin) == key->pressed_level)
        {
            if(key->pressed_ticks >= GPHAL_KEY_DETECT_TICKS)
            {
                key->key_state = KEY_STATE_PRESSED;
                key->pressed_ticks = 0;
                key->multi_press_count++;
                key->multi_press_ticks = 0;
                if(key->cb)
                {
                    if(key->multi_press_count > 1)
                        key->cb(key, KEY_EVENT_MULTI_PRESS, key->user_data);
                    else
                        key->cb(key, KEY_EVENT_PRESS, key->user_data);
                }
            }
            key->pressed_ticks++;
        }
    }
    else if(key->key_state == KEY_STATE_PRESSED)
    {
        key->pressed_ticks++;
        if(gphal_gpio_read(key->pin) != key->pressed_level)
        {
            key->key_state = KEY_STATE_UNPRESSED;
            if(key->cb)
                key->cb(key, KEY_EVENT_RELEASE, key->user_data);
            key->pressed_ticks = 0;
        }
        else if(key->pressed_ticks == key->hold_ticks && key->cb)
            key->cb(key, KEY_EVENT_LONG_PRESS, key->user_data);
    }
}

void gphal_key_register_callback(gphal_key_handle_t key, gphal_key_cb_t cb, void *user_data)
{
    assert(key);
    if(key->state == STATE_RUNNUNG) return;
    key->cb = cb;
    key->user_data = user_data;
}

void gphal_key_set_hold(gphal_key_handle_t key, uint32_t hold_ticks)
{
    assert(key);
    if(key->state == STATE_RUNNUNG) return;
    key->hold_ticks = hold_ticks;
}

void gphal_key_set_pressed_level(gphal_key_handle_t key, int pressed_level)
{
    assert(key);
    if(key->state == STATE_RUNNUNG) return;
    key->pressed_level = pressed_level;
}

void gphal_key_set_pin(gphal_key_handle_t key, int pin)
{
    assert(key);
    if(key->state == STATE_RUNNUNG) return;
    key->pin = pin;
}

void gphal_key_set_multi_press(gphal_key_handle_t key, uint32_t multi_press_interval_ticks)
{
    assert(key);
    if(key->state == STATE_RUNNUNG) return;
    key->multi_press_interval_ticks = multi_press_interval_ticks;
}

inline uint32_t gphal_key_get_multi_press_count(gphal_key_handle_t key)
{
    return key->multi_press_count;
}
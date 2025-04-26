#include "key.h"

gphal_key_handle_t gphal_key_init(uint32_t keep_ticks, int pin, int pressed_level, gphal_key_cb_t cb, void *user_data)
{
    gphal_key_handle_t key = NULL;
    key = (gphal_key_handle_t)gphal_malloc(sizeof(gphal_key_t));
    if(key)
    {
        key->self = key;
        key->cb = cb;
        key->user_data = user_data;
        key->state = STATE_INIT;
        key->keep_ticks = keep_ticks;
        key->pressed_ticks = 0;
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
    if(key->pressed_ticks == 0)
    {
        if(gphal_gpio_read(key->pin) == key->pressed_level)
        {
            if(key->cb)
                key->cb(key, KEY_EVENT_PRESSED, key->user_data);
            key->pressed_ticks = 1;
        }
    }
    else
    {
        if(!gphal_gpio_read(key->pin) != key->pressed_level)
        {
            if(key->cb)
                key->cb(key, KEY_EVENT_POPUP, key->user_data);
            key->pressed_ticks = 0;
        }
        else if(key->pressed_ticks == key->keep_ticks && key->cb)
            key->cb(key, KEY_EVENT_TIMEOUT, key->user_data);
        key->pressed_ticks++;
    }
}

void gphal_key_register_callback(gphal_key_handle_t key, gphal_key_cb_t cb, void *user_data)
{
    assert(key);
    if(key->state == STATE_RUNNUNG) return;
    key->cb = cb;
    key->user_data = user_data;
}

void gphal_key_set_keep(gphal_key_handle_t key, uint32_t keep_ticks)
{
    assert(key);
    if(key->state == STATE_RUNNUNG) return;
    key->keep_ticks = keep_ticks;
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
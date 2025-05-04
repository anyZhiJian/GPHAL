#ifndef __GPHAL_H
#define __GPHAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#define gphal_malloc                    malloc
#define gphal_free                      free
#define gphal_assert                    assert
#define gphal_gpio_write(pin, level)    0
#define gphal_gpio_read(pin)            0

typedef enum
{
    STATE_INIT,
    STATE_READY,
    STATE_RUNNUNG,
} gphal_state_t;

typedef void (* gphal_cb_t)(void *param);

extern inline void gphal_tick_handler(uint32_t ticks);
extern inline uint32_t gphal_get_ticks(void);

#ifdef __cplusplus
}
#endif

#endif
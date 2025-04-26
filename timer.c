#include "timer.h"

gphal_timer_handle_t gphal_timer_init(uint32_t period_ticks, gphal_timer_cb_t cb, void *user_data)
{
    gphal_timer_handle_t timer = NULL;
    if(!period_ticks || period_ticks > ((uint32_t)-1) / 2 + 1) return timer;
    timer = (gphal_timer_handle_t)gphal_malloc(sizeof(gphal_timer_t));
    if(timer)
    {
        timer->self = timer;
        timer->cb = cb;
        timer->user_data = user_data;
        timer->state = STATE_INIT;
        timer->period_ticks = period_ticks;
        timer->target_ticks = 0;
        timer->remain_ticks = 0;
    }
    return timer;
}

void gphal_timer_deinit(gphal_timer_handle_t timer)
{
    assert(timer);
    if(timer->state == STATE_RUNNUNG) return;
    gphal_free(timer);
}

void gphal_timer_register_callback(gphal_timer_handle_t timer, gphal_timer_cb_t cb, void *user_data)
{
    assert(timer);
    if(timer->state == STATE_RUNNUNG) return;
    timer->cb = cb;
    timer->user_data = user_data;
}

void gphal_timer_set_period(gphal_timer_handle_t timer, uint32_t period_ticks)
{
    assert(timer);
    if(timer->state == STATE_RUNNUNG) return;
    timer->period_ticks = period_ticks;
}

void gphal_timer_enable(gphal_timer_handle_t timer)
{
    assert(timer);
    if(timer->state != STATE_INIT) return;
    timer->remain_ticks = timer->period_ticks;
    timer->state = STATE_READY;
}

void gphal_timer_disable(gphal_timer_handle_t timer)
{
    assert(timer);
    if(timer->state == STATE_RUNNUNG) return;
    timer->state = STATE_INIT;
}

void gphal_timer_start(gphal_timer_handle_t timer)
{
    assert(timer);
    if(timer->state != STATE_READY) return;
    timer->target_ticks = gphal_ticks + timer->remain_ticks;
    timer->state = STATE_RUNNUNG;
}

void gphal_timer_stop(gphal_timer_handle_t timer)
{
    assert(timer);
    if(timer->state != STATE_RUNNUNG) return;
    timer->state = STATE_READY;
}

void gphal_timer_handler(gphal_timer_handle_t timer)
{
    assert(timer);
    if(timer->state != STATE_RUNNUNG) return;
    uint32_t now_ticks = gphal_ticks;
    timer->remain_ticks = timer->target_ticks - now_ticks;
    if((int32_t)(timer->remain_ticks) <= 0)
    {
        timer->target_ticks = now_ticks + timer->period_ticks;
        if(timer->cb)
            timer->cb(timer->self, timer->user_data);
    }
    
}

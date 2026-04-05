#include "include/event_queue.h"


inline void EVENT_Q_init(event_q_t *q)
{
    q->head     = 0;
    q->tail     = 0;
    q->count    = 0;
}
inline bool EVENT_Q_push(event_q_t *q, event_t ev)
{
    if(q->count >= EV_Q_SIZE)
    {
        return false;
    }
    
    q->buf[q->head] = ev;
    q->head = (uint8_t)((q->head + 1) % EV_Q_SIZE);
    q->count++;
    return true;
}
inline bool EVENT_Q_pop(event_q_t *q, event_t *ev)
{
    if(q->count == 0)
    {
        return false;
    }
    *ev = q->buf[q->tail];
    q->tail = (uint8_t)((q->tail + 1) % EV_Q_SIZE);
    q->count--;
    return true;
}
inline bool EVENT_Q_is_empty(const event_q_t *q)
{
    return (q->count == 0);
}
inline bool EVENT_Q_is_full(const event_q_t *q)

{
    return (q->count >= EV_Q_SIZE);
}


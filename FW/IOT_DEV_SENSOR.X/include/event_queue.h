#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "system.h"

#define EV_Q_SIZE 8




typedef struct {
    event_t buf[EV_Q_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
}event_q_t;

inline void EVENT_Q_init(event_q_t *q);
inline bool EVENT_Q_push(event_q_t *q, event_t ev);
inline bool EVENT_Q_pop(event_q_t *q, event_t *ev);
inline bool EVENT_Q_is_empty(const event_q_t *q);
inline bool EVENT_Q_is_full(const event_q_t *e);


#endif

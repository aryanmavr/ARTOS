/*******************************************************************************
 * ARTOS - ARM Real-Time Operating System
 *
 * Author: Aryan
 * Date: 18 May 2025
 *
 * Description: Core RTOS header file defining thread structures and API
 ******************************************************************************/
#ifndef ARTOS_H
#define ARTOS_H

#include <stdint.h>

/* Thread Control Block */
typedef struct {
    void *sp;           /* stack pointer */
    uint32_t timeout;   /* timeout counter */
    uint8_t prio;       /* thread priority */
} OSThread;

typedef void (*OSThreadHandler)();

void OS_init(void *stkSto, uint32_t stkSize);
void OS_onIdle(void);
void OS_sched(void);
void OS_run(void);
void OS_delay(uint32_t ticks);
void OS_tick(void);
void OS_onStartup(void);

void OSThread_start(
    OSThread *me,
    uint8_t prio,
    OSThreadHandler threadHandler,
    void *stkSto, uint32_t stkSize);

#endif /* ARTOS_H */

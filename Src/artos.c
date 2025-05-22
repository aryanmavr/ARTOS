/*******************************************************************************
 * ARTOS - ARM Real-Time Operating System
 *
 * Author: Aryan
 * Date: 18 May 2025
 *
 * Description: Core RTOS implementation with priority-based scheduler
 ******************************************************************************/
#include <stdint.h>
#include "artos.h"
#include "qassert.h"

Q_DEFINE_THIS_FILE

/* ARM Cortex-M System Control Block registers */
#define SCB_SHPR3    ((uint32_t volatile *)0xE000ED20)
#define SCB_ICSR     ((uint32_t volatile *)0xE000ED04)
#define PENDSV_PRIORITY_LOWEST  (0xFFU << 16)
#define PENDSV_TRIGGER          (1U << 28)

OSThread * volatile OS_curr;
OSThread * volatile OS_next;

OSThread *OS_thread[32 + 1];
uint32_t OS_readySet;
uint32_t OS_delayedSet;

#define LOG2(x) (32U - __builtin_clz(x))

OSThread idleThread;
void main_idleThread() {
    while (1) {
        OS_onIdle();
    }
}

void OS_init(void *stkSto, uint32_t stkSize) {
    OS_curr = (OSThread *)0;
    OS_next = (OSThread *)0;
    OS_readySet = 0U;
    OS_delayedSet = 0U;

    for (uint8_t i = 0U; i < Q_DIM(OS_thread); ++i) {
        OS_thread[i] = (OSThread *)0;
    }

    /* Set PendSV to lowest priority */
    *SCB_SHPR3 |= PENDSV_PRIORITY_LOWEST;

    OSThread_start(&idleThread,
                   0U,
                   &main_idleThread,
                   stkSto, stkSize);
}

void OS_sched(void) {
    OSThread *next;
    if (OS_readySet == 0U) {
        next = OS_thread[0];
    }
    else {
        next = OS_thread[LOG2(OS_readySet)];
        Q_ASSERT(next != (OSThread *)0);
    }

    if (next != OS_curr) {
        OS_next = next;
        *SCB_ICSR = PENDSV_TRIGGER;
    }
}

void OS_run(void) {
    OS_onStartup();

    __asm volatile ("cpsid i");
    OS_sched();
    __asm volatile ("cpsie i");

    Q_ERROR();
}

void OS_tick(void) {
    uint32_t workingSet = OS_delayedSet;
    while (workingSet != 0U) {
        OSThread *t = OS_thread[LOG2(workingSet)];
        uint32_t bit;
        Q_ASSERT((t != (OSThread *)0) && (t->timeout != 0U));

        bit = (1U << (t->prio - 1U));
        --t->timeout;
        if (t->timeout == 0U) {
            OS_readySet   |= bit;
            OS_delayedSet &= ~bit;
        }
        workingSet &= ~bit;
    }
}

void OS_delay(uint32_t ticks) {
    uint32_t bit;
    __asm volatile ("cpsid i");

    Q_REQUIRE(OS_curr != OS_thread[0]);

    OS_curr->timeout = ticks;
    bit = (1U << (OS_curr->prio - 1U));
    OS_readySet &= ~bit;
    OS_delayedSet |= bit;
    OS_sched();
    __asm volatile ("cpsie i");
}

void OSThread_start(
    OSThread *me,
    uint8_t prio,
    OSThreadHandler threadHandler,
    void *stkSto, uint32_t stkSize)
{
    uint32_t *sp = (uint32_t *)((((uint32_t)stkSto + stkSize) / 8) * 8);
    uint32_t *stk_limit;

    Q_REQUIRE((prio < Q_DIM(OS_thread))
              && (OS_thread[prio] == (OSThread *)0));

    /* Initialize stack frame */
    *(--sp) = (1U << 24);  /* xPSR */
    *(--sp) = (uint32_t)threadHandler; /* PC */
    *(--sp) = 0x0000000EU; /* LR */
    *(--sp) = 0x0000000CU; /* R12 */
    *(--sp) = 0x00000003U; /* R3 */
    *(--sp) = 0x00000002U; /* R2 */
    *(--sp) = 0x00000001U; /* R1 */
    *(--sp) = 0x00000000U; /* R0 */
    *(--sp) = 0x0000000BU; /* R11 */
    *(--sp) = 0x0000000AU; /* R10 */
    *(--sp) = 0x00000009U; /* R9 */
    *(--sp) = 0x00000008U; /* R8 */
    *(--sp) = 0x00000007U; /* R7 */
    *(--sp) = 0x00000006U; /* R6 */
    *(--sp) = 0x00000005U; /* R5 */
    *(--sp) = 0x00000004U; /* R4 */

    me->sp = sp;
    me->timeout = 0U;

    stk_limit = (uint32_t *)(((((uint32_t)stkSto - 1U) / 8) + 1U) * 8);

    for (sp = sp - 1U; sp >= stk_limit; --sp) {
        *sp = 0xDEADBEEFU;
    }

    OS_thread[prio] = me;
    me->prio = prio;
    if (prio > 0U) {
        OS_readySet |= (1U << (prio - 1U));
    }
}

__attribute__ ((naked))
void PendSV_Handler(void) {
__asm volatile (
    "  CPSID         I                 \n"

    "  LDR           r1,=OS_curr       \n"
    "  LDR           r1,[r1,#0x00]     \n"
    "  CMP           r1,#0             \n"
    "  BEQ           PendSV_restore    \n"

    "  PUSH          {r4-r11}          \n"

    "  LDR           r1,=OS_curr       \n"
    "  LDR           r1,[r1,#0x00]     \n"
    "  MOV           r0,sp             \n"
    "  STR           r0,[r1,#0x00]     \n"

    "PendSV_restore:                   \n"
    "  LDR           r1,=OS_next       \n"
    "  LDR           r1,[r1,#0x00]     \n"
    "  LDR           r0,[r1,#0x00]     \n"
    "  MOV           sp,r0             \n"

    "  LDR           r1,=OS_next       \n"
    "  LDR           r1,[r1,#0x00]     \n"
    "  LDR           r2,=OS_curr       \n"
    "  STR           r1,[r2,#0x00]     \n"

    "  POP           {r4-r11}          \n"

    "  CPSIE         I                 \n"

    "  BX            lr                \n"
    );
}

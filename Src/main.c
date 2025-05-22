/*******************************************************************************
 * ARTOS - ARM Real-Time Operating System
 *
 * Author: Aryan
 * Date: 18 May 2025
 *
 * Description: Main application demonstrating multi-threaded LED control
 ******************************************************************************/
#include <stdint.h>
#include "artos.h"
#include "bsp.h"

uint32_t stack_blinky1[64];  /* Increased stack size for safety */
OSThread blinky1;
void main_blinky1() {
    while (1) {
        BSP_ledGreenOn();
        OS_delay(BSP_TICKS_PER_SEC / 4U);
        BSP_ledGreenOff();
        OS_delay(BSP_TICKS_PER_SEC / 4U);
    }
}

uint32_t stack_blinky2[64];
OSThread blinky2;
void main_blinky2() {
    while (1) {
        BSP_ledBlueOn();
        OS_delay(BSP_TICKS_PER_SEC / 2U);
        BSP_ledBlueOff();
        OS_delay(BSP_TICKS_PER_SEC / 2U);
    }
}

uint32_t stack_blinky3[64];
OSThread blinky3;
void main_blinky3() {
    while (1) {
        BSP_ledRedOn();
        OS_delay(BSP_TICKS_PER_SEC);
        BSP_ledRedOff();
        OS_delay(BSP_TICKS_PER_SEC);
    }
}

uint32_t stack_idleThread[64];

int main() {
    BSP_init();
    OS_init(stack_idleThread, sizeof(stack_idleThread));

    OSThread_start(&blinky1, 5U, &main_blinky1,
                   stack_blinky1, sizeof(stack_blinky1));

    OSThread_start(&blinky2, 2U, &main_blinky2,
                   stack_blinky2, sizeof(stack_blinky2));

    OSThread_start(&blinky3, 1U, &main_blinky3,
                   stack_blinky3, sizeof(stack_blinky3));

    OS_run();
    return 0;
}

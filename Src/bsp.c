/*******************************************************************************
 * ARTOS - ARM Real-Time Operating System
 *
 * Author: Aryan
 * Date: 18 May 2025
 *
 * Description: Board Support Package for STM32F446RE with 180MHz operation
 ******************************************************************************/
#include <stdint.h>

#include "bsp.h"
#include "artos.h"
#include "qassert.h"
#include "stm32f4xx.h"

/* External LED connections */
#define LED_RED   (1U << 7)  /* PA7 */
#define LED_BLUE  (1U << 8)  /* PA8 */
#define LED_GREEN (1U << 9)  /* PA9 */

void SystemClock_Config(void) {
    /* Enable power control clock */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    /* Set voltage scaling for maximum performance */
    PWR->CR |= PWR_CR_VOS;

    /* Enable HSE and wait for ready */
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0);

    /* Configure PLL: 8MHz HSE -> 180MHz SYSCLK */
    RCC->PLLCFGR = (
        (4U << RCC_PLLCFGR_PLLM_Pos) |    /* PLLM = 4 */
        (180U << RCC_PLLCFGR_PLLN_Pos) |  /* PLLN = 180 */
        (0U << RCC_PLLCFGR_PLLP_Pos) |    /* PLLP = 2 */
        (7U << RCC_PLLCFGR_PLLQ_Pos) |    /* PLLQ = 7 */
        RCC_PLLCFGR_PLLSRC_HSE
    );

    /* Enable PLL and wait for ready */
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);

    /* Configure Flash for 180MHz operation */
    FLASH->ACR = (
        FLASH_ACR_ICEN |
        FLASH_ACR_DCEN |
        FLASH_ACR_PRFTEN |
        (5U << FLASH_ACR_LATENCY_Pos)
    );

    /* Configure bus prescalers */
    RCC->CFGR = (
        RCC_CFGR_HPRE_DIV1 |    /* AHB = 180MHz */
        RCC_CFGR_PPRE1_DIV4 |   /* APB1 = 45MHz */
        RCC_CFGR_PPRE2_DIV2     /* APB2 = 90MHz */
    );

    /* Select PLL as system clock */
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    /* Update system clock variable */
    SystemCoreClock = 180000000UL;
}

void SysTick_Handler(void) {
    OS_tick();
    __disable_irq();
    OS_sched();
    __enable_irq();
}

void BSP_init(void) {
    SystemClock_Config();

    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Configure pins as outputs */
    GPIOA->MODER &= ~(GPIO_MODER_MODER7_Msk | GPIO_MODER_MODER8_Msk | GPIO_MODER_MODER9_Msk);
    GPIOA->MODER |= (GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);

    /* Configure as push-pull */
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT7 | GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);

    /* Set medium speed */
    GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED7_0 | GPIO_OSPEEDR_OSPEED8_0 | GPIO_OSPEEDR_OSPEED9_0);

    /* No pull resistors */
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD7_Msk | GPIO_PUPDR_PUPD8_Msk | GPIO_PUPDR_PUPD9_Msk);
}

void BSP_ledRedOn(void) {
    GPIOA->BSRR = LED_RED;
}

void BSP_ledRedOff(void) {
    GPIOA->BSRR = (LED_RED << 16);
}

void BSP_ledBlueOn(void) {
    GPIOA->BSRR = LED_BLUE;
}

void BSP_ledBlueOff(void) {
    GPIOA->BSRR = (LED_BLUE << 16);
}

void BSP_ledGreenOn(void) {
    GPIOA->BSRR = LED_GREEN;
}

void BSP_ledGreenOff(void) {
    GPIOA->BSRR = (LED_GREEN << 16);
}

void OS_onStartup(void) {
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);
    NVIC_SetPriority(SysTick_IRQn, 0U);
}

void OS_onIdle(void) {
    __NOP();
}

_Noreturn void Q_onAssert(char const * module, int_t location) {
    (void)module;
    (void)location;
#ifndef NDEBUG
    GPIOA->BSRR = (LED_RED | LED_BLUE | LED_GREEN);
    for (;;) {
    }
#endif
    NVIC_SystemReset();
}

_Noreturn void assert_failed(char const * module, int_t location) {
    Q_onAssert(module, location);
}

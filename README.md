# ARTOS - ARM Real-Time Operating System

A lightweight RTOS implementation built from scratch for ARM Cortex-M processors, demonstrated on the STM32F446RE microcontroller.

## Features

- Preemptive priority-based scheduler
- Context switching using PendSV interrupt
- Thread synchronization with OS_delay()
- Supports up to 32 concurrent threads

## Hardware Setup

**Required:**
- STM32F446RE Nucleo Board
- 3 LEDs with 220Ω resistors(optional)

**Connections:**
- PA7 → Red LED → GND
- PA8 → Blue LED → GND
- PA9 → Green LED → GND

## Build and Run

1. Open project in STM32CubeIDE
2. Build and flash to STM32F446RE
3. Connect LEDs as shown above
4. Reset board - all 3 LEDs should blink at different rates

## Demo Application

Three threads demonstrate preemptive multitasking:

| Thread | LED | Period | Priority |
|--------|-----|--------|----------|
| blinky1 | Green | 500ms (250ms on/250ms off) | 5 (High) |
| blinky2 | Blue | 1000ms (500ms on/500ms off) | 2 (Medium) |
| blinky3 | Red | 2000ms (1000ms on/1000ms off) | 1 (Low) |

## Expected Behavior

When the project is built and running correctly:

- **Green LED**: Fast blinking - ON for 250ms, OFF for 250ms (highest priority thread)
- **Blue LED**: Medium blinking - ON for 500ms, OFF for 500ms (medium priority thread)
- **Red LED**: Slow blinking - ON for 1000ms, OFF for 1000ms (lowest priority thread)

All three LEDs will blink **simultaneously and independently** - demonstrating that the RTOS is successfully running multiple threads concurrently. The different blink rates show the priority-based scheduling in action.

## System Architecture

ARTOS follows a layered architecture:

**Application Layer (main.c)**
- Thread definitions and application logic
- User-defined thread functions

**RTOS Kernel (artos.c/h)**
- Priority-based scheduler
- Context switching via PendSV interrupt
- Thread management and synchronization

**Hardware Abstraction (bsp.c/h)**
- STM32F446RE board support
- GPIO control and system clock configuration
- Hardware-specific initialization

## Technical Details

- **Target:** STM32F446RE (ARM Cortex-M4)
- **System Tick:** 1ms resolution
- **Stack Size:** 256 bytes per thread
- **Scheduling:** Priority-based preemptive

## API Example

```c
// Initialize RTOS
OS_init(stack_idleThread, sizeof(stack_idleThread));

// Create a thread
OSThread_start(&blinky1, 5U, &main_blinky1,
               stack_blinky1, sizeof(stack_blinky1));

// Start scheduler
OS_run();

// Thread function
void main_blinky1() {
    while (1) {
        BSP_ledGreenOn();
        OS_delay(250);  // 250ms
        BSP_ledGreenOff();
        OS_delay(250);
    }
}
```

## License

Open source project.

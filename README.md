# ARTOS - ARM Real-Time Operating System

A minimal RTOS implementation for ARM Cortex-M processors, specifically designed for the STM32F446RE microcontroller.

## Features

- **Preemptive Priority-Based Scheduler**: Higher priority threads can interrupt lower priority ones
- **Context Switching via PendSV**: Efficient ARM Cortex-M context switching mechanism  
- **Thread Synchronization**: OS_delay() function for precise timing control
- **Stack Overflow Protection**: Stack areas pre-filled with 0xDEADBEEF for debugging
- **High Performance**: Optimized for 180MHz operation on STM32F446RE
- **Minimal Footprint**: 2.58KB ROM 

## Hardware Requirements

- **STM32F446RE Nucleo Board**
- **3 External LEDs** connected to:
  - PA7 (Red LED) 
  - PA8 (Blue LED)
  - PA9 (Green LED)

## System Architecture

**Three-Layer Design:**

1. **Application Layer** (main.c)
   - Thread management and application logic
   - Three demonstration threads with different priorities

2. **ARTOS Core** (artos.c) 
   - Priority-based scheduler
   - Context switching via PendSV interrupt
   - Thread synchronization and timing

3. **Hardware Layer** (bsp.c)
   - STM32F446RE board support
   - GPIO control for LED demonstration
   - System clock configuration (180MHz)


## Thread Demonstration

The demo application showcases three concurrent threads with different priorities:

| Thread | LED Color | Blink Rate | Priority | Purpose |
|--------|-----------|------------|----------|---------|
| blinky1 | Green | 250ms on/off | 5 (High) | Demonstrates high-priority execution |
| blinky2 | Blue | 500ms on/off | 2 (Medium) | Shows medium-priority scheduling |
| blinky3 | Red | 1000ms on/off | 1 (Low) | Illustrates low-priority background task |

## Performance Metrics

- **System Clock**: 180MHz (maximum for STM32F446RE)
- **System Tick**: 1ms resolution
- **Thread Stack Size**: 256 bytes each
- **Maximum Threads**: 32 concurrent threads supported

## Expected Behavior

When running correctly, you should observe:
- **Green LED**: Fast blinking - highest priority
- **Blue LED**: Medium blinking - medium priority  
- **Red LED**: Slow blinking - lowest priority
- All LEDs run **simultaneously** without interfering with each other

## Code Structure

ARTOS/
- src/
  - artos.c/h (Core RTOS implementation)
  - bsp.c/h (Board Support Package)  
  - main.c (Application threads)
  - qassert.h (Debug assertions)
  - syscalls.c (System call stubs)
- README.md
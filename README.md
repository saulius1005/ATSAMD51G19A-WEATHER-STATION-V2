
# ATSAMD51 ILI9341 LCD Demo with DMA and SPI
## Wiring Diagram

<img width="2075" height="1331" alt="schema SPI" src="https://github.com/user-attachments/assets/2e3d5340-a2ca-4e16-99ed-1fdddd39b560" />


## Overview

This repository contains a minimal embedded project for the **ATSAMD51G19A** microcontroller, demonstrating:

- Driving a **320x240 ILI9341 LCD** via **SPI**
- Using both **CPU-driven** and **DMA-driven** frame buffer transfers
- Efficient **32-bit SPI packed transfers**
- Low-level **GPIO and SERCOM initialization**
- DMA descriptor chaining for large frame buffers and color fills

The project is intended as a learning and testing platform for high-performance LCD control on ARM Cortex-M0+ / M4-class MCUs.

---

## Features

- **LCD framebuffer drawing**
  - CPU-only method
  - DMA-assisted method for minimal CPU usage
- **Full-screen color fill**
  - Both CPU and DMA methods
- **Hardware abstraction**
  - Direct GPIO control for DC and RESET
  - SERCOM SPI configured for 32-bit transfers
  - DMA descriptors aligned and chained for efficient large transfers
- **Flexible clock setup**
  - 2 MHz TCXO reference
  - 128 MHz DPLL core clock configuration

---

## Supported Hardware

- Microcontroller: **ATSAMD51G19A**
- LCD: **ILI9341 320x240** (SPI interface)
- GPIO connections:
  - PA05: SPI SCK
  - PA06: SPI CS
  - PA07: SPI MOSI
  - PA08: LCD RESET
  - PA09: LCD DC
- Optional: External 24Mhz TCXO oscillator (for precise 2 MHz reference)

---

Note: This is unofficial code and might have some issues.

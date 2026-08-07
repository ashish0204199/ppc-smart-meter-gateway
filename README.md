# MeterBridge-FW

A firmware-focused STM32 energy telemetry and load-control system
with a Python interoperability and fault-injection testbench.

## Objective

The STM32 NUCLEO-G474RE firmware will:

- Simulate voltage and current measurements
- Calculate power and accumulated energy
- Transmit telemetry using UART
- Process load-control commands
- Detect communication errors
- Enter a safe state after communication timeout

Python will receive and store telemetry, transmit commands and
perform automated fault-injection tests.

## Technology

- STM32 NUCLEO-G474RE
- Embedded C
- STM32 HAL
- UART
- Python and pySerial
- pytest
- SQLite
- Git

## Project status

Development started on 7 August 2026.

This is an educational interoperability demonstrator. It is not a
certified Smart Meter Gateway or production electricity meter.

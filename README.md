# serial_STWBC2

An open source, header-only library to analyze serial data from the STWBC2 Qi transmitter.

## Overview

The **serial_STWBC2** library is designed to simplify the parsing and analysis of serial messages sent by the STWBC2 Qi transmitter. It is implemented in C++ and encapsulated in a single header file, making it easy to integrate into your project.

This library:
- Is **header-only** – simply include the header file in your project.
- Provides a **Data** structure that holds key parameters such as:
  - **state** – current state of the transmitter.
  - **frequency** – operating frequency.
  - **controlError** – control error value.
  - **dutyCycle** – duty cycle percentage.
  - **bridgeVoltage** – voltage at the bridge.
  - **rxPower** – received power.
  - **inputVoltage** – input voltage.
- Implements helper functions to resolve the various fields from the raw serial data.
- Uses a simple loop-based mechanism to collect and process serial messages.

## Features

- **Header-Only Library:** No need to compile or link additional files.
- **Simple API:** Initialize with a serial port and call the loop function to continuously process incoming data.
- **Customizable:** Designed to work with any platform supporting a `Stream` class (commonly used in Arduino environments).

## Installation

1. **Download the Library:**
   - Clone the repository:
     ```bash
     git clone https://github.com/KochC/serial_STWBC2.git
     ```
   - Or download the ZIP file from the repository page and extract it.

2. **Include in Your Project:**
   - Copy the `include/stwbc2.h` file into your project.
   - In your source file, include the header:
     ```cpp
     #include "stwbc2.h"
     ```

## Usage

Below is an example of how to use the library in your project:

```cpp
#include "stwbc2.h"

// Assume 'Serial' is your instance of Stream (e.g., on Arduino)
void setup() {
  // Initialize the serial communication, if needed
  Serial.begin(9600);

  // Initialize the STWBC2 library with your serial port
  STWBC2::init(&Serial);
}

void loop() {
  // Process incoming serial data
  STWBC2::loop();

  // Use the parsed data from the STWBC2::data structure
  // For example, print the frequency:
  Serial.print("Frequency: ");
  Serial.println(STWBC2::data.frequency);
}
```

## API Reference
### Initialization
 - `void STWBC2::init(Stream *serialPort)`  
   Set the serial port from which the STWBC2 data will be read.
### Data Processing
 - `void STWBC2::loop()`  
   Continuously read incoming serial data, build messages, and parse them into the `STWBC2::data` structure.

### Data Structure
 - `STWBC2::Data`  
   Holds the parsed data with the following fields:
   - `uint8_t state`
   - `uint32_t frequency`
   - `uint16_t controlError`
   - `double dutyCycle`
   - `double bridgeVoltage`
   - `double rxPower`
   - `double inputVoltage`

### Contributing
Contributions to improve the library or add new features are welcome! Feel free to fork the repository and submit pull requests.

### License
This project is licensed under the Apache-2.0 License. See the LICENSE file for details.

### Keywords
qi, serial, st, pd, power-deliver, qi-charging, charging, wireless-charging



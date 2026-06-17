# Speaker Controller - Technical Overview

## Project Summary

Speaker Controller is a portable smart speaker platform that combines traditional analog audio controls with modern wireless control and visualization capabilities.

The system uses an Arduino UNO R4 WiFi to host a web interface that allows users to remotely control amplifier settings from a phone, tablet, or computer. Instead of electronically modifying amplifier circuitry, servo motors physically rotate the amplifier's knobs, preserving the amplifier's original analog signal path.

The project is designed around the principle that physical controls and digital controls should always remain synchronized.

---

## Core Concept

The amplifier is a standalone external analog amplifier.

The embedded system never directly processes audio and is not electrically connected to the amplifier's audio circuitry.

Instead:

- Users adjust settings through a web interface.
- The Arduino updates the desired state.
- Servo motors rotate the amplifier knobs.
- Physical knob positions always represent the actual system state.

Likewise:

- Users may manually adjust the knobs.
- Potentiometer values are read by the controller.
- The web interface is updated accordingly.

This creates a bidirectional synchronization system.

---

## System Architecture

```text
Phone / Tablet / PC
         │
         ▼
   Web Interface
         │
         ▼
 Arduino UNO R4 WiFi
         │
         ▼
 Hardware Control Layer
         │
 ┌───────┼────────┐
 ▼       ▼        ▼
Servos  LEDs  Potentiometers
         │
         ▼
 External Amplifier
         │
         ▼
      Speakers
```

---

## Hardware Overview

### Main Controller

#### Arduino UNO R4 WiFi

Responsibilities:

- Wi-Fi Access Point
- Embedded web server
- API handling
- State management
- Hardware coordination

---

### Planned Secondary Controller

#### Seeed Studio XIAO ESP32-S3

Planned responsibilities:

- Servo control
- Potentiometer monitoring
- LED effects
- Hardware timing tasks
- Future animation engine

Communication with the UNO R4 will occur through UART (TX/RX).

---

## Audio Control System

### Amplifier

The amplifier remains fully analog.

The embedded system acts only as a mechanical control layer.

Benefits:

- No modification of amplifier electronics
- Preserves sound characteristics
- Maintains compatibility with many amplifier types
- Allows manual and remote control simultaneously

---

### Servo Motors

Four servos are used to rotate amplifier controls.

Current channels:

| Function | Servo |
|-----------|--------|
| Microphone | Servo 1 |
| Treble | Servo 2 |
| Bass | Servo 3 |
| Volume | Servo 4 |

Responsibilities:

- Rotate knobs
- Maintain synchronization
- Restore saved settings

---

### Potentiometers

Motorized potentiometers serve as both:

- User input devices
- Position feedback devices

This allows users to:

- Turn controls manually
- Adjust controls remotely
- Maintain synchronized values

---

## Lighting System

### LED Strip

Planned uses:

- Audio visualization
- Status indicators
- Volume display
- Bass display
- Treble display
- Startup effects

### LED Matrix

Planned uses:

- Visualizer animations
- System status information
- Audio level displays
- Future graphical effects

---

## Audio Visualization

### Microphone Module

An internal microphone captures ambient music from the speaker enclosure.

Planned processing:

- Audio level detection
- Peak detection
- Beat detection
- Frequency visualization

Output devices:

- LED strip
- LED matrix

---

## Power System

The current design includes multiple buck converters.

Purpose:

- Separate power rails
- Stable servo power delivery
- LED power management
- Controller power regulation

Benefits:

- Reduced noise
- Improved stability
- Better scalability

---

## Communication Architecture

### Current

```text
Browser
   │
HTTP
   │
   ▼
UNO R4 WiFi
   │
   ▼
Servos
```

### Planned

```text
Browser
   │
HTTP
   │
   ▼
UNO R4 WiFi
   │
UART
   │
   ▼
XIAO ESP32-S3
   │
 ┌─┼─┬─┐
 ▼ ▼ ▼ ▼
LEDs Pots Servos
```

---

## Software Stack

### Firmware

- C++
- Arduino Framework
- PlatformIO

### Web Interface

- HTML
- CSS
- JavaScript

### Communication

- Wi-Fi Access Point
- HTTP API
- UART (planned)

---

## Software Architecture

The project follows a layered architecture where hardware-specific code is separated from application logic.

```text
Application Layer
       │
       ▼
Controller Layer
       │
       ▼
Driver Layer
       │
       ▼
Hardware
```

This separation ensures that business logic remains independent of the hardware implementation.

---

## Source Structure

```text
src/
├── main.cpp
│
├── controller/
│   ├── AudioController.*
│   ├── WebController.*
│   └── ...
│
├── driver/
│   ├── ServoDriver.*
│   ├── PotentiometerDriver.*
│   ├── LedDriver.*
│   ├── WifiApServer.*
│   └── ...
│
├── generated/
│   └── WebAssets.h
│
└── util/
    └── Shared helper utilities
```

---

## Driver Layer

The driver layer is responsible for direct interaction with hardware.

Drivers should only know how to communicate with a specific device and should not contain application logic.

Examples:

### ServoDriver

Responsibilities:

- Configure servo hardware
- Move servos to target positions
- Read servo state

### PotentiometerDriver

Responsibilities:

- Read analog values
- Convert ADC values
- Apply filtering

### LedDriver

Responsibilities:

- Control LED strip
- Control LED matrix
- Update pixel data

### WifiApServer

Responsibilities:

- Create access point
- Handle HTTP requests
- Manage network connections

A driver should never know why a device is being used.

---

## Controller Layer

Controllers coordinate multiple drivers to implement application behaviour.

Controllers contain system logic and business rules.

Examples:

### AudioController

Coordinates:

- ServoDriver
- PotentiometerDriver

Responsibilities:

- Synchronize physical and remote controls
- Maintain audio settings state
- Restore saved values
- Process user adjustments

### WebController

Coordinates:

- WifiApServer
- AudioController

Responsibilities:

- Process API requests
- Validate user input
- Generate JSON responses
- Expose system state

Controllers decide what should happen.

Drivers decide how it happens.

---

## Example Flow

When a user changes volume from the web interface:

```text
Browser
   │
   ▼
WebController
   │
   ▼
AudioController
   │
   ▼
ServoDriver
   │
   ▼
Servo Motor
```

When a user rotates a physical knob:

```text
PotentiometerDriver
        │
        ▼
AudioController
        │
        ▼
WebController
        │
        ▼
Web Interface Update
```

---

## Design Principles

### Separation of Concerns

Controllers contain application logic.

Drivers contain hardware logic.

### Single Responsibility

Each driver controls one hardware subsystem.

Each controller manages one domain of functionality.

### Hardware Independence

Application logic should remain unchanged if hardware is replaced.

For example:

```text
AudioController
       │
       ▼
ServoDriver
```

could later become:

```text
AudioController
       │
       ▼
MotorDriver
```

without modifying the AudioController itself.

### Scalability

Future controllers can be added without modifying existing hardware drivers.

Examples:

- VisualizerController
- SettingsController
- CommunicationController
- DiagnosticsController
---

## Current Features

- Wi-Fi Access Point
- Embedded web interface
- Servo-based amplifier control
- Remote volume control
- Remote bass control
- Remote treble control
- Remote microphone control
- Physical control synchronization

---

## Planned Features

### Persistence

- Save settings to memory
- Restore settings after reboot

### LED Feedback

Temporary display of:

- Volume level
- Bass level
- Treble level
- Microphone level

before returning to visualization mode.

### Music Visualization

- Audio reactive LEDs
- Peak indicators
- Beat detection
- Matrix animations

### Mobile Improvements

- Responsive UI
- Dark mode
- Progressive Web App support

### Maintenance

- OTA firmware updates
- Diagnostics
- Configuration management

---

## Development Roadmap

### Phase 1

- Servo control
- Web interface
- Wi-Fi networking

### Phase 2

- Persistent settings
- LED integration
- Audio visualization

### Phase 3

- XIAO ESP32-S3 integration
- UART communication
- Hardware abstraction improvements

### Phase 4

- OTA updates
- Authentication
- Advanced animations
- Expanded diagnostics

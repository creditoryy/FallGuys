# FallGuys

ENSC 351 Project - Fall 2025

## Description
ESP32-based project using PlatformIO for embedded development.

## Hardware Requirements
- ESP32 Development Board
- USB Cable for programming and power

## Software Requirements
- Python 3.x
- PlatformIO Core

## Installation

1. **Install PlatformIO:**
   ```bash
   pip install platformio
   ```

2. **Clone the repository:**
   ```bash
   git clone https://github.com/creditoryy/FallGuys.git
   cd FallGuys
   ```

3. **Install project dependencies:**
   ```bash
   pio pkg install
   ```

## Usage

### Build the project:
```bash
pio run
```

### Upload to ESP32:
1. Connect your ESP32 to your computer via USB
2. Run:
   ```bash
   pio run --target upload
   ```

### Monitor serial output:
```bash
pio device monitor
```

### Build + Upload + Monitor (all in one):
```bash
pio run --target upload && pio device monitor
```

## Project Structure
```
FallGuys/
├── src/           # Source files (.cpp)
├── include/       # Header files (.h)
├── lib/           # Project libraries
├── platformio.ini # PlatformIO configuration
└── README.md
```

## Contributors
[Add team members here]

## License
[Add license information here]

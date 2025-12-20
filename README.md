# Nova_Code

[![Latest Tag](https://img.shields.io/github/v/tag/VinciEcoDrive/Nova_Code)](https://github.com/VinciEcoDrive/Nova_Code/tags)
![CI Build Status](https://github.com/VinciEcoDrive/Nova_Code/actions/workflows/build_nova_code.yml/badge.svg)

This repo contains the source code of our Efficiency Prototype NOVA competing at the [Shell Eco Marathon](https://www.shellecomarathon.com/).

**Target Hardware:** ESP32 (DOIT DevKit V1)  
**Framework:** Arduino via PlatformIO

## Prerequisites

1. [VSCode](https://code.visualstudio.com/) 
2. [PlatformIO Extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)
3. [ESP32 Drivers](https://www.silabs.com/software-and-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads)

*Note: PlatformIO will automatically download the required libraries and toolchains when you open the project for the first time.*

## Usage

### 1. Getting Started

1. Clone the repository:
```bash
git clone https://github.com/VinciEcoDrive/Nova_Code.git
```

2. Open the folder in VS Code. Wait a moment for PlatformIO to initialize the environment and download dependencies.

### 2. Building and Flashing

1. Connect your ESP32 board to your computer via USB.
2. Click on the **PlatformIO Alien Icon** in the VS Code sidebar.
3. Navigate to `Project Tasks` -> `esp32doit-devkit-v1`.
4. Choose your action:
   - Build: Compiles the code to check for errors.
   - Upload: Compiles and flashes the binary to the board.
   - Monitor: Opens the Serial Monitor (115200 baud).

## CI & Artifacts

This project uses GitHub Actions for Continuous Integration to automatically compile the firmware.

### Versioning & Artifacts
The CI pipeline automatically injects the version number into the firmware:
- **Tagged Builds:** The firmware filename will be `firmware-vXX.X.X.bin` (matching the tag).
- **Test Builds:** For regular commits or PRs, the filename will be `firmware-test-build.bin`.

### Downloading the Firmware
To download the compiled binary without setting up the development environment:
1. Go to the [Actions Tab](https://github.com/VinciEcoDrive/Nova_Code/actions).
2. Click on the latest successful workflow run.
3. Scroll down to the **Artifacts** section.
4. Download the `firmware-VERSION` zip file (contains the `.bin` file).

## Troubleshooting

- If PlatformIO doesn't detect the project, try refreshing the workspace.
- Ensure the ESP32 drivers are installed on your system.
- Check that all library dependencies are properly downloaded.

## Contributing

We welcome contributions from the team!
Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code style, the CI/CD pipeline, and our Versioning Strategy `(YEAR.x.x)`.

Please ensure your code follows the project's style guidelines and includes appropriate tests.


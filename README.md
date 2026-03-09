# Nova_Code

[![Latest Tag](https://img.shields.io/github/v/tag/VinciEcoDrive/Nova_Code)](https://github.com/VinciEcoDrive/Nova_Code/tags)
![CI Build Status](https://github.com/VinciEcoDrive/Nova_Code/actions/workflows/ci.yml/badge.svg)

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

This project uses GitHub Actions for both Continuous Integration and firmware releases.

### Workflows
- **CI (`ci.yml`)** runs on Pull Requests and pushes to `main`/`master`.
- **Release (`release_nova_code.yml`)** runs only when a tag matching `v*` is pushed.

### Versioning & Output Files
- **CI builds:** Produce a validation artifact named `firmware-ci-<branch>-<date>-<sha>.bin`.
- **Tagged releases:** Produce `firmware-v<YY>.<MINOR>.<PATCH>.bin` and publish it to GitHub Releases.

### Downloading Firmware
Use one of the following depending on your goal:
1. **Production firmware:** Go to [Releases](https://github.com/VinciEcoDrive/Nova_Code/releases) and download the `.bin` asset directly.
2. **CI validation firmware:** Go to the [Actions Tab](https://github.com/VinciEcoDrive/Nova_Code/actions) and download the artifact from a workflow run (GitHub provides artifacts as `.zip`).

## Troubleshooting

- If PlatformIO doesn't detect the project, try refreshing the workspace.
- Ensure the ESP32 drivers are installed on your system.
- Check that all library dependencies are properly downloaded.

## Contributing

We welcome contributions from the team!
Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code style, the CI/CD pipeline, and our Versioning Strategy `(YEAR.x.x)`.

Please ensure your code follows the project's style guidelines and includes appropriate tests.


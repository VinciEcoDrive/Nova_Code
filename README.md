# Code_PE_2026


This repo contains the source code of our Efficiency Prototype competing on the [Shell Eco Marathon](https://www.shellecomarathon.com/).

## Prerequisites

1. [VSCode](https://code.visualstudio.com/) 
2. [PlatformIO Extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)
3. [ESP32 Drivers](https://www.silabs.com/software-and-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads)

PIO will automatically download the required library.

## Usage

### Getting Started


1. Clone the repository:
```bash
git clone https://github.com/VinciEcoDrive/Code_PE_2026.git
```

2. Open the folder in VS Code. PlatformIO should automatically detect the project and initialize the environment.

### Building and Flashing

1.  Connect your ESP32 board to your computer.
2.  Click on the PlatformIO icon in the sidebar.
3.  Under `Project Tasks`, select `env:esp32doit-devkit-v1`.
4.  Click `Build` to compile the code.
5.  Click `Upload` to flash the code to the board.

## Troubleshooting

- If PlatformIO doesn't detect the project, try refreshing the workspace.
- Ensure the ESP32 drivers are installed on your system.
- Check that all library dependencies are properly downloaded.

## Contributing

We welcome contributions! Please follow these steps:

1. Create a feature branch (`git checkout -b feature/YourFeature`).
2. Commit your changes (`git commit -m 'Add YourFeature'`).
3. Push to the branch (`git push origin feature/YourFeature`).
4. Open a Pull Request.

Please ensure your code follows the project's style guidelines and includes appropriate tests.


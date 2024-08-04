# Score Badminton

This project uses an ESP32 DevKit V1 and an LED matrix MAX7219 to display badminton scores in real-time.

## Table of Contents
1. [Project Description](#project-description)
2. [Features](#features)
3. [Requirements](#requirements)
4. [Installation](#installation)
5. [Usage](#usage)
6. [Documentation Video](#documentation-video)
7. [Contribution](#contribution)
8. [License](#license)

## Project Description
This project aims to create a scoreboard system for badminton games using an ESP32 DevKit V1 and an LED matrix MAX7219. The system allows users to monitor scores in real-time through a clear and readable LED display.

## Features
- Display real-time badminton scores
- Score adjustment via web interface
- Clear and bright LED matrix display

## Requirements
- ESP32 DevKit V1
- LED matrix MAX7219
- Jumper wires
- Breadboard
- Arduino IDE software

## Installation
1. Clone this repository:
    ```bash
    git clone https://github.com/username/score-badminton.git
    ```
2. Install the required libraries in the Arduino IDE:
    - `MD_Parola`
    - `MD_MAX72XX`
    - `SPI`
    - `WiFi`
    - `WebServer`

3. Connect the hardware according to the pin diagram below:
   
   ![Pin Diagram](image/pin.png)

4. Open the `.ino` file in the Arduino IDE and upload it to the ESP32 DevKit V1.

## Usage
1. Power on the ESP32 DevKit V1.
2. Connect to the Wi-Fi network with the credentials specified in the code.
3. Open a web browser and navigate to the IP address displayed on the LED matrix.
4. Use the web interface to adjust the scores.

## Documentation Video
A video demonstration of the project is available [here](video/video.mp4).

## Contribution
We welcome contributions from everyone. Please create a pull request or submit an issue to fix bugs or add new features.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Code Explanation
Here is a brief explanation of the main parts of the code:

### Libraries
```cpp
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <WiFi.h>
#include <WebServer.h>
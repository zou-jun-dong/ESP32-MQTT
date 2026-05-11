# ESP32-S3 Robust MQTT Node

This project is a robust IoT node built on the ESP32-S3 microcontroller using the MQTT protocol. It features bidirectional communication, enabling real-time telemetry data upload and remote downlink control.

## Key Features
* **Non-blocking Architecture:** Utilizes a `millis()`-based time interval mechanism to avert blocking the main loop, ensuring continuous system responsiveness.
* **Structured Data Transmission:** Formats and publishes simulated temperature data as JSON strings for standardized data exchange.
* **Bidirectional Control:** Implements callback functions to receive downlink commands (e.g., toggling an onboard LED) via subscribed topics.
* **Network Self-healing:** Features an autonomous state machine with a cooldown timer to automatically recover from Wi-Fi or MQTT disconnections without serial flooding.

## Environment & Dependencies
* **Framework:** Arduino framework via PlatformIO.
* **PubSubClient:** Core library for MQTT protocol implementation.
* **ArduinoJson:** Used for efficient JSON serialization of telemetry data.
* **WiFi:** Built-in ESP32 core networking library.

## Getting Started
1. **Clone & Open:** Clone this repository and open it in VS Code with the PlatformIO extension installed.
2. **Configure Credentials:** Update your Wi-Fi SSID and password in the `secret.h` file.
3. **Build & Upload:** Build the project and upload the firmware to your ESP32-S3 board.
4. **Monitor:** Open the Serial Monitor at `115200` baud rate to view real-time logs.

**Daily Development Log**
 * **Day1:** Connected to Wi-Fi and the MQTT broker.Utilized the client.loop() function to maintian connection and avert disconnection.
 * **Day2:** Subcribed to a specific topic.Implemented a callback function to remotely control the onboard LED
 * **Day3:** Published simulated temperature data every 5 seconds.Used a non-blocking time interval mechanism to avert blocking in the main loop.
 * **Day4:** Formatted the telemetry data into JSON strings before publishing,making the playload more structured and standardized.
 * **Day5:** Addressed Wi-Fi and MQTT auto-connection issues.Resolved a bug where immediated,continuous reconnection attempts caused network failures by introducing a cooldown interval.
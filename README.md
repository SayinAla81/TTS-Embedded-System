# Arduino ESP32 Text-to-Speech Project

This project demonstrates a system where users can input text on a local web interface. The text is sent to an ESP32 module, which converts it into speech using a speaker connected to the module. The system integrates a Django-based backend, an HTML frontend, and MQTT for data communication between the backend and the ESP32 module.

## Features
- **Local Web Interface**: Users can type text into a simple HTML-based frontend.
- **Django Backend**: Manages the form submission and MQTT message publishing.
- **ESP32 Module**: Receives the text via MQTT, processes it, and plays the generated speech on a speaker.
- **Text-to-Speech Conversion**: Converts the input text to audible speech using the ESP32.

## Project Structure
- **Django Application**
  - `manage.py`: The entry point for running the Django server.
  - `models.py`: Contains the data model for storing user inputs.
- **Arduino Code**
  - `TTS_arduino.ino`: The sketch for ESP32 to receive MQTT messages and play speech.

## Dependencies
### Backend
- Python 3.x
- Django 3.x or higher
- paho-mqtt (Python MQTT library)

### Frontend
- HTML and basic CSS

### ESP32
- Arduino IDE
- PubSubClient library for MQTT
- ESP32 board package

## Installation and Setup
### Backend
1. **Install Dependencies**:
   ```bash
   pip install django paho-mqtt
  ```

2. **Start the Django Project**:
    ```bash
    python manage.py runserver
    ```
3. **Open the local web interface in your browser**: http://127.0.0.1:8000/.

### ESP32
#### Setup Arduino IDE:
- **Install the ESP32 board package**:
  1. Open Arduino IDE.
  2. Go to `File > Preferences`, and in the "Additional Board Manager URLs," add:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
  3. Go to `Tools > Board > Board Manager`, search for "ESP32," and install the board package.

- **Install the PubSubClient library for MQTT**:
  1. Go to `Sketch > Include Library > Manage Libraries`.
  2. Search for "PubSubClient" and install it.

#### Upload the Code:
1. Open `TTS_arduino.ino` in the Arduino IDE.
2. Update the following parameters in the code:
   - **WiFi SSID and Password**: Replace with your network credentials.
   - **MQTT Broker Details**: Configure the broker's address, port, and credentials if necessary.
3. Connect your ESP32 to your computer via USB and select the correct port and board from the `Tools` menu.
4. Compile and upload the sketch to the ESP32.

#### Connect a Speaker:
- Connect a compatible speaker to the ESP32. Use the GPIO pin specified in the code for audio output.

---

### How It Works
1. **User Interaction**:
   - The user enters text in the local web interface.
2. **Backend Processing**:
   - The text is processed and sent to the ESP32 via MQTT.
3. **ESP32 Processing**:
   - The ESP32 receives the text, converts it to speech, and plays it through a connected speaker.

---

### Configuration
#### MQTT
- The backend publishes MQTT messages to a specific topic on the broker.
- The ESP32 subscribes to the same topic to receive messages.

#### Backend Configuration
- Update the MQTT broker details in the Django backend if required.

---

### Example Usage
1. Start the Django server by running:
   ```bash
   python manage.py runserver
   ```
2. Upload the ESP32 code using the Arduino IDE.
3. Navigate to the local web interface (http://127.0.0.1:8000/).
4. Enter text and submit the form.
5. The ESP32 receives the text, converts it into speech, and plays it through the speaker.

### Contributing
Feel free to submit issues or pull requests to improve the project.

---

### License
This project is licensed under the MIT License.

---

### Acknowledgments
- Thanks to the developers of Django and Arduino for making such robust tools.
- MQTT communication made possible by the PubSubClient library.

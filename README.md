# FluffelDrone 🛸

FluffelDrone is a school project for **FluffelBot**. The goal is to build a small, low-cost drone that can be controlled over Wi-Fi.

The project is still a work in progress. It is not confirmed to be flight-ready.

## Planned hardware

The project notes describe these main parts:

- ESP32-CAM board with an OV2640 camera
- MPU6050 motion sensor
- Four small brushed motors
- A custom all-in-one PCB

These are the planned parts from the project notes. The bill of materials in this repository does not list the parts yet.

## Software

- The PlatformIO configuration is set up for an **ESP32-CAM** using the Arduino framework.
- It lists the **PubSubClient** and **Adafruit MPU6050** libraries.
- The Python requirements file lists **paho-mqtt** and **python-dotenv**.

The firmware and ground-control code could not be found in the expected source-file locations, so the flight controls and video streaming have not been verified.

## Project status

- [x] PlatformIO project configuration
- [x] Initial Python dependency list
- [ ] Complete and verify the firmware and ground-control code
- [ ] Finish and test the hardware
- [ ] Test the first flight safely

## Safety

Do not try to fly the drone until the hardware, software, and failsafe functions have been checked.

---

Made for the school project **FluffelBot**.

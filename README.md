# FluffelDrone 🛸

![Fusion](https://halflife.hackclub-assets.com/hackclub-half-life/sessions/4fbHVD2HJmUknZX98YuazYMMhIgWKpgb/088c2f7abf212ee6e46e57684c220eedd7e68d5faa39fe857cf0ac2349648325.png)

![Fusion Top](https://halflife.hackclub-assets.com/hackclub-half-life/sessions/4fbHVD2HJmUknZX98YuazYMMhIgWKpgb/34293e5cb0ec5d3cb2e510b2a2fbc2f7bac608d918683207c4598bf552f603f8.png)

![New PCB Design](https://halflife.hackclub-assets.com/hackclub-half-life/sessions/4fbHVD2HJmUknZX98YuazYMMhIgWKpgb/d127b77ce6228ac30c2db8ebdd53a88c3f84da65711a327231304b6e341bee2b.png)

FluffelDrone is an ultra-lightweight, budget-friendly (€30), and open-source DIY Wi-Fi micro drone. Built around the **ESP32-CAM** module and configured via a custom-designed All-in-One (AIO) PCB, this drone stream lines live video and receives real-time flight commands via **MQTT** using a custom Python script.

---

## Features 🚀

- **Ultra Low Cost:** Designed entirely with standard components keeping the total build cost under €30.
- **Wi-Fi Flight Control:** Steerable from any computer or local network device via MQTT messaging protocol.
- **Live Video Streaming:** Built-in OV2640 camera module allows real-time First-Person-View (FPV) video feed.
- **Custom AIO PCB:** Optimized 50x50 mm 2-layer board layout with a massive GND copper pour for optimal thermal dissipation and interference shielding.
- **Glued Frame Design:** No screws or mounting holes used on the PCB to strip away unnecessary weight, maximizing thrust-to-weight ratio.

---

## Hardware Architecture & Components 🛠️

### Core Electronics
- **Flight Controller & Video:** ESP32-CAM Development Board (OV2640 Camera module included).
- **IMU Lagesensor:** MPU6050 (6-DOF Gyroscope + Accelerometer) placed precisely at the geometric center of the PCB for ideal PID loop calculations.
- **Voltage Regulator:** TPS63070RNMR

### Drivetrain & Power
- **Motors:** 4x 8520 Coreless Brushed Motors (8.5mm x 20mm, 3.7V).
- **Motor Drivers:** 4x SI2302 N-Channel MOSFETs (SOT-23 package).
- **Protection:** 4x 1N4148 Fast-Switching Flyback Diodes wired in parallel to each motor to absorb voltage spikes.
- **Power Source:** 1S LiPo Battery (3.7V, ~550mAh).

---

## Circuit & PCB Layout 📐

The electronics have been configured using **EasyEDA**. The system has passed a strict **Design Rule Check (DRC) with 0 errors**.

### Trace Width Configuration:
- **Main Power Traces (Battery to Motors / Regulators):** Set to `0.8mm - 1.0mm` to easily handle up to 4-5A peak current.
- **Signal Traces (ESP32 to MOSFET Gates / I2C Bus):** Kept minimal at `0.25mm` to save space.
- **Ground (GND):** Handled via a continuous **Copper Area Pour** on the Bottom Layer.

---

## Software Setup 💻

The software stack consists of two major components: the drone's firmware and the ground control script.

### 1. ESP32 Firmware (Arduino C++)
Floshed onto the ESP32-CAM via an FTDI USB-to-TTL adapter. It operates at a fast control loop (~100Hz) reading sensor data from the MPU6050, processing basic Proportional (P) stabilization, and running a background MQTT callback.

**Subscribed MQTT Topics:**
- `drone/cmd/throttle` (Integer: 0 - 255)
- `drone/cmd/pitch` (Float: Direction angle)
- `drone/cmd/roll` (Float: Direction angle)

### 2. Ground Station Control (Python Script)
Runs on any PC within the same Wi-Fi network. It intercepts keystrokes via `pynput` and instantly publishes commands to the MQTT Broker (e.g., Mosquitto).

*   **W / S:** Increase / Decrease Throttle
*   **I / K:** Pitch Forward / Backward
*   **J / L:** Roll Left / Right
*   **SPACEBAR:** Instant Software Emergency Kill-Switch (Safety Failsafe)

---

## Project Status & Milestones 📈

- [x] Hardware Concept & Component Sourcing under €30 limit
- [x] Full Circuit Schematic Design (EasyEDA)
- [x] 2-Layer AIO PCB Layout Routing (Auto-Router tuned, DRC verified)
- [x] Initial Flight Controller Firmware Structure
- [x] Python Keyboard-to-MQTT Ground Station Controller Script
- [ ] PCB Fabrication & Delivery (JLCPCB Tier 1 Funding)
- [ ] 3D Printing Frame & Final Weight-optimized assembly
- [ ] Maiden Flight & PID Tuning

---

## License 📄
This project is open-source. Feel free to clone, modify, and build your own FluffelDrone!

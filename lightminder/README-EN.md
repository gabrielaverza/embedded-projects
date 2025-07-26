# English Ver. - LightMinder

Embedded systems project developed to monitor inefficient lighting usage in indoor environments. The solution detects when there is **a light on with no one present** and emits visual and sound alerts to encourage turning it off, promoting energy savings.

---

## ⚙️ System Overview

The system is built with two microcontrollers that communicate via UART:

### Arduino Mega
- Uses an **LDR sensor** to detect ambient light.
- Uses a **PIR sensor** to detect human presence.
- When **light is on with no presence**, it starts a countdown.
- If the situation remains unchanged after a certain period, it sends the `'A'` character via UART to the PIC.
- Upon receiving the `'R'` signal back, it resumes monitoring.

### PIC18F4520 (Microgenios kit)
- Receives the `'A'` signal via UART.
- Activates **visual alerts (LED, fan)** and **sound alerts (Buzzer)**.
- Displays the system status on an **LCD display** and counts the alert time.
- The alert can be stopped:
  - By pressing a physical button.
  - Or automatically after a timeout.
- Once it stops, it sends back `'R'` via UART to the Arduino.

---

## 📁 Files in this folder

- `Arduino_master.ino`: Source code for the Arduino Mega.
- `PIC_slave.c`: C code for the PIC18F4520 (Microgenios kit).

---

## 🔌 UART Communication

- **Baud rate:** 9600 bps
- **Simple protocol with two commands:**
  - `'A'`: alert – light on without presence.
  - `'R'`: resume monitoring.

---

## 🛠️ Requirements

- Arduino Mega 2560
- PIC18F4520 (Microgenios kit)
- LDR sensor
- PIR sensor (HC-SR501)
- LCD Display (I2C)
- UART communication between microcontrollers

---

## 🧰 Tools and Software Used

- **Arduino IDE**: for writing Arduino code and using the serial monitor.
- **MikroC PRO for PIC**: to compile the C code and generate the `.hex` for the PIC.
- **PICKit2 Programmer**: to upload the `.hex` file to the PIC microcontroller.
- **PICSimLab**: to simulate and test the PIC circuit.

---

## 📷 Schematics and Setup

### 🔌 Arduino Wiring Diagram (based on Arduino Uno):

> ![Arduino Schematic](assets/arduino.png)

*Note: Diagram shows Arduino Uno, but the final system uses Arduino Mega.*

### 🧭 PIC Kit Switch Configuration for Used Components:

> ![PIC DIP Switch Config](assets/pic.png)

### 🔌 UART Communication Wiring Diagram:

> ![UART Schematic](assets/uart.png)
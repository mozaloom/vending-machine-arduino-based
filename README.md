# Vending Machine (Arduino-Based)

A small-scale, Arduino-powered vending machine prototype developed as an Embedded Systems project at Al‑Balqa' Applied University.

---

## Project Overview

This repository contains the hardware designs, firmware, documentation, and media for a prototype vending machine that:

- Accepts and validates coins via a load-cell weight sensor and IR sensors  
- Allows users to select items via a 4×4 keypad  
- Displays status, prices, and prompts on a 16×2 LCD  
- Dispenses products using servo-driven mechanism  
- Housings and mounts crafted in wood and 3D-printed plastic  

---

## Repository Structure

```

.
├── LICENSE
├── README.md                      ← you are here
├── VendingMachineDocumentation.pdf ← full project report
├── circuit
│   └── vending Machine Diagram.fzz  ← Fritzing circuit diagram
├── code
│   └── Vending\_Machine\_Final.ino    ← Arduino Mega firmware
├── images
│   ├── vm-0.png
│   ├── vm-1.jpg
│   └── prototype photos
├── presentation
│   └── VM2.pptx                    ← final presentation slides
└── videos
└── vm-0.mp4                    ← demonstration video

````

---

## Hardware Components

| Component               | Qty | Description                                  |
|-------------------------|:---:|----------------------------------------------|
| Arduino Mega 2560       |  1  | Main microcontroller                         |
| Servo motors            |  4  | Item-dispensing mechanism                    |
| Load-cell + HX711       |  1  | Coin weight measurement                      |
| IR obstacle sensors     |  3  | Coin speed and diameter detection            |
| 4×4 Keypad              |  1  | User input for item selection                |
| 16×2 LCD (I²C)          |  1  | Transaction and menu display                 |
| Wood panels & fasteners | —   | Enclosure structure                          |
| 3D-printed parts        | —   | Coin slot, mounts, decorative elements       |
| Wiring, resistors, breadboard | — | Circuit interconnects                    |

---

## Installation & Setup

1. **Clone this repository**  
   ```bash
   git clone https://github.com/your-username/vending-machine-arduino-based.git
   cd vending-machine-arduino-based
````

2. **Assemble hardware**

   * Follow the Fritzing diagram in `circuit/vending Machine Diagram.fzz`.
   * Mount sensors, servo motors, keypad, and LCD into the wooden frame.
   * Wire all components to the Arduino Mega as shown in the documentation (pp. 8–12).

3. **Upload firmware**

   * Open `code/Vending_Machine_Final.ino` in the Arduino IDE.
   * Select “Arduino Mega 2560” as the board and the correct COM port.
   * Install required libraries:

     * `LiquidCrystal_I2C`
     * `Keypad`
     * `HX711`
   * Upload to the board.

4. **Power & test**

   * Supply 5 V to the Arduino (USB or external).
   * Insert coins and use the keypad to select items.
   * Verify LCD prompts and servo dispensing.

---

## Usage

1. **Startup**

   * LCD displays welcome message and item list.
2. **Insert coin**

   * Machine measures weight and dimensions; LCD shows accepted value.
3. **Select item**

   * Enter the item code on the keypad (e.g. `A1`).
   * LCD confirms selection and remaining balance.
4. **Dispense**

   * Servo motors rotate to push the item into the delivery slot.
   * LCD shows “Thank you!” and resets for the next user.

---

## Documentation

For detailed design decisions, circuit schematics, code structure, and test results, see **VendingMachineDocumentation.pdf**.

---

## Project Timeline

* **Nov–Dec 2023**: Design (3D, woodcraft), hardware integration, coin-detection implementation
* **Jan 2024**: Firmware development, testing, presentation preparation
* **Jan 2 2024**: Final demonstration and report submission

---

## License

This project is released under the [MIT License](LICENSE).

# Glove-Controlled Robot

A wireless, gesture-controlled robot built entirely from scratch as an Engineer's Thesis project. The system consists of a custom-built data glove and a 3-axis mobile robot — both designed in Fusion 360 and manufactured with a 3D printer.

## Demo

![Glove on hand](docs/glove_on_hand.jpg)
![Robot](docs/robot.jpg)

## Overview

The operator wears the glove on their right hand. Bending individual fingers sends commands wirelessly to the robot, which responds in real time. The robot can drive in all directions and control a 3-axis manipulator with a gripper — all using one hand.

## System Architecture

```
[Glove — Arduino Nano]
    5x flex sensors (velostat + copper tape)
    → voltage divider circuit
    → analogRead (A1–A5)
    → HC-12 radio module (TX)
            |
            | wireless (up to 1000m)
            |
[Robot — Arduino Uno]
    HC-12 radio module (RX)
    → L298N motor driver → 4x DC motors (IT-25GA370, 12V)
    → VarSpeedServo → 3x MG995 servos (manipulator)
    → VarSpeedServo → 1x SG90 servo (gripper)
```

## Controls

| Gesture | Action |
|---------|--------|
| Pinky + Finger 1 | Drive forward |
| Pinky + Finger 2 | Drive backward |
| Pinky + Finger 3 | Turn left |
| Pinky + Finger 4 | Turn right |
| Finger 2 (alone) | Manipulator axis 1 forward |
| Finger 2 + Thumb | Manipulator axis 1 backward |
| Finger 3 (alone) | Manipulator axis 2 forward |
| Finger 3 + Thumb | Manipulator axis 2 backward |
| Finger 4 (alone) | Manipulator axis 3 forward |
| Finger 4 + Thumb | Manipulator axis 3 backward |
| Strong thumb bend | Toggle gripper open/close |

## Hardware

### Glove
- Arduino Nano
- 5x custom flex sensors (velostat + copper tape + plexiglass)
- HC-12 radio module
- 9V battery
- 3D-printed rings and housing (Fusion 360 + Creality Ender-3)

### Robot
- Arduino Uno
- HC-12 radio module
- L298N dual H-bridge motor driver
- 4x IT-25GA370 DC motors (12V, 100 RPM)
- 3x Tower Pro MG995 servos (manipulator)
- 1x Tower Pro SG90 servo (gripper)
- 12V LiPo battery (3.6Ah)
- DC-DC step-down converter (12V → 5V for servos)
- 3D-printed chassis, arm, and wheels (Fusion 360 + Creality Ender-3)

## Flex Sensor Design

Each sensor was built manually from:
- Velostat (piezoresistive polymer film)
- Copper tape electrodes
- Plexiglass backing
- Soldered copper wire leads

When a finger bends, the velostat compresses and resistance drops. This is read by Arduino via a voltage divider circuit.

## Specs

| Parameter | Value |
|-----------|-------|
| Wireless range | up to 1000m (line of sight) |
| Max payload (gripper) | ~170g |
| Max robot payload | ~2kg |
| Gripper jaw opening | 60mm |
| Robot weight | 1630g |
| Glove battery life | ~3 hours |
| Robot battery life | ~38 minutes |

## Build & Flash

```bash
# Open in Arduino IDE
# Glove: flash glove.ino to Arduino Nano
# Robot: flash robot.ino to Arduino Uno

# Required libraries:
# - SoftwareSerial (built-in)
# - VarSpeedServo (install via Library Manager)
```

## Author

MrGessio — Mechatronics Engineer  
Engineer's Thesis, 2022  
GitHub: [github.com/MrGessio](https://github.com/MrGessio)

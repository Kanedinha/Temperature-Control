# Temperature-Control
 Assingment for Mecatronic subject of IPB

The main idea is use a triac and a optocoupler to control the number of sinusoidal cycles over the Load, so we can control de RMS Voltage over the Load. that circuit can be called as a solid state relay, this circuit already have modules projected to work in high voltage and high current.

To complete de feedback loop, we can use a simple NTC sensor. So the following schema is projected that way
![image](https://github.com/Kanedinha/Temperature-Control/assets/59540104/183ca9ab-0111-4507-a460-bc9e8cc0ed33)

The following block diagram is used to model the system:
![image](https://github.com/Kanedinha/Temperature-Control/blob/main/Assets/Block%20Diagram.jpeg)

One detail that impact the dinamic of system is de distance of the sensor from the heater, this mean the system have a slow response of the air temperature. this detail need to include in the system modeling.
![image](https://github.com/Kanedinha/Temperature-Control/assets/59540104/d94bb714-a647-47af-94e5-2fd2ffb1cdbd)

For the PCB we can use the KiCAD project to guide us how to make an PCB, because how we will use a high voltage electrical network (230 Vrms; 50 Hz - in Portugal), its better use a PCB than a breadboard.
![image](https://github.com/Kanedinha/Temperature-Control/assets/59540104/c1c61bb9-f21f-4485-b033-4a6747136963)



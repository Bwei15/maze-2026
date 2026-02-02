/*
═══════════════════════════════════════════════════════════════════════════════
🧠 MAZE – TurtleBot Projekt (Version 0.06)

🧩 Dateistruktur:
- Main_v0_04.ino   → Hauptprogramm (setup, loop, Zustandswechsel)
- States.ino       → Zustände: Kalibrieren, Starten, Fahren
- setup.ino        → Motor- und Sensor-Initialisierung
- sensoren.ino     → Sensoren einlesen und auswerten
- drive.ino        → Fahr- und Drehfunktionen (Motorsteuerung)
- funktionen.ino   → Hilfsfunktionen (Akku, Restart, Quaternion)
- variablen.h      → Globale Variablen, IDs, Konstanten
- turtlebot3_sensor → Treiber für IMU/Sensoren (NICHT BEARBEITEN!)

🔁 Ablauf:
1️⃣ setup() → Kalibrieren (state = 0)
2️⃣ loop()  → je nach state:
    - 0: kalibrierung()  
    - 1: start() (Taster prüfen, Akku anzeigen)  
    - 2: forward_search() (Fahren starten)
    - ... weitere states kommen hier noch dazu
═══════════════════════════════════════════════════════════════════════════════
*/


#include "variablen.h"
#include "turtlebot3_sensor.h"

Turtlebot3Sensor imu;

// ------------------------ Setup ------------------------
void setup() {
  Serial.begin(115200);
}


// ------------------------ Loop ------------------------

void loop() {
  if (sensors_ready) {
    sensoren_auslesen();
  }
  // State Machine 
  switch (state) {
    case 0:
      //Sensor Kalibirierung
      kalibrierung();
      break;
    case 1:
      start();
      break;
    case 2:
      forward_search();
      break;
    case 3:
      vorwaerts();
      break;
  }
}

void _loop(){
  sensoren_auslesen();
}

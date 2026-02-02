/*
📘 Datei: funktionen.ino
Beschreibung:
Sammelt kleine Hilfsfunktionen, die an mehreren Stellen gebraucht werden.

Funktionen:
- battery_voltage()      → misst Batteriespannung
- restart(reason, wait)  → zeigt Grund an, wartet und setzt state = 0
- quaternionToEuler()    → rechnet IMU-Quaternion in Roll/Pitch/Yaw um
*/

float battery_voltage() {
  adc_value = analogRead(BDPIN_BAT_PWR_ADC);
  vol_value = map(adc_value, 0, 1023, 0, 330 * 57 / 10);
  vol_value = vol_value / 100;
  return vol_value;
}

void restart(String reason, int delay) {
  Serial.println("\n \n \n[System]     Neustarten - Grund: " + reason +
                 "\n \n \n");
  delay(delay);
  state = 0;
}

void quaternionToEuler(float q[4], float rpy[3]) {
  // orientation[0] = w
  // orientation[1] = x
  // orientation[2] = y
  // orientation[3] = z

  // Roll (x-Achse Neigung)
  double sinr_cosp = 2 * (q[0] * q[1] + q[2] * q[3]);
  double cosr_cosp = 1 - 2 * (q[1] * q[1] + q[2] * q[2]);
  rpy[0] = atan2(sinr_cosp, cosr_cosp);

  // Pitch (y-Achse Neigung)
  double sinp = 2 * (q[0] * q[2] - q[3] * q[1]);
  if (abs(sinp) >= 1)
    rpy[1] = copysign(M_PI / 2, sinp); // 90 Grad (Gimbal Lock)
  else
    rpy[1] = asin(sinp);

  // Yaw (z-Achse Drehung)
  double siny_cosp = 2 * (q[0] * q[3] + q[1] * q[2]);
  double cosy_cosp = 1 - 2 * (q[2] * q[2] + q[3] * q[3]);
  rpy[2] = atan2(siny_cosp, cosy_cosp);
}

void write8(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(TCS_ADDR);
  Wire.write(0x80 | reg); // COMMAND_BIT
  Wire.write(val);
  Wire.endTransmission();
}

uint16_t read16(uint8_t reg) {
  Wire.beginTransmission(TCS_ADDR);
  Wire.write(0x80 | reg);
  Wire.endTransmission();
  Wire.requestFrom(TCS_ADDR, (uint8_t)2);
  uint16_t low = Wire.read();
  uint16_t high = Wire.read();
  return (high << 8) | low;
}

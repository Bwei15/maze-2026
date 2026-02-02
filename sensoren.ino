/*
📘 Datei: sensoren.ino
Beschreibung:
Liest alle Sensoren aus (aktuell nur die IMU) und berechnet Roll, Pitch, Yaw.

Funktionen:
- sensoren_auslesen() → aktualisiert IMU-Daten
- gyro_auslesen()     → berechnet Orientierung in Grad
*/

void sensoren_auslesen() {
  // 1. Prüfen: Ist genug Zeit vergangen? (z.B. 50ms)
  if (millis() - last_sensor_time < SENSOR_INTERVAL) {
    return;
  }

  // 2. Zeit merken
  last_sensor_time = millis();

  // 3. Erst JETZT Sensoren lesen
  gyro_auslesen();
  tof_auslesen(tof_zaeler);
}

void gyro_auslesen() {
  imu.updateIMU();
  float* orientation = imu.getOrientation();
  float rpy_winkel[3];
  quaternionToEuler(orientation, rpy_winkel);

  roll_deg = rpy_winkel[0] * 180.0 / M_PI;
  pitch_deg = rpy_winkel[1] * 180.0 / M_PI;
  yaw_deg = rpy_winkel[2] * 180.0 / M_PI;
}

void tof_auslesen(int nummer) {

  for (int i = 0; i < nummer; i++) {
    VL53L0X_RangingMeasurementData_t measure;
    tof[i].rangingTest(&measure, false);
    distance[i] = measure.RangeMilliMeter;
  }
}

void tof_auslesen_fahren(int modus) {
  if (modus == 1) {
    VL53L0X_RangingMeasurementData_t measure;
    tof[0].rangingTest(&measure, false);
    distance[0] = measure.RangeMilliMeter;
    for (int i = 2; i < 4; i++) {
      VL53L0X_RangingMeasurementData_t measure;
      tof[i].rangingTest(&measure, false);
      distance[i] = measure.RangeMilliMeter;
    }
  } else if (modus == 2) {
    VL53L0X_RangingMeasurementData_t measure;
    tof[5].rangingTest(&measure, false);
    distance[5] = measure.RangeMilliMeter;
    for (int i = 2; i < 4; i++) {
      VL53L0X_RangingMeasurementData_t measure;
      tof[i].rangingTest(&measure, false);
      distance[i] = measure.RangeMilliMeter;
    }
  }
}
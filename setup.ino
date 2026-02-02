/*
📘 Datei: setup.ino
Beschreibung:
Initialisiert alle Motoren, Sensoren und prüft, ob sie richtig funktionieren.

Funktionen:
- motoren_pruefen()  → sucht alle Dynamixel-Motoren und meldet Fehler
- gyro_kalibrieren() → startet IMU-Kalibrierung (Roboter ruhig halten!)
*/

void motoren_pruefen() {
  error_motoren = false;
  const char *log;
  result = dxl_wb.init(DEVICE_NAME, BAUDRATE, &log);
  if (!result) {
    error_motoren = true;
    Serial.println("[ERROR] - Motoren können nicht initialisiert werden.");
    Serial.println(log);
    return;
  }
  uint8_t ids[] = {dxl_id_1, dxl_id_2, dxl_id_3, dxl_id_4};
  for (uint8_t i = 0; i < 4; i++) {
    uint16_t mn = 0;
    result = dxl_wb.ping(ids[i], &mn, &log);
    if (!result) {
      error_motoren = true;
      Serial.print("[ERROR] - Motor ");
      Serial.print(i + 1);
      Serial.print(" (ID: ");
      Serial.print(ids[i]);
      Serial.println(") nicht erreichbar.");
      Serial.println(log);
    }
    result = dxl_wb.wheelMode(ids[i], 0, &log);
    if (!result) {
      error_motoren = true;
      Serial.print("[ERROR] - Modus für Motor  ");
      Serial.print(ids[i]);
      Serial.print(": ");
      Serial.println(log);
    }
  }
  if (error_motoren) {
    restart("Fehler bei den Motoren.", 0);
  }
}

void gyro_kalibrieren() {
  delay(2000);
  bool init_ok = imu.init();

  if (!init_ok) {
    Serial.println("[System]     IMU Initialisierung fehlgeschlagen!");
    while (1)
      ;
  }
  Serial.println("[Gyro]       Kalibriere Gyro... Bitte Board ruhig halten.");
  if (melody_played == true) {
    int noteDuration = 1000 / noteDurations[2];
    tone(BDPIN_BUZZER, melody[2], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BDPIN_BUZZER);
  }
  imu.calibrationGyro();
  Serial.println("[Gyro]       Kalibrierung abgeschlossen.");
}

void tofs_adressieren() {
  // 1) Alle Sensoren ausschalten
  for (int i = 0; i < tof_zaeler; i++) {
    pinMode(xshutPins[i], OUTPUT);
    digitalWrite(xshutPins[i], LOW);
  }
  delay(100);

  // 2) Jeden Sensor einzeln aktivieren → Adresse ändern → nächster
  for (int i = 0; i < tof_zaeler; i++) {
    digitalWrite(xshutPins[i], HIGH);
    delay(50);

    if (!tof[i].begin(addresses[i])) { // immer erst Default-Adresse!
      Serial.print("Sensor ");
      Serial.print(i + 1);
      Serial.println(" nicht gestartet!");
      digitalWrite(xshutPins[i], LOW);
      delay(50);
      continue;
    }

    tof[i].begin(addresses[i], false, &Wire,
                 Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_SPEED); // jetzt neue
                                                              // Adresse setzen
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(" -> neue Adresse: 0x");
    Serial.println(addresses[i], HEX);
  }
}

void tofs_pruefen() {
  Serial.println("[System]     Prüfe I2C Bus...");
  int gefundene_tofs = 0;

  // Wir prüfen gezielt nur, ob unsere neuen Adressen erreichbar sind
  for (int i = 0; i < tof_zaeler; i++) {
    Wire.beginTransmission(addresses[i]);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("[Check]      ToF ");
      Serial.print(i);
      Serial.println(" OK.");
      gefundene_tofs++;
    } else {
      Serial.print("[ERROR]      ToF ");
      Serial.print(i);
      Serial.println(" fehlt!");
    }
  }

  if (gefundene_tofs != tof_zaeler) {
    restart("Nicht alle ToFs gefunden", 2000);
  } else {
    Serial.println("[System]     Alle ToFs bereit.");
  }
}

void farbsensor_starten() {
  digitalWrite(Farbsensor_pin, HIGH);
  delay(2000);
  write8(ENABLE, ENABLE_PON);
  delay(3);
  write8(ENABLE, ENABLE_PON | ENABLE_AEN);
  write8(CONTROL, 0x01); // Gain x4

  Serial.println("TCS34725 gestartet!");
}
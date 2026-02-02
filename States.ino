/*
📘 Datei: States.ino
Beschreibung:
Steuert die Hauptzustände des Roboters (Kalibrieren, Starten, Fahren).

Funktionen:
- kalibrierung()   → prüft Motoren und Sensoren, startet Kalibrierung
- start()          → zeigt Akku, wartet auf Tastendruck zum Start
- forward_search() → Beispiel: dreht rechts, fährt vorwärts

Hinweis:
Später können hier neue Zustände ergänzt werden (z. B. Maze-Navigation).
*/

// In Datei: States.ino

void kalibrierung() {
  Serial.println("\n \n[System]     Sensoren - kalibrieren...");
  pinMode(BDPIN_PUSH_SW_1, INPUT);
  pinMode(Farbsensor_pin, OUTPUT);
  // WICHTIG: Kurze Wartezeit, damit sich die Batterie-Spannung stabilisiert
  delay(500);

  Wire.begin();
  // --- Checkpoint 0: RESET ---
  digitalWrite(Farbsensor_pin, LOW);

  // --- Checkpoint 1: Gyro Start ---
  gyro_kalibrieren();
  // Wenn du den ersten Ton hörst, war das hier erfolgreich.

  // DEBUG-BEEP 1 (Kurz hoch)
  tone(BDPIN_BUZZER, 1000, 100);
  delay(200);

  // --- Checkpoint 2: Motoren ---
  motoren_pruefen();
  // Wenn er hier abstürzt, hörst du den nächsten Beep NICHT.

  // DEBUG-BEEP 2 (2x Kurz)
  tone(BDPIN_BUZZER, 1000, 100);
  delay(150);
  tone(BDPIN_BUZZER, 1000, 100);
  delay(200);

  // --- Checkpoint 3: TOF Sensoren ---
  tofs_adressieren();
  tofs_pruefen();

  // DEBUG-BEEP 3 (3x Kurz - Fast geschafft)
  tone(BDPIN_BUZZER, 1000, 100);
  delay(150);
  tone(BDPIN_BUZZER, 1000, 100);
  delay(150);
  tone(BDPIN_BUZZER, 1000, 100);
  delay(200);

  // --- Checkpoint 3: Farbsensor ---
  farbsensor_starten();

  // DEBUG-BEEP 4
  tone(BDPIN_BUZZER, 1000, 100);
  delay(150);
  tone(BDPIN_BUZZER, 1000, 100);
  delay(150);
  tone(BDPIN_BUZZER, 1000, 100);
  delay(200);
  tone(BDPIN_BUZZER, 1000, 100);
  delay(250);

  Serial.println("[System]     Sensoren - Fertig");
  setup_status = true;
  sensors_ready = true;
  state = 1;
}



void start() {
  if (setup_status == true) {
    Serial.println("[Battery]    " + String(battery_voltage()) + "V");
    setup_status = false;
    if (battery_voltage() <= 0.01) {
      restart("Kein Akku angeschlossen. Schließe einen an!", 3000);
    }
  }

  push_state = digitalRead(BDPIN_PUSH_SW_1) << 0;
  push_state |= digitalRead(BDPIN_PUSH_SW_2) << 1;
  if (melody_played == true) {
    for (int thisNote = 0; thisNote < 8; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(BDPIN_BUZZER, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(BDPIN_BUZZER);
    }
    melody_played = false;
  }
  farbsensor_auslesen();
  
  if (push_state == 1) {
    Serial.println("[System]     Drive");
    state = 2;
  } else if (push_state == 2) {
    restart("Vom Nutzer ausgewählt.", 0);
  }
}

void forward_search() {
  _loop();
  wanderkennung();
  if (!wand_rechts) {
    abbiegen("rechts");
  } else if (!wand_vorne) {
    vorwaerts();  // Wenn rechts Wand ist, aber vorne frei -> Geradeaus
  } else if (!wand_links) {
    abbiegen("links");  // Nur wenn rechts UND vorne zu ist
  } else {
    abbiegen("drehen");  // Sackgasse
  }
}

void vorwaerts() {
  Serial.println("[System]     voraerts()");
  kachel(1);
  state = 2;
}

void abbiegen(String dir) {
  if (dir == "rechts") {
    Serial.println("[System]     abbiegen(rechts)");
    turn_right(100);
    _loop();
    kachel(1);
  } else if (dir == "links") {
    Serial.println("[System]     abbiegen(links)");
    turn_left(100);
    _loop();
    kachel(1);
  } else if (dir == "drehen") {
    Serial.println("[System]     abbiegen(drehen)");
    turn_right(100);
    turn_right(100);
    _loop();
    kachel(1);
  }
  state = 2;
}

void wanderkennung() {
  if (distance[0] <= 320) {
    wand_vorne = true;
  } else {
    wand_vorne = false;
  }
  if (((distance[2] + distance[3]) / 2) <= 320) {
    wand_rechts = true;
  } else {
    wand_rechts = false;
  }
  if (((distance[5]) / 2) <= 320) {
    wand_hinten = true;
  } else {
    wand_hinten = false;
  }
  if (((distance[6] + distance[7]) / 2) <= 320) {
    wand_links = true;
  } else {
    wand_links = false;
  }

  // -------


  Serial.println("-------");
  if (wand_vorne) {
    Serial.println("+---+");
  } else {
    Serial.println("+   +");
  }
  String mitte = "";

  if (wand_links) {
    mitte += "|";
  } else {
    mitte += " ";
  }

  mitte += " R ";

  if (wand_rechts) {
    mitte += "|";
  } else {
    mitte += " ";
  }
  Serial.println(mitte);

  if (wand_hinten) {
    Serial.println("+---+");
  } else {
    Serial.println("+   +");
  }

  Serial.println("-------");
}

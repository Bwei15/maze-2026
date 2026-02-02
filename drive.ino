/*
📘 Datei: drive.ino
Beschreibung:
Enthält alle Fahrfunktionen – z. B. drehen, vorwärts fahren.

Funktionen:
- turn_right(speed) → dreht 90° nach rechts (momentan blockierend)
- turn_left(speed)  → dreht 90° nach links
- drive(mode, speed)→ steuert Motoren je nach Modus:
    0   = vorwärts
    90  = rechts drehen
    270 = links drehen
    500 = stop

Rückgabe: true = Roboter fährt noch, false = Bewegung fertig
*/

void turn_left(int speed) {
  while (drive(90, speed)) {
    gyro_auslesen();
  };
}
void turn_right(int speed) {
  while (drive(270, speed)) {
    gyro_auslesen();
  };
}

void kachel(int anzahl) {
  if (distance[0] > 600) {
    int kachel_hinten = (int)(distance[5] / 320);
    float goal_tof = (kachel_hinten + 1) * 320 + 40;
    Serial.print("Start Dist (h): ");
    Serial.println(distance[5]);
    Serial.print("Kacheln (h): ");
    Serial.println(kachel_hinten);
    Serial.print("Distance (h): ");
    Serial.println(goal_tof);

    while (distance[5] <= goal_tof) {
      tof_auslesen_fahren(2);

      if ((distance[2] - distance[3]) > 30 || (distance[2] > 200) || (distance[3] > 200)) {
        drive(0, 100);
      } else {
        drive(1, 100);
      }
      Serial.print(distance[5]);
      Serial.print(" | ");
      Serial.println(goal_tof);
    }
    drive(0, 0);

  } else {
    int kachel_vorne = (int)(distance[0] / 320);
    float goal_tof = (kachel_vorne - 1) * 320 + 140;
    Serial.print("Start Dist: ");
    Serial.println(distance[0]);
    Serial.print("Kacheln: ");
    Serial.println(kachel_vorne);
    Serial.print("Distance: ");
    Serial.println(goal_tof);

    while (distance[0] >= goal_tof) {
      tof_auslesen_fahren(1);

      if ((distance[2] - distance[3]) > 30 || (distance[2] > 200) || (distance[3] > 200)) {
        drive(0, 100);
      } else {
        drive(1, 100);
      }
      Serial.print(distance[0]);
      Serial.print(" | ");
      Serial.println(goal_tof);
    }
    drive(0, 0);
  }
}

bool drive(int mode, int speed) {

  if (mode == 500) {
    dxl_wb.goalVelocity(dxl_id_1, (int32_t)0);
    dxl_wb.goalVelocity(dxl_id_2, (int32_t)0);
    dxl_wb.goalVelocity(dxl_id_3, (int32_t)0);
    dxl_wb.goalVelocity(dxl_id_4, (int32_t)0);
  }
  if (mode == 0) {
    float gyro_wert = new_zero - yaw_deg;
    dxl_wb.goalVelocity(dxl_id_1, (int32_t)(-speed));
    dxl_wb.goalVelocity(dxl_id_2, (int32_t)(speed));
    dxl_wb.goalVelocity(dxl_id_3, (int32_t)(-speed));
    dxl_wb.goalVelocity(dxl_id_4, (int32_t)(speed));
  }
  if (mode == 1) {
    //Serial.print("2: ");
    //Serial.println(distance[2]);
    //Serial.print("3: ");
    //Serial.println(distance[3]);
    winkel_fehler = distance[2] - distance[3];
    abstand_aktuell = (distance[2] + distance[3]) / 2;
    abstand_fehler = wunsch_abstand - abstand_aktuell;
    korrektur = winkel_fehler * kw + ((abstand_fehler * ka) * -1);
    dxl_wb.goalVelocity(dxl_id_1, (int32_t)(-speed + korrektur));
    dxl_wb.goalVelocity(dxl_id_2, (int32_t)(speed + korrektur));
    dxl_wb.goalVelocity(dxl_id_3, (int32_t)(-speed - korrektur));
    dxl_wb.goalVelocity(dxl_id_4, (int32_t)(speed - korrektur));
  }
  if (mode == 90 && !isTurning) {
    goal = umrechenen_winkel(yaw_deg + 90);
    isTurning = true;
  }
  if (mode == 270 && !isTurning) {
    goal = umrechenen_winkel(yaw_deg - 90);
    isTurning = true;
    Serial.print("Deg: ");
    Serial.print(yaw_deg);
    Serial.print("goal: ");
    Serial.println(goal);
  }

  if (isTurning) {
    gyro_auslesen();
    diff = umrechenen_winkel(goal - yaw_deg);
    //Serial.print("Diff: ");
    //Serial.println(fabs(diff));
    if (fabs(diff) > 5.0) {
      //Serial.print("Winkel: ");
      //Serial.println(yaw_deg);
      float kP = 2.5;
      float w = kP * diff;
      if (w > speed) w = speed;
      if (w < -speed) w = -speed;
      dxl_wb.goalVelocity(dxl_id_1, (int32_t)-w);
      dxl_wb.goalVelocity(dxl_id_2, (int32_t)-w);
      dxl_wb.goalVelocity(dxl_id_3, (int32_t) + w);
      dxl_wb.goalVelocity(dxl_id_4, (int32_t) + w);
    } else {
      isTurning = false;
      new_zero = yaw_deg;
      dxl_wb.goalVelocity(dxl_id_1, (int32_t)0);
      dxl_wb.goalVelocity(dxl_id_2, (int32_t)0);
      dxl_wb.goalVelocity(dxl_id_3, (int32_t)0);
      dxl_wb.goalVelocity(dxl_id_4, (int32_t)0);
    }
  }
  return isTurning;
}

float umrechenen_winkel(float winkel) {
  if (winkel >= 180) {
    winkel -= 360;
  }
  if (winkel < -180) {
    winkel += 360;
  }
  return winkel;
}
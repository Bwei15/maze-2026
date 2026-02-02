

void Kamera_auslesen()  {
    if (Serial2.available() && (rechts == false || links == false)) {
    String msg = Serial2.readStringUntil('\n'); // bis zum Zeilenende lesen 
    Serial.println(msg);  // Ausgabe auf Serial Monitor

    //Links:
    if (msg == "LS" or msg == "LY") {
      Serial.println("S/Y auf der linken Seite");
      links = true;      
    }
    else if (msg == "LH" or msg == "LR")  {
      Serial.println("H/R auf der linken Seite");
      links = true;
    }
    else if (msg == "LU" or msg == "LG")  {
      Serial.println("U/G auf der linken Seite");
      links = true;
    }

    //rechts:
    if (msg == "RS" or msg == "RY") {
      Serial.println("S/Y auf der rechten Seite");
      rechts = true;
    }
    else if (msg == "RH" or msg == "RR")  {
      Serial.println("H/R auf der rechten Seite");
      rechts = true;
    }
    else if (msg == "RU" or msg == "RG")  {
      Serial.println("U/G auf der rechten Seite");
      rechts = true;
    }
  }
  else  {
    if(millis() >= (timer_kamera + 10000)) {
      rechts = false;
      links = false;
      Serial.println("Reset nach 2 sec.");
    }
  }
  if(links){
    drive(0,0);
    delay(1000);
  }
}
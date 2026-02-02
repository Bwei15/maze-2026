/*
📘 Datei: variablen.h
Beschreibung:
Enthält alle wichtigen Konstanten, IDs und Statusvariablen.

Beispiele:
- DEVICE_NAME, BAUDRATE      → Kommunikation mit Dynamixel
- DXL_ID_1..4                → Motoren
- state, setup_status, yaw_deg etc. → aktuelle Systemwerte
*/

#define tof_zaeler 8

#include "Adafruit_VL53L0X.h"
#include "Deneyap_RenkDonusturme.h"
#include <DynamixelWorkbench.h>
#include <Wire.h>

Adafruit_VL53L0X tof[tof_zaeler]; // -> 8 Sensoren

#if defined(__OPENCM904__)
#define DEVICE_NAME "3"
#elif defined(__OPENCR__)
#define DEVICE_NAME ""
#endif

#define BAUDRATE 1000000
#define DXL_ID_1 1
#define DXL_ID_2 2
#define DXL_ID_3 4
#define DXL_ID_4 3

#define ENABLE_PON 0x01
#define ENABLE_AEN 0x02
#define TCS_ADDR 0x29

// Register
#define ENABLE 0x00
#define ATIME 0x01
#define CONTROL 0x0F
#define CDATAL 0x14
#define CDATAH 0x15
#define RDATAL 0x16
#define RDATAH 0x17
#define GDATAL 0x18
#define GDATAH 0x19
#define BDATAL 0x1A
#define BDATAH 0x1B
#define STATUS 0x13

DynamixelWorkbench dxl_wb;

int state = 0;

//-----
// Events
bool start_pressed = false;
bool front_blocked = false;
bool right_open = false;
bool left_open = false;
bool victim_seen = false;
bool black_detected = false;
bool blue_detected = false;
bool no_progress = false;

//----- Motoren ----
bool result = false;
bool error_motoren = false;
uint16_t model_number = 0;
uint8_t dxl_id_1 = DXL_ID_1;
uint8_t dxl_id_2 = DXL_ID_2;
uint8_t dxl_id_3 = DXL_ID_3;
uint8_t dxl_id_4 = DXL_ID_4;

//---- Melody ----
bool melody_played = true;
int melody[] = {262, 196, 196, 220, 196, 0, 247, 262};
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

//---- Akku ----
float vol_value;
int adc_value;

//---- main ----
bool setup_status = true;
int current_ms = 0;

//---- Button ----
int push_state;

//---- Gyro ----
float roll_deg = 0;
float pitch_deg = 0;
float yaw_deg = 0;

//---- Drive ----
bool isTurning = false;
float goal = 0;
float new_zero = 0;
float diff = 0;

//---- TOFs ----
int xshutPins[tof_zaeler] = {0, 4, 5, 6,
                             7, 1, 2, 3}; // jeweils eigener XSHUT-Pin
//  Sensoren Nr beim Bot:    1, 2, 3, 4, 5, 6, 7, 8      alles -1 im Serial
//  Monitor
int addresses[tof_zaeler] = {0x30, 0x31, 0x32, 0x33,
                             0x34, 0x35, 0x36, 0x37}; // neue Adressen
float distance[tof_zaeler];
bool sensors_ready = false;

//---- Sensoren ----
float last_sensor_time = 0;
float SENSOR_INTERVAL = 50;

//---- Wände ----
bool wand_vorne = 0;
bool wand_rechts = 0;
bool wand_links = 0;
bool wand_hinten = 0;

//---- Wandverfolger ----
int wunsch_abstand = 100;
float kw = 1.5;
float ka = 1.2;
float winkel_fehler = 0;
float abstand_aktuell = 0;
float abstand_fehler = 0;
float korrektur = 0;

//---- Kamera ----
bool links = false;
bool rechts = false;
float timer_kamera = 0;

//---- Farbsensor ----
int Farbsensor_pin = 9;
uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;
double h, s, v;
char farbe = 0;
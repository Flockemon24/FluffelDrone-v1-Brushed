#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// --- WLAN & MQTT Konfiguration ---
const char* ssid = "DEIN_WLAN_NAME";
const char* password = "DEIN_WLAN_PASSWORT";
const char* mqtt_server = "111.111.1.111"; // IP deines MQTT-Brokers (z.B. Raspberry Pi)

// --- Pinbelegung Motoren (PWM) ---
const int MOTOR_FL = 12; // Vorne Links
const int MOTOR_FR = 13; // Vorne Rechts
const int MOTOR_BL = 14; // Hinten Links
const int MOTOR_BR = 15; // Hinten Rechts

// --- Steuerungs-Variablen (Setpoints) ---
int target_throttle = 0; // Gas (0 - 255)
float target_roll = 0;
float target_pitch = 0;

WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_MPU6050 mpu;

// --- MQTT Nachrichten-Empfang (Callback) ---
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  // JSON oder einfachen String parsen (Hier vereinfacht als Befehl:Wert)
  if (String(topic) == "drone/cmd/throttle") {
    target_throttle = message.toInt();
  } else if (String(topic) == "drone/cmd/pitch") {
    target_pitch = message.toFloat();
  } else if (String(topic) == "drone/cmd/roll") {
    target_roll = message.toFloat();
  }
}

void setup() {
  Serial.begin(115200);
  
  // Motoren als Ausgang definieren
  pinMode(MOTOR_FL, OUTPUT); pinMode(MOTOR_FR, OUTPUT);
  pinMode(MOTOR_BL, OUTPUT); pinMode(MOTOR_BR, OUTPUT);

  // MPU6050 Sensor starten
  if (!mpu.begin()) {
    Serial.println("Fehler: MPU6050 nicht gefunden!");
    while (1) { delay(10); }
  }

  // WLAN Verbindung herstellen
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }

  // MQTT konfigurieren
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Drone_Client")) {
      // Kanäle abonnieren (Subscriben)
      client.subscribe("drone/cmd/throttle");
      client.subscribe("drone/cmd/pitch");
      client.subscribe("drone/cmd/roll");
    } else {
      delay(2000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // Verarbeitet eingehende MQTT-Befehle

  // 1. Sensordaten auslesen
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float current_roll = a.acceleration.x;
  float current_pitch = a.acceleration.y;

  // 2. Stark vereinfachter Stabilisierungs-Regler (P-Regler)
  // In der Praxis nutzt man hier einen vollwertigen PID-Algorithmus!
  float error_roll = target_roll - current_roll;
  float error_pitch = target_pitch - current_pitch;

  float correction_roll = error_roll * 10.0;   // Verstärkungsfaktor
  float correction_pitch = error_pitch * 10.0;

  // 3. Motor-Mischer (Berechnung der Leistung für jeden Motor)
  int pwm_FL = target_throttle + correction_pitch - correction_roll;
  int pwm_FR = target_throttle + correction_pitch + correction_roll;
  int pwm_BL = target_throttle - correction_pitch - correction_roll;
  int pwm_BR = target_throttle - correction_pitch + correction_roll;

  // Werte begrenzen (PWM darf nur zwischen 0 und 255 liegen)
  pwm_FL = constrain(pwm_FL, 0, 255);
  pwm_FR = constrain(pwm_FR, 0, 255);
  pwm_BL = constrain(pwm_BL, 0, 255);
  pwm_BR = constrain(pwm_BR, 0, 255);

  // 4. Motoren ansteuern (Wenn Throttle fast 0 ist, Motoren komplett aus)
  if(target_throttle < 10) {
    analogWrite(MOTOR_FL, 0); analogWrite(MOTOR_FR, 0);
    analogWrite(MOTOR_BL, 0); analogWrite(MOTOR_BR, 0);
  } else {
    analogWrite(MOTOR_FL, pwm_FL); analogWrite(MOTOR_FR, pwm_FR);
    analogWrite(MOTOR_BL, pwm_BL); analogWrite(MOTOR_BR, pwm_BR);
  }

  delay(10); // Loop läuft mit ca. 100Hz für schnelle Stabilisierung
}

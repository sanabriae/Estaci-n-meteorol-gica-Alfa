// Este programa se creo para enviar via internet a traves de la pagina ThingSpeak 
//los datos de una estacion meteorologica experimental diseñada por Dr. Eduardo Sanabria
// Modelo Alfa V1
//ATENCION!!! DEBES CREAR UNA CUENTA EN https://thingspeak.mathworks.com/ PARA OBTENER LAS APIS DE LECTURA Y ESCRITURA
//ADEMAS ALLI PODRAS CONFIGURAR LA VISUALIZACION, ES GRATIS PARA EXPERIEMNTACION HASTA 3 MILLONES DE DATOS

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <RTClib.h>

// ------------ CONFIGURACIÓN DEL USUARIO --------------
const char* ssid     = "#######";        //incluye el wifi user
const char* password = "#######";  //incluye la clave del wifi

// API KEY de escritura de ThingSpeak
String ts_api_key = "XXXXXXX";   // <--- colocar el api write que te da la pagina ThingSpeak

const char* ts_host = "api.thingspeak.com";
// -----------------------------------------------------

// Pines y objetos
#define DHTPIN D5
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;
RTC_DS3231 rtc;

// Intervalo de envío (15 minutos)
unsigned long lastSend = 0;
const unsigned long INTERVAL = 900000UL ;   // 15 minutos, cambiar la cantidad de milis segun necesidad

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== Data Logger ThingSpeak ===");

  // --- WiFi ---
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectada!");

  // --- Sensores ---
  dht.begin();

  if (!bmp.begin(0x76)) {
    Serial.println("Error BMP280!");
    while (1);
  }

  if (!rtc.begin()) {
    Serial.println("Error RTC DS3231!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC sin hora, ajustando a hora de compilación...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {

  if (millis() - lastSend >= INTERVAL) {
    lastSend = millis();
    enviarDatos();
  }
}

void enviarDatos() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float p = bmp.readPressure() / 100.0;  // hPa
  DateTime now = rtc.now();

  if (isnan(h) || isnan(t)) {
    Serial.println("Error leyendo DHT22!");
    return;
  }

  Serial.println("\n--- Lectura ---");
  Serial.printf("Temp: %.2f °C   Hum: %.2f %%   Pres: %.2f hPa\n", t, h, p);

  // ------- Crear fecha y hora separadas --------
  char fecha[11];
  char hora[9];

  snprintf(fecha, sizeof(fecha), "%04d-%02d-%02d",
           now.year(), now.month(), now.day());

  snprintf(hora, sizeof(hora), "%02d:%02d:%02d",
           now.hour(), now.minute(), now.second());

  Serial.printf("Fecha: %s  Hora: %s\n", fecha, hora);

  // ------- Enviar a ThingSpeak --------
  WiFiClient client;

  if (!client.connect(ts_host, 80)) {
    Serial.println("Error: no conecta a ThingSpeak!");
    return;
  }

  // URL con 5 fields
  String url = "/update?api_key=" + ts_api_key;
  url += "&field1=" + String(t, 2);
  url += "&field2=" + String(h, 2);
  url += "&field3=" + String(p, 2);
  url += "&field4=" + String(fecha);    // Fecha
  url += "&field5=" + String(hora);     // Hora

  Serial.println("Enviando a ThingSpeak:");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + ts_host + "\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("Respuesta:");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    if (line == "\r") break;
  }

  client.stop();
}
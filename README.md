Estación Meteorológica Experimental "Alfa V1"
Este repositorio contiene el firmware para la Estación Meteorológica Experimental Alfa V1, diseñada para el monitoreo ambiental y la transmisión de datos en tiempo real a través de la plataforma ThingSpeak.

Desarrollado por el Dr. Eduardo Sanabria.

📝 Descripción
El sistema utiliza un microcontrolador ESP8266 para recolectar variables climáticas críticas y enviarlas vía Wi-Fi a un servidor en la nube. Está diseñado para aplicaciones de ecología experimental, permitiendo el seguimiento remoto de condiciones microclimáticas.

🛠️ Características del Hardware
El modelo Alfa V1 integra los siguientes componentes:

Microcontrolador: ESP8266 (NodeMCU / Wemos D1 Mini).

Sensor de Temperatura y Humedad: DHT22 (Precisión para estudios biológicos).

Sensor de Presión Atmosférica: BMP280 (I2C, dirección 0x76).

Reloj en Tiempo Real (RTC): DS3231 (Para el sellado de tiempo preciso en campo).

Conectividad: Wi-Fi 2.4GHz.

📊 Configuración de Datos (ThingSpeak)
El programa envía 5 campos de datos (fields) cada 15 minutos:

Field 1: Temperatura (°C)

Field 2: Humedad Relativa (%)

Field 3: Presión Atmosférica (hPa)

Field 4: Fecha (YYYY-MM-DD)

Field 5: Hora (HH:MM:SS)

🚀 Instalación y Uso
Requisitos previos
Es necesario tener instalado el IDE de Arduino con las siguientes librerías:

ESP8266WiFi

Adafruit_Sensor

DHT sensor library

Adafruit_BMP280_Library

RTClib (Adafruit)

Configuración
Antes de cargar el código, modifica las siguientes variables en el archivo principal:

C++
const char* ssid     = "TU_SSID";
const char* password = "TU_PASSWORD";
String ts_api_key    = "TU_WRITE_API_KEY";

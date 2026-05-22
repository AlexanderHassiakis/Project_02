# EXEMPEL README STRUKTUR:


# P02+ – IoT-extension av driverbiblioteket

Kort och kärnfull beskrivning av projektet. Till exempel: "En utökning av ett inbäddat drivrutinsbibliotek för ESP32-S3 skriven i C++, med fokus på objektorienterad arkitektur, nätverkskommunikation via MQTT, samt hårdvaruoberoende testning med hjälp av stubs."

## 👥 Gruppmedlemmar
* **Namn Namnsson** (Användarnamn/Email) - 
* **Namn Namnsson** (Användarnamn/Email) - 

---

## 🏗️ Arkitektur & Designprinciper

Detta projekt bygger vidare på arkitekturen från P02. Systemet är strikt lageruppdelat för att uppnå **hårdvaruoberoende**:

* **Interfaces (Abstraktion):** Systemlogiken kommunicerar *endast* med abstrakta gränssnitt (t.ex. `driver::mqtt::Interface`). Logiken har ingen aning om ifall den körs på ett riktigt chip eller en simulator.
* **Factory Pattern:** En central fabrik (`Factory`) ansvarar för att skapa och leverera rätt drivrutiner till systemlogiken.
* **Drivers & Stubs:** Vi har implementerat dubbla uppsättningar av drivrutiner:
  1. `Esp32s3`: Verkliga drivrutiner som använder ESP-IDF:s nätverks- och hårdvarustack.
  2. `Stub`: Simulerade drivrutiner för lokal testning på datorn utan hårdvara.
* **Tunn Main:** `main.cpp` innehåller ingen applikationslogik, utan initierar bara fabriken och startar `Logic::run()`.

---

## 🚀 Funktioner (Mål för G/VG)

### G-Krav (Uppfyllda)
* [x] **MQTT-Driver:** Full integration med ESP-IDF:s MQTT-klient och anslutning till HiveMQ-broker.
* [x] **Styrning via MQTT:** Systemet lyssnar på topic `ESP32_Commands` och tolkar kommandon (`on`, `off`, `blink`, `blink off`, `period x`) via klassens interna kommandotolk.
* [x] **Telemetri (Temperatur):** Systemet läser periodiskt av TMP36-sensorn via ADC och publicerar datan på topic `sensor/temp`.

### VG-Krav (Om tillämpligt)
* [ ] **Lägre kommunikationslager:** Implementerat [I2C / CAN / RS-485] för kommunikation med extern hårdvara.
* [ ] **Gateway-funktion:** MQTT-kommandon skickas vidare till det lägre lagret, och sensordata från det lägre lagret publiceras upp till MQTT.

---

## 🛠️ Hur man bygger och kör projektet

### Förutsättningar
* ESP-IDF installerat (v5.x rekommenderas).
* En MQTT-klient (t.ex. **MQTT Explorer**) för att skicka kommandon och se temperaturdata.

### Konfiguration
Innan du bygger, öppna `source/driver/mqtt/esp32s3.cpp` och ändra dina Wi-Fi-uppgifter:
```cpp
strlcpy(reinterpret_cast<char *>(wifi_config.sta.ssid), "DITT_WIFI_NAMN", ...);
strlcpy(reinterpret_cast<char *>(wifi_config.sta.password), "DITT_LÖSENORD", ...);
```
### TOPIC : ESP32_Commands för att skicka kommando över MQTT.

--- 

## 1 Konfigurera projekt ifall man vill köra i terminalen.
 idf.py menuconfig

## 2. Bygg koden
idf.py fullclean
idf.py reconfigure
idf.py build

## 3. Flashat till ESP32-S3 och starta serieövervakaren
idf.py flash 'COM' monitor


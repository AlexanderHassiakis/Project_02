
# P02+ – IoT-extension av driverbiblioteket

Projektet är en utökning av ett inbäddat drivrutinsbibliotek för ESP32-S3 skrivet i C++ med fokus på en objektorienterad arkitektur. Systemet läser av hårdvarusensorer, hanterar trådsäker exekvering, kommunicerar via MQTT, erbjuder en lokal kommandotolk via USB-Serial/JTAG samt möjliggör hårdvaruoberoende testning med hjälp av stubbar.


## 👥 Gruppmedlemmar
* **Alexander Hassiakis** 
* **Suhaib Qasim** 
* **Tim Raymond Tun** 
* **Fadi Lazkani** 

---

## 🏗️ Arkitektur & Designprinciper

Detta projekt bygger vidare på arkitekturen från P02. Systemet är strikt lageruppdelat för att uppnå **hårdvaruoberoende**:

* **Interfaces (Abstraktion):** Systemlogiken kommunicerar *endast* med abstrakta gränssnitt (t.ex. `driver::mqtt::Interface`). Logiken har ingen aning om ifall den körs på ett riktigt chip eller en simulator.
* **Factory Pattern:** En central fabrik (`Factory`) ansvarar för att skapa och leverera rätt drivrutiner till systemlogiken.
* **Drivers & Stubs:** Vi har implementerat dubbla uppsättningar av drivrutiner:
  1. `Esp32s3`: Verkliga drivrutiner som använder ESP-IDF:s nätverks- och hårdvarustack.
  2. `Stub`: Simulerade drivrutiner för lokal testning på datorn utan hårdvara.
* **Tunn Main:** `main.cpp` innehåller ingen applikationslogik, utan initierar bara fabriken och startar `Logic::run()`.


## 🛠️ Hårvaru uppkoppling / Portar
* USB - kommuntiation med USB-Serial/JTAG
* Port A1 - ADC for temprature read.
* Port A3 - LED 


## 🛠️ Hur man bygger och kör projektet

### Förutsättningar

* ESP-IDF installerat (v5.x rekommenderas).
* En MQTT-klient (t.ex. **MQTT Explorer**) för att skicka kommandon och se temperaturdata.
* WSL installerat för att köra stubar utan hårdvara.
* ESP32-S3 enhet.
* breadboard med komponenter.

### Konfiguration
Innan du bygger, öppna `source/driver/mqtt/esp32s3.cpp` och ändra dina Wi-Fi-uppgifter:
```cpp
strlcpy(reinterpret_cast<char *>(wifi_config.sta.ssid), "DITT_WIFI_NAMN", ...);
strlcpy(reinterpret_cast<char *>(wifi_config.sta.password), "DITT_LÖSENORD", ...);
```
### TOPIC : ESP32_Commands för att skicka kommando över MQTT
* Lägg till sensor/temp i topic:s under advandced i MQTT explorer.

--- 

## 1. Konfigureras ifall man vill köra kommandon i ESP-IDF terminalen.
 * idf.py menuconfig
 * Sök på Channel for console output
 * Ändra från defult till USB Serial/JTAG Controller

## 1.5 Putty 
* Starta Putty
* Sätt dessa inställningar.
    * Baud : 115200
    * Connection Typ: Serial
    * Line discipline option : ALL 'on force on'
    * Implicit CR & LF : Markerade
    * SSH/Parity : None
    * SSH/Flow control : None

## 2. Bygg koden
* idf.py fullclean
* idf.py reconfigure
* idf.py build

## 3. Flashat till ESP32-S3 och starta serieövervakaren
* idf.py flash 'Val of PORT ex: COM9' monitor

---

## Guide att köra koden i WSL

* 1. Ändra till stub koden i main.

* 2. För att söka på filer i windows 
    * cd /mnt/"Skriv sökvägen"

* 3. Kör kod för wsl, kopiera och lägg i terminalen.
    * g++ -std=c++20 main/source/main.cpp -I main/include -o logic_test && ./logic_test
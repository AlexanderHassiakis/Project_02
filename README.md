
# P02+ – IoT-extension av driverbiblioteket

Projektet är en utökning av ett inbäddat drivrutinsbibliotek för ESP32-S3 skrivet i C++ med fokus på en objektorienterad arkitektur. Systemet läser av hårdvarusensorer, hanterar trådsäker exekvering, kommunicerar via MQTT, erbjuder en lokal kommandotolk via USB-Serial/JTAG samt möjliggör hårdvaruoberoende testning med hjälp av stubbar.


## 👥 Gruppmedlemmar
* **Alexander Hassiakis** 
* **Suhaib Qasim** 
* **Tim Raymond Tun** 
* **Fadi Lazkani** 

---

## 🏗️ Arkitektur & Designprinciper

Projektet tillämpar en **skiktad arkitektur (Layered Architecture)** och strikt **lös koppling (Loose Coupling)**. Genom att separera applikationslogiken från hårdvaran uppnås totalt **hårdvaruoberoende**, vilket gör systemet extremt modulärt, testbart och portabelt.

Följande designmönster och principer har varit styrande i arkitekturen:

### 1. Dependency Inversion Principle (DIP) via Gränssnitt
Applikationslagret (`app::logic::Logic`) kommunicerar *aldrig* direkt med den underliggande hårdvaran eller specifika drivrutiner. Istället sker all interaktion via abstrakta gränssnitt (t.ex. `driver::mqtt::Interface` och `driver::adc::Interface`). 
* **Fördel:** Logiken är helt ovetande om ifall den körs på ett fysiskt mikrorundkort eller i en simulator, vilket eliminerar dolda beroenden.

### 2. Factory Pattern (Fabriksmönster)
För att kapsla in skapandet av drivrutiner används en central fabrik (`Factory`). Fabriken fungerar som systemets sammanfogande länk (Dependency Injection) och ansvarar för att leverera rätt implementationer till logiken baserat på vilken miljö applikationen kompileras för:
* `driver::factory::Esp32s3`: Instansierar de skarpa drivrutinerna för produktion.
* `driver::factory::Stub`: Instansierar de simulerade drivrutinerna för testning.

### 3. Separation of Concerns (Drivers vs. Stubs)
Systemet tillhandahåller två fullständiga uppsättningar av drivrutinslagret:
1. **Produktionsskiktet (`Esp32s3`):** Realiserar gränssnitten mot det fysiska chippet och dess hårdvarustackar via ESP-IDF (t.ex. Wi-Fi, FreeRTOS-trådar, hårdvaru-ADC och UART).
2. **Simuleringsskiktet (`Stub`):** Realiserar samma gränssnitt men i ren mjukvara. Genom smarta implementationer (såsom statiska instans-bakdörrar i MQTT-stubben) kan terminalen på en vanlig dator simulera nätverks- och sensorhändelser i realtid.

### 4. Ren Design med "Tunn Main"
`main.cpp` fungerar uteslutande som applikationens startpunkt (Entry Point). Den innehåller ingen affärslogik, utan dess enda ansvar är att initiera fabriken, konfigurera minnesallokeringen för systemlogiken och starta exekveringstråden (`Logic::run()`). Detta håller startsekvensen ren och lätt att felsöka.

---

## 🛠️ Hur man bygger och kör projektet

### Förutsättningar

* ESP-IDF installerat (v5.x rekommenderas).
* En MQTT-klient (t.ex. **MQTT Explorer**) för att skicka kommandon och se temperaturdata.
* WSL installerat för att köra stubar utan hårdvara.
* ESP32-S3 enhet.
* breadboard med komponenter.

## 🛠️ Hårvaru uppkoppling / Portar
* USB - kommuntiation med USB-Serial/JTAG
* Port A1 - ADC för temperatur läsning.
* Port A3 - LED positiv anslutning(matning).

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
* idf.py add-dependency "espressif/mqtt"
* idf.py reconfigure
* idf.py build

## 3. Flashat till ESP32-S3 och starta serieövervakaren
* idf.py -p "PORT" flash monitor : Val of PORT exempel: COM9

---

## Guide att köra koden i WSL

* Ändra till stub koden i main.

* Öppna windows filer i wsl 
    * cd /mnt/"Skriv sökvägen"

* Körkod för wsl, kopiera och lägg i terminalen.
    * g++ -std=c++20 main/source/main.cpp -D STUB -I main/include -o logic_test && ./logic_test
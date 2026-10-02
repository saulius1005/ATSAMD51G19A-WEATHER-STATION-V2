
# Weather Station v2.0

(Still in progress. Very early version- Alfa of the Alfa :D )

## Wiring Diagram

<img width="640" height="480" alt="atsamd51g19a_weather_station_v_2_0" src="https://github.com/user-attachments/assets/bc5221d7-8a3f-4956-b523-c6776d961844" />

The main purpose of this weather station is to calculate the position of the Sun in the sky based on environmental, location, and time data. The calculated Sun position and other environmental parameters are then transmitted to the solar tracker towers, which return their operating data. All collected and calculated data is finally transmitted to a server and stored in Google Sheets for monitoring, analysis, and further processing.

## Main differences from Weather Station v1.0

1. The MCU is now a 32-bit **ATSAMD51G19A** instead of the 8-bit **AVR64DD32**.
2. The CPU runs at **120 MHz** instead of 24 MHz.
3. **DMA** is used for faster and more efficient data processing.
4. An additional **RS485 data line** is provided for the sensors' separate board, allowing the sensors to be placed farther away from the main board. The older version used USART and I2C, which significantly limited the maximum cable length.
5. Temperature, pressure, and humidity are now measured using a single **BME680** sensor instead of two separate sensors: **SHT21** for humidity and **BMP390** for temperature and pressure.
6. The new 32-bit MCU eliminates the need for a separate 8-bit microcontroller whose only purpose was accurate time calculation. Time calculation is now handled by the main MCU using its **RTC calendar**, providing the same result.
7. The main board power supply can now operate from **14 V to 60 V**.
8. Data transmission to the towers remains unchanged, but the new board can also receive data from them.
9. The new weather station includes a **SIMCom A7672E** module, which is used as a time reference after startup. It initially uses GSM network time and, when available, GNSS time.
10. The A7672E module is also used to transmit data to a server, such as Google Apps Script, which then stores the environmental data, calculated Sun position, and tower data directly in **Google Sheets**.
11. The new weather station has a **240×320 color LCD** instead of the previous 128×64 display. The LCD also has a sleep mode, which activates approximately one minute after the last user interaction.
12. A **touchscreen** is now used for user interaction instead of the previous 3×4 keypad.

---

# Main Weather Station Workflow

After startup, the A7672E module is initialized by configuring its GSM, GNSS, and Internet connectivity settings.

If available, the GSM network time is then read and immediately applied.

Next, the weather station collects sensor data from the separate sensor board through **RS485**.

After the sensor data has been received, the weather station calculates the Sun's position in the sky.

The calculated Sun position is then corrected using atmospheric refraction data, especially when the Sun is close to the horizon.

After the Sun position calculation is completed, the weather station sends data to each tower one by one and waits for a response from each tower.

Finally, all collected data, including the weather station data and tower responses, is transmitted to Google Sheets using the A7672E module over a **4G LTE Cat 1** connection.

---

# Sensor Board Data

The sensor board communicates with the main board using **RS485 at 230400 baud**.

The main board sends a data request:

```text
{GET}
```

The sensor board responds with a frame such as:

```text
[1804023e0b0707e88d]
```

The frame is hexadecimal and contains the following data:

```text
18   - temperature: +24 °C
0402 - pressure: 1026 hPa
3e   - humidity: 76 %
0b   - wind speed: 11 m/s
07   - wind direction: NW
07e8 - sunlight sensor voltage: 2024 mV
8d   - CRC8 CDMA2000
```

The temperature is transmitted as a `uint8_t` value and later interpreted as an `int8_t`.

Wind direction is encoded as follows:

```text
00 - N
01 - NE
02 - E
03 - SE
04 - S
05 - SW
06 - W
07 - NW
```

The sunlight level is currently measured using an **SFH206K** sensor for testing purposes. In the future, it will be replaced with a sensor better suited for visible-light measurement.

If the data frame is corrupted or the CRC is incorrect, the frame is simply ignored.

After valid sensor data has been received, the Sun position calculation begins. The calculation uses the location settings, including latitude, longitude, and altitude, together with the exact current time.

An additional atmospheric refraction calculation is used to improve the accuracy of the calculated Sun position when the Sun is close to the horizon.

---

# Tower Data

Tower communication uses a separate RS485 channel operating at **115200 baud**.

The system supports up to **256 towers**.

An example tower data frame is:

```text
{008918e9940b77e8c3}CRLF
```

The frame contains:

```text
00   - tower ID (00–FF)
8918 - Sun azimuth: 350.96°
e994 - Sun elevation: -57.40°
0b   - wind speed: 11 m/s
07   - wind direction: NW
07e8 - sunlight level: 2024 mV
c3   - CRC8 CDMA2000
```

The Sun elevation value is transmitted as an unsigned value and must be interpreted as a signed value.

If the frame is corrupted or the CRC is incorrect, it is ignored.

## Tower Response

An example tower response frame is:

```text
[004792126807dc3fb025a0370ee15b10]
```

The frame contains:

```text
00  - responding tower ID
4792 - current azimuth: 183.22°
1268 - current elevation: 47.12°
07dc - solar panel voltage: 201.2 V
3fb  - solar panel current: 10.19 A
00   - tower end-switch value
       (not implemented yet; reserved for future use)
25a  - azimuth motor voltage: 60.2 V
037  - azimuth motor current: 0.55 A
0ee  - elevation motor voltage: 23.8 V
15b  - elevation motor current: 3.47 A
10   - CRC8 CDMA2000
```

If the response frame is corrupted or the CRC is incorrect, it is ignored.

After data has been exchanged with all towers, the collected data is transmitted to the server through the A7672E module using USART at **115200 baud** and a **4G LTE Cat 1** Internet connection.

The complete data frame is transmitted once per minute.

The transmitted frame contains the weather station data followed by the response data from every tower.

The weather station data occupies 26 bytes:

```c
USART_RS485_printf(USART_GSM_REG, "%04x%04x%03x%02x%03x%02x%02x%01x|", 
                    (uint16_t)solar_params.coarse_azimuth, 
                    (uint16_t)solar_params.coarse_elevation, 
                    (uint16_t)sensors.SUN.level, 
                    (uint8_t)(sensors.BME680.temperature), 
                    (uint16_t)sensors.BME680.pressure), 
                    (uint8_t)(sensors.BME680.humidity), 
                    (uint8_t)sensors.WIND.speed, 
                    (uint8_t)sensors.WIND.direction);
```

Tower data is then appended to the frame:

```c
for(uint8_t i = 0; i < A7672E_NET.towers_in_total; i++){
    USART_RS485_printf(USART_GSM_REG, "%s|", towers[i].prepared_to_server);
}
```

Each tower contributes 30 bytes of data.

---

# User Interface

The weather station uses a **240×320 ILI9341 color LCD** connected through SPI at **30 MHz**.

Touch input is handled by an **XPT2046** controller using SPI at **2.5 MHz**.

After the board is powered on, the LCD displays the A7672E initialization sequence.

### A7672E Initialization

1. The system waits for startup confirmation messages from the SIMCom A7672E module, such as `SMS Ready`, `PB Ready`, etc.
2. GSM and Internet connectivity functions are then initialized.
3. The final initialization window displays the GNSS initialization sequence.

After the module has been initialized, the main window is displayed.

---

# Main Window

The main window contains seven sections:

## 1. Time

The Time section displays the current time and the source used for time synchronization.

Initially, the time is obtained from GSM. If GNSS becomes available, the time is automatically corrected using GNSS data and the display switches to indicate GNSS as the time source.

Pressing the Time section opens the **Time Settings** window.

The Time Settings window displays additional parameters such as:

* Time zone
* DST status
* Current time source

The user can:

* Reset GSM time
* Reset GNSS time
* Set the time manually
* Press **Auto** to immediately set the time according to the available GSM or GNSS source

Pressing the `<` button returns to the main window.

---

## 2. Weather Data

The Weather Data section displays environmental data:

* Temperature
* Atmospheric pressure
* Humidity
* Wind speed
* Wind direction

Pressing this section does not perform any action.

---

## 3. Sun Data

The Sun Data section displays:

* Sun azimuth
* Sun elevation
* Sunlight level in millivolts

Pressing this section does not perform any action.

---

## 4. Location Data

The Location Data section displays:

* Latitude
* Longitude
* Altitude

Pressing this section opens the **Location Settings** window.

The Location Settings window also displays additional information:

* Current time
* GSM signal strength
* GSM network registration status
* GNSS lock status

The latitude, longitude, and altitude can be changed manually by pressing their corresponding buttons.

If GNSS is available, pressing **Auto** automatically fills all location parameters using GNSS data.

After the parameters have been successfully updated, the updated values are displayed in green for one second.

If GNSS is not available, pressing **Auto** indicates the lack of GNSS location data by displaying the corresponding indication in red.

Pressing `<` returns to the main window.

---

## 5. Network Data

The Network Data section displays:

* GSM signal strength
* Network registration status
* GNSS lock status
* Server response from the latest data exchange

Pressing this section opens the **Network Settings** window.

The Network Settings window allows the user to change:

* APN name for the Internet provider
* Trusted phone number *(not implemented yet; reserved for future improvements)*
* Google Apps Script data-sending address

Pressing `<` returns to the main window.

---

## 6. Towers Data

The Towers Data section displays:

* Total number of towers in the system
* Total solar panel power
* Total azimuth motor power
* Total elevation motor power

Pressing this section opens the **Tower Settings** window.

The Tower Settings window allows the user to:

* View individual tower data
* Set the total number of towers in the system

Pressing `<` returns to the main window.

---

# LCD Sleep Mode

The LCD enters sleep mode approximately one minute after the last touchscreen interaction.

The LCD sleep mode does **not** stop the main system functions.

Regardless of which window is currently open, the following processes continue running:

* Sensor data collection
* Tower data exchange
* Sun position calculation
* Data processing
* Data transmission to the server

The LCD is only used for displaying information and user interaction; it does not interrupt the main weather station workflow.

The server response code **302** is interpreted as a successful response because the A7672E module does not support HTTP redirections.

# 📡 GPS Component for ESP32 (ESP-IDF)

## 🧭 Overview

This project implements a modular GPS driver component for ESP32 using **ESP-IDF**.  
The system communicates with a UART-based GPS module and parses NMEA sentences to extract real-time location and time data.

Designed for reusable integration into embedded and IoT applications.

---

## ⚙️ Features

- UART communication using ESP-IDF driver
- NMEA sentence parsing (GPGGA / GPRMC)
- Real-time extraction of:
  - Latitude
  - Longitude
  - UTC Time
- Modular component structure (`gps.c` / `gps.h`)
- Clean and scalable driver architecture

---

## 🛠️ Hardware Used

- ESP32 Dev Module
- UART-based GPS module (e.g., NEO-6M)

---

## 💻 Software Stack

- ESP-IDF


---

## 🔌 UART Configuration (Example)

| Parameter | Value |
|-----------|--------|
| Baud Rate | 9600 |
| Data Bits | 8 |
| Parity    | None |
| Stop Bits | 1 |



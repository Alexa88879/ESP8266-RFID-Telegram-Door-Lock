# 🚪 ESP8266 RFID Access Control System with Telegram Alerts & Web Interface

A smart, Wi-Fi-enabled door access system built using **ESP8266**, **RFID**, **L298N motor driver**, **OLED Display**, and **Telegram bot integration**. Authorized users can unlock the door by scanning an RFID card or using a secure web interface. All access attempts are logged and notified via Telegram.

---

## 🔧 Features

- ✅ RFID-based secure authentication
- 🌐 Web interface to remotely unlock door
- 📩 Real-time Telegram notifications
- 🖥 OLED display feedback (Access Granted/Denied)
- 🔁 Motor control via L298N for lock mechanism
- 🔒 Unauthorized access alerts
- 📡 Wi-Fi enabled and OTA-capable

---

## 🛠️ Hardware Components

| Component             | Quantity |
|-----------------------|----------|
| ESP8266 NodeMCU       | 1        |
| RC522 RFID Reader     | 1        |
| L298N Motor Driver    | 1        |
| Servo/DC Motor        | 1        |
| OLED Display (SSD1306)| 1        |
| 12V Battery Pack      | 1        |
| Connecting Wires      | As needed |

---

## 🔌 Hardware Connections

### 📟 **L298N Motor Driver → ESP8266**
| L298N Pin | ESP8266 Pin |
|-----------|-------------|
| ENA       | D0          |
| IN1       | D8          |
| IN2       | RX (GPIO3)  |
| OUT1      | Servo (+)   |
| OUT2      | Servo (–)   |
| 12V       | Battery +   |
| GND       | Battery –   |

---

### 🖥 **OLED Display (SSD1306) → ESP8266**
| OLED Pin | ESP8266 Pin |
|----------|-------------|
| SDA      | D2          |
| SCL      | D1          |
| VDD      | 3V          |
| GND      | GND         |

---

### 📡 **RC522 RFID Reader → ESP8266**
| RFID Pin | ESP8266 Pin |
|----------|-------------|
| SDA      | D4          |
| SCK      | D5          |
| MOSI     | D7          |
| MISO     | D6          |
| RST      | D3          |
| 3.3V     | 3V          |
| GND      | GND         |

---

## 🧠 How It Works

1. RFID card is scanned.
2. UID is checked against authorized list.
3. If authorized:
   - Motor is activated (door unlocks).
   - OLED shows **Access Granted**.
   - Telegram notification is sent.
4. If unauthorized:
   - OLED shows **Access Denied**.
   - Telegram alert is sent.
5. Optionally, the door can be unlocked via web interface hosted on ESP8266.

---

## 📲 Telegram Bot Setup

1. Create a new bot using [BotFather](https://t.me/botfather).
2. Save the **Bot Token**.
3. Start a chat with your bot and send a message.
4. Use [@userinfobot](https://t.me/userinfobot) to get your **chat ID**.
5. Replace the placeholders in the code:
   ```cpp
   String botToken = "<your_bot_token>";
   String chatId = "<your_chat_id>"; the webpage one in new page
when link click

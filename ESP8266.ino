#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>

// WiFi credentials
const char* ssid = "YourWiFiName";
const char* password = "YourWiFiPassword";

// Telegram Bot credentials
String botToken = "<bot auth token>";
String chatId = "<chat id>";

// HTTP server
ESP8266WebServer server(80);

// OLED display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// RFID RC522 setup
#define SS_PIN D4
#define RST_PIN D3
MFRC522 mfrc522(SS_PIN, RST_PIN);

// L298N motor control
#define ENA D0
#define IN1 D8
#define IN2 3


// Authorized UID
byte allowedUID[4] = {0x63, 0x9, 0x31, 0x3};//Enter The Uid in HEX FORMATE You can read from by writting esp8266 code or if you mobile capable you can read from mobile

// Function to send Telegram message
void sendTelegramMessage(String message) {
  WiFiClientSecure client;
  client.setInsecure();

  if (!client.connect("api.telegram.org", 443)) {
    Serial.println("Connection to Telegram failed");
    return;
  }

  String url = "/bot" + botToken + "/sendMessage?chat_id=" + chatId + "&text=" + message;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.telegram.org\r\n" +
               "Connection: close\r\n\r\n");

  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
  Serial.println("Telegram message sent");
}

void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  //pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  //digitalWrite(BUZZER_PIN, LOW);

  Wire.begin(D2, D1);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED init failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.println("Connecting WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi. IP: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setCursor(10, 20);
  display.println("WiFi Connected");
  display.display();
  delay(1000);

  SPI.begin();
  mfrc522.PCD_Init();

  display.clearDisplay();
  display.setCursor(10, 20);
  display.println("Scan RFID Card");
  display.display();

  // Web server root page
  server.on("/", []() {
    String html = "<!DOCTYPE html><html><head><title>RFID Access Control</title>";
    html += "<style>body{font-family:sans-serif;text-align:center;padding-top:30px;}button{font-size:20px;padding:15px 30px;margin:20px;border:none;background:#2196F3;color:white;border-radius:10px;}button:hover{background:#0b7dda;}</style>";
    html += "</head><body>";
    html += "<h2>ESP8266 RFID Access System</h2>";
    html += "<button onclick=\"fetch('/unlock')\">Unlock Door</button>";
    html += "</body></html>";

    server.send(200, "text/html", html);
  });

  // Web server unlock endpoint
  server.on("/unlock", []() {
    Serial.println("HTTP Unlock Triggered");
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 20);
    display.println("UNLOCKED");
    display.display();

    sendTelegramMessage("Door unlocked via Web.");

    digitalWrite(ENA, HIGH);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    delay(2000);
    digitalWrite(ENA, LOW);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    delay(2000);
    display.clearDisplay();
    display.setCursor(10, 20);
    display.println("Scan RFID Card");
    display.display();

    server.send(200, "text/plain", "Door Unlocked");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

  Serial.print("UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  bool match = true;
  for (byte i = 0; i < 4; i++) {
    if (mfrc522.uid.uidByte[i] != allowedUID[i]) {
      match = false;
      break;
    }
  }

  if (match) {
    Serial.println("Access Granted");
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 20);
    display.println("GRANTED");
    display.display();

    //digitalWrite(BUZZER_PIN, HIGH);
   // delay(300);
    //digitalWrite(BUZZER_PIN, LOW);

    digitalWrite(ENA, HIGH);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    delay(2000);
    digitalWrite(ENA, LOW);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    sendTelegramMessage("Access Granted to authorized card.");

    delay(2000);
    display.clearDisplay();
    display.setCursor(10, 20);
    display.println("Scan RFID Card");
    display.display();
  } else {
    Serial.println("Access Denied");
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 20);
    display.println("DENIED");
    display.display();

    sendTelegramMessage("Access Denied to unauthorized card.");

    delay(2000);
    display.clearDisplay();
    display.setCursor(10, 20);
    display.println("Scan RFID Card");
    display.display();
  }

  mfrc522.PICC_HaltA();
  while (mfrc522.PICC_IsNewCardPresent() || mfrc522.PICC_ReadCardSerial()) delay(50);
} 

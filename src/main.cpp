#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
 
#define ENABLE_USER_AUTH
#define ENABLE_DATABASE
#include <FirebaseClient.h>
 
// ========= CONFIG =========
const char *ssid = "UW MPSK";
const char *password = "6w59AePGC5aWJGiv";
 
#define DATABASE_URL "https://battery-management-lab-yan-default-rtdb.firebaseio.com/"
#define API_KEY      "AIzaSyDjE4KJ9bopshe7DoJKDXQYUYBiL3r5NBU"
#define USER_EMAIL   "zy901103@uw.edu"
#define USER_PASSWORD "Zy_901103"
 
// Timeline for 5 modes in ~60 seconds
#define IDLE_MS 10000              // Mode 1: Idle - 10s
#define ULTRASONIC_ONLY_MS 10000   // Mode 2: Ultrasonic only - 10s
// WiFi connect + auth ~5s
#define ULTRA_WIFI_MS 12000        // Mode 3: Ultrasonic + WiFi - 12s
#define ULTRA_WIFI_FB_MS 15000     // Mode 4: Ultrasonic + WiFi + Firebase - 15s
#define DEEP_SLEEP_MS 10000        // Mode 5: Deep Sleep - 10s
// Total: 10+10+5+12+15+10 = 62s ≈ 1 minute
 
#define MAX_WIFI_RETRIES 10
 
int uploadIntervalMs = 1000;
 
// ========= HC-SR04 PINS =========
const int trigPin = D2;
const int echoPin = D3;
const float soundSpeed = 0.034f;
 
// ========= FIREBASE CLIENT OBJECTS =========
UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);
FirebaseApp app;
 
WiFiClientSecure ssl;
using AsyncClient = AsyncClientClass;
AsyncClient asyncClient(ssl);
 
RealtimeDatabase Database;
 
// ========= TIMING =========
unsigned long lastUploadMs = 0;
 
// ========= HELPERS =========
float measureDistance();
void connectToWiFi();
void initFirebase();
void sendDataToFirebase(float distance);
void processData(AsyncResult &aResult);
 
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n\n========== 5 Power Modes Test ==========\n");
 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 
  unsigned long totalStart = millis();
 
  // ======== MODE 1: Idle ESP32 ========
  Serial.println(">>> MODE 1: Idle ESP32 (10s)");
  Serial.println("    - No WiFi, no ultrasonic\n");
  unsigned long start = millis();
  while (millis() - start < IDLE_MS) {
    delay(50);
  }
  Serial.printf("Mode 1 done. Elapsed: %lus\n\n", (millis()-totalStart)/1000);
 
  // ======== MODE 2: Ultrasonic only ========
  Serial.println(">>> MODE 2: Ultrasonic sensor only (10s)");
  Serial.println("    - WiFi OFF, ultrasonic ON\n");
  start = millis();
  while (millis() - start < ULTRASONIC_ONLY_MS) {
    measureDistance();
    delay(100);
  }
  Serial.printf("Mode 2 done. Elapsed: %lus\n\n", (millis()-totalStart)/1000);
 
  // -------- WiFi Connect --------
  Serial.println(">>> Connecting WiFi + Firebase...");
  connectToWiFi();
  
  initFirebase();
  
  Serial.println("Waiting for Firebase authentication...");
  unsigned long authStart = millis();
  while (!app.ready() && millis() - authStart < 10000) {
    app.loop();
    delay(100);
  }
  
  if (app.ready()) {
    Serial.println("✅ Firebase ready!\n");
  } else {
    Serial.println("❌ Firebase timeout!\n");
  }
  Serial.printf("Auth done. Elapsed: %lus\n\n", (millis()-totalStart)/1000);
 
  // ======== MODE 3: Ultrasonic + WiFi (no Firebase) ========
  Serial.println(">>> MODE 3: Ultrasonic + WiFi (12s)");
  Serial.println("    - Ultrasonic ON, WiFi ON, no Firebase sending\n");
  start = millis();
  while (millis() - start < ULTRA_WIFI_MS) {
    app.loop(); // maintain WiFi connection
    measureDistance();
    delay(100);
  }
  Serial.printf("Mode 3 done. Elapsed: %lus\n\n", (millis()-totalStart)/1000);
 
  // ======== MODE 4: Ultrasonic + WiFi + Firebase ========
  Serial.println(">>> MODE 4: Ultrasonic + WiFi + Firebase (15s)");
  Serial.println("    - Ultrasonic ON, WiFi ON, sending to Firebase\n");
  
  start = millis();
  int sendCount = 0;
  while (millis() - start < ULTRA_WIFI_FB_MS) {
    app.loop();
    
    float d = measureDistance();
    
    if (app.ready()) {
      sendDataToFirebase(d);
      sendCount++;
    }
    
    delay(50);
  }
  Serial.printf("Mode 4 done. Sent %d times. Elapsed: %lus\n\n", sendCount, (millis()-totalStart)/1000);
 
  // ======== MODE 5: Deep Sleep ========
  Serial.println(">>> MODE 5: Deep Sleep (10s)");
  Serial.printf("Total elapsed: %lus\n", (millis()-totalStart)/1000);
  Serial.println("💤 Going to deep sleep...\n");
  
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
 
  esp_sleep_enable_timer_wakeup((uint64_t)DEEP_SLEEP_MS * 1000ULL);
  delay(100);
  esp_deep_sleep_start();
}
 
void loop() {
  // Not used
}
 
// ====== Ultrasonic ======
float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  long duration = pulseIn(echoPin, HIGH, 30000);
  
  if (duration == 0) {
    return 0;
  }
  
  float distance = duration * soundSpeed / 2.0f;
  return distance;
}
 
// ====== WiFi ======
void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  Serial.print("Connecting");
  int cnt = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    cnt++;
    if (cnt > MAX_WIFI_RETRIES) {
      Serial.println("\n❌ WiFi failed");
      ESP.restart();
    }
  }
  Serial.println("\n✅ WiFi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}
 
// ====== Firebase ======
void initFirebase() {
  ssl.setInsecure();
  ssl.setHandshakeTimeout(10);
 
  initializeApp(asyncClient, app, getAuth(user_auth), processData, "authTask");
  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);
}
 
void sendDataToFirebase(float distance) {
  unsigned long now = millis();
  if (now - lastUploadMs < (unsigned long)uploadIntervalMs) return;
  lastUploadMs = now;
  
  Database.set<float>(
    asyncClient,
    "/test/distance",
    distance,
    processData,
    "RTDB_SetDistance"
  );
}
 
// ====== Async result handler ======
void processData(AsyncResult &aResult) {
  if (!aResult.isResult()) return;
 
  if (aResult.isEvent()) {
    Serial.printf("  [Event] %s: %s (code: %d)\n",
                    aResult.uid().c_str(),
                    aResult.eventLog().message().c_str(),
                    aResult.eventLog().code());
  }
 
  if (aResult.isError()) {
    Serial.printf("  [ERROR] %s: %s (code: %d)\n",
                    aResult.uid().c_str(),
                    aResult.error().message().c_str(),
                    aResult.error().code());
  }
 
  if (aResult.available()) {
    Serial.printf("  [Success] %s\n", aResult.uid().c_str());
  }
}
 
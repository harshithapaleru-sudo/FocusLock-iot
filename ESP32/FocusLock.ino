#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// FocusLock IoT - ESP32 Controller

#define RED_LED 18
#define GREEN_LED 19
#define BUZZER 23

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(
SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET
);

const char* ssid = "FocusLock";
const char* password = "focus1234";

WebServer server(80);

bool youtubeRestricted = false;
bool instagramRestricted = false;
bool gamesRestricted = false;
bool studyRunning = false;

unsigned long studyStart = 0;
const unsigned long STUDY_DURATION = 60000;

int points = 0;

void beep(int duration) {
digitalWrite(BUZZER, HIGH);
delay(duration);
digitalWrite(BUZZER, LOW);
}

void updateHardware() {
bool restricted =
youtubeRestricted ||
instagramRestricted ||
gamesRestricted;

digitalWrite(RED_LED, restricted ? HIGH : LOW);
digitalWrite(GREEN_LED, restricted ? LOW : HIGH);

display.clearDisplay();
display.setTextColor(SSD1306_WHITE);
display.setTextSize(1);
display.setCursor(0, 0);

display.println("FOCUSLOCK IoT");
display.println("----------------");
display.print("YouTube: ");
display.println(youtubeRestricted ? "LOCKED" : "OPEN");
display.print("Instagram: ");
display.println(instagramRestricted ? "LOCKED" : "OPEN");
display.print("Games: ");
display.println(gamesRestricted ? "LOCKED" : "OPEN");

display.println("----------------");
display.print("Study: ");
display.println(studyRunning ? "RUNNING" : "READY");
display.print("Points: ");
display.println(points);

display.display();
}

String makePage() {
String page = "<!DOCTYPE html><html><head>";
page += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
page += "<title>FocusLock IoT</title>";
page += "<style>";
page += "body{background:#071426;color:white;font-family:Arial;text-align:center;padding:15px}";
page += ".card{background:#10243b;padding:18px;margin:12px auto;border-radius:14px;max-width:400px}";
page += "button{padding:13px;margin:6px;border:0;border-radius:8px;background:#22c7a9;font-weight:bold}";
page += "</style></head><body>";
page += "<h2>FOCUSLOCK IoT</h2>";
page += "<div class='card'><h3>App Restriction Demo</h3>";

page += "<p>YouTube: ";
page += youtubeRestricted ? "LOCKED" : "OPEN";
page += "</p><a href='/youtube'><button>Toggle YouTube</button></a>";

page += "<p>Instagram: ";
page += instagramRestricted ? "LOCKED" : "OPEN";
page += "</p><a href='/instagram'><button>Toggle Instagram</button></a>";

page += "<p>Games: ";
page += gamesRestricted ? "LOCKED" : "OPEN";
page += "</p><a href='/games'><button>Toggle Games</button></a>";

page += "</div><div class='card'><h3>Study Session</h3>";
page += "<p>Status: ";
page += studyRunning ? "RUNNING" : "READY";
page += "</p><p>Points: " + String(points) + "</p>";
page += "<a href='/study'><button>Start 1-Minute Study</button></a>";
page += "</div>";
page += "<p>FocusLock IoT Project</p>";
page += "</body></html>";

return page;
}

void handleHome() {
server.send(200, "text/html", makePage());
}

void handleYoutube() {
youtubeRestricted = !youtubeRestricted;
beep(100);
updateHardware();
server.sendHeader("Location", "/");
server.send(303);
}

void handleInstagram() {
instagramRestricted = !instagramRestricted;
beep(100);
updateHardware();
server.sendHeader("Location", "/");
server.send(303);
}

void handleGames() {
gamesRestricted = !gamesRestricted;
beep(100);
updateHardware();
server.sendHeader("Location", "/");
server.send(303);
}

void handleStudy() {
if (!studyRunning) {
studyRunning = true;
studyStart = millis();
beep(150);
updateHardware();
}

server.sendHeader("Location", "/");
server.send(303);
}

void setup() {
Serial.begin(115200);

pinMode(RED_LED, OUTPUT);
pinMode(GREEN_LED, OUTPUT);
pinMode(BUZZER, OUTPUT);

digitalWrite(BUZZER, LOW);

Wire.begin(21, 22);

if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
Serial.println("OLED not found. Check wiring.");
} else {
display.clearDisplay();
display.display();
}

WiFi.softAP(ssid, password);

Serial.println("FocusLock WiFi Started");
Serial.print("Dashboard: http://");
Serial.println(WiFi.softAPIP());

server.on("/", handleHome);
server.on("/youtube", handleYoutube);
server.on("/instagram", handleInstagram);
server.on("/games", handleGames);
server.on("/study", handleStudy);

server.begin();

updateHardware();
}

void loop() {
server.handleClient();

if (studyRunning &&
millis() - studyStart >= STUDY_DURATION) {
studyRunning = false;
points += 10;
beep(500);
updateHardware();
}
}

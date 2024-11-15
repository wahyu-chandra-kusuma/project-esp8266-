#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Konfigurasi LED
const int ledPin = 5;

// Inisialisasi Web Server di port 80
ESP8266WebServer server(80);

// Ganti dengan SSID dan password WiFi Anda
const char* ssid = "Redmi Note 10S";
const char* password = "chandraak123";

// Fungsi untuk menangani perubahan PWM
void handlePWM() {
  if (server.hasArg("value")) {
    int pwmValue = server.arg("value").toInt();
    analogWrite(ledPin, pwmValue);
  }
  server.send(200, "text/plain", "OK");
}

// Fungsi untuk menangani permintaan halaman web
void handleRoot() {
  String html = "<html>\
  <head>\
  <meta name='viewport' content='width=device-width, initial-scale=1'/>\
  <title>Kendalikan LED</title>\
  <style>body { font-family: Arial; text-align: center; }\
  input[type=range] { width: 300px; }</style>\
  </head>\
  <body>\
  <h1>Kendalikan Terang LED</h1>\
  <p><b>Nilai PWM: <span id='pwmValue'>0</span></b></p>\
  <input type='range' min='0' max='1023' value='0' id='slider' onchange='sendPWM(this.value)'/>\
  <script>\
    var slider = document.getElementById('slider');\
    var pwmValueText = document.getElementById('pwmValue');\
    function sendPWM(value) {\
      pwmValueText.innerText = value;\
      var xhr = new XMLHttpRequest();\
      xhr.open('GET', '/setPWM?value=' + value, true);\
      xhr.send();\
    }\
  </script>\
  </body>\
  </html>";

  server.send(200, "text/html", html);
}

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);

  // Konfigurasi pin LED sebagai output
  pinMode(ledPin, OUTPUT);
  
  // Hubungkan ke WiFi
  Serial.print("Menghubungkan ke ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi terhubung");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());

  // Set halaman web
  server.on("/", handleRoot);
  server.on("/setPWM", handlePWM);  // Ini dipindahkan ke setup()

  // Memulai server
  server.begin();
  Serial.println("Server dimulai");
}

void loop() {
  server.handleClient();
}
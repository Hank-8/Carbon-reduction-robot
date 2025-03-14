#include <MatrixMiniR4.h>
#include "WiFiS3.h"


// WiFi 網路設定
char ssid[] = "Smart_dog";
char pass[] = "12345678";
int status = WL_IDLE_STATUS;
WiFiServer server(80);


int a1Read;
int sum_temp;
int temp = 0;


void setup() {
  MiniR4.OLED.clearDisplay();
  MiniR4.begin();
  Serial.begin(115200);
  initializeWiFi();
  server.begin();
}

void loop() {
  getTemp();
  handleClientRequests();
}

void initializeWiFi() {
  Serial.println("Initializing WiFi...");
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFi module communication failed!");
    while (true)
      ;
  }

  //WIFI IP設定
  WiFi.config(IPAddress(192, 168, 4, 1));

  //WIFI韌體確認
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) Serial.println("Please upgrade the firmware");

  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    while (true)
      ;
  }
  // 等待十秒:
  delay(6000);
}


// 網頁處理
void handleClientRequests() {
  WiFiClient client = server.available();  // listen for incoming clients

  if (client) {                    // if you get a client,
    Serial.println("new client");  // print a message out the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    while (client.connected()) {   // loop while the client's connected
      delayMicroseconds(10);       // This is required for the Arduino Nano RP2040 Connect - otherwise it will loop so fast that SPI will never be served.
      if (client.available()) {    // if there's bytes to read from the client,
        char c = client.read();    // read a byte, then
        Serial.write(c);           // print it out to the serial monitor
        if (c == '\n') {           // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");  // the connection will be closed after completion of the response
            client.println("Refresh: 3");         // refresh the page automatically every 3 sec
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head>");
            client.println("<meta charset=\"UTF-8\">");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("<style>");
            client.println("  body { font-family: Arial, sans-serif; text-align: center; color: #333; margin: 0; padding: 0; background-color: #f4f4f9; }");
            client.println("  h1 { color: #5a5d5e; margin-top: 20px; font-size: 8vw; }");
            client.println("  .data { font-size: 6vw; color: #555; margin: 15px 0; }");
            client.println("  .link { font-size: 6vw; color: #007bff; text-decoration: none; margin-top: 20px; display: inline-block; }");
            client.println("  .link:hover { color: #0056b3; }");
            client.println("</style>");
            client.println("</head>");
            client.println("<body>");
            client.println("<h1>減碳巡邏機器人</h1>");
            client.print("<div class=\"data\">現在產線溫度: " + String(temp) + "°C</div>");
            client.print("<div class=\"data\">建議: " + String((temp>35) ? "環境溫度異常" : "環境溫度正常") + "</div>");
            client.print("</div>");
            client.println("</body>");
            client.println("</html>");
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void getTemp() {
  for(int i0=0;i0<10;i0++)
  {
    sum_temp += MiniR4.A1.getAIR();
    delay(5);
  }
  temp = map(sum_temp/10,0,1023,15,75);
  sum_temp = 0;
}




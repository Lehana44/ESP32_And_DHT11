#include <WiFi.h>
#include <HTTPClient.h>

char* ssid = "SAMSUNG J4";
char* password = "ROBOT1234";

const char* device_id = "Cube123";
const char* sensor_id = "temp";

  void connectWifi(){
  WiFi.begin(ssid, password); //Initiate the wifi connection here with the credentials earlier preset
  
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  return;
}

  void loop(){
  float randomValue = random(10, 30);
  sendDataToWaziCloud(randomValue);
  delay(300000); // Wait for 5 minutes (in ms) to send the next generated random value
}

  void sendDataToWaziCloud(float value) {
  
  // We cancel the send process if our board is not yet connectedd to the internet, and try reconnecting to wifi again.
    if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected.");
    connectWifi();
    return;
  }
  
  HTTPClient http;
  
  // Initialize the API endpoint to send data. This endpoint is responsible for receiving the data we send
  String endpoint = "https://api.waziup.io/api/v2/devices/" + String(device_id) + "/sensors/" + String(sensor_id) + "/value";
  http.begin(endpoint);
  
  // Header content for the data to send
  http.addHeader("Content-Type", "application/json;charset=utf-8");
  http.addHeader("accept", "application/json;charset=utf-8");
  
  // Data to send
  String data = "{ \"value\": " + String(value) + " }";
  
  int httpResponseCode = http.POST(data);
  
    if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("Response: " + response);
    } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
}


  void setup() {
  Serial.begin(115200); //Set the baudrate to the board youre using (115200 is fine)
  delay(100);
  connectWifi();
}

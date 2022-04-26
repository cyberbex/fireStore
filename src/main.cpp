#include <Arduino.h>

// This example shows how to set the server value (timestamp) to document field, update and dellete the document. This operation required Email/password, custom or OAUth2.0 authentication.

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Bruno Migliorini"
#define WIFI_PASSWORD "edicula88"

/* 2. Define the API Key */
#define API_KEY "AIzaSyBPsaK5CoraOrSvRIn1nODDb9pcm77QEMI"

/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "fir-d3d7c"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "cyberbex@gmail.com"
#define USER_PASSWORD "esp32demo"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long dataMillis = 0;
double temp = 21.5, humi = 84.5;

void setup()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);
}

void loop()
{

  // Firebase.ready() should be called repeatedly to handle authentication tasks.

  if (Firebase.ready() && (millis() - dataMillis > 60000 || dataMillis == 0))
  {
    dataMillis = millis();

    String documentPath = "House/Room_1";
    FirebaseJson content;

    content.set("fields/temperatura/doubleValue", String(temp).c_str());
    content.set("fields/humidity/doubleValue", String(humi).c_str());

    if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw()))
    {
      Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
      return;
    }
    else
    {
      Serial.println(fbdo.errorReason());
    }
  }
}
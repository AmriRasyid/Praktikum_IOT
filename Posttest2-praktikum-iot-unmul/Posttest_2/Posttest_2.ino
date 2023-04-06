#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define WIFI_SSID "Universitas Mulawarman"
#define WIFI_PASSWORD ""

#define BOT_TOKEN "6204748487:AAFb62ne75ta3FderZAUbR4k-Yo9dinIXM4"

const unsigned long BOT_MTBS = 1000;

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // last time messages' scan has been done

#define LEDPIN D6
#define LEDPIN1 D7
#define LEDPIN2 D8
int ledStatus = 0;

void handleNewMessages(int numNewMessages) {
  Serial.print("Handle New Messages: ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";
    
    if (text == "/ledon") {
      bot.sendMessage(chat_id, "Led is ON", "");
      digitalWrite(LEDPIN, HIGH);
      delay(2000);
      digitalWrite(LEDPIN1, HIGH);
      delay(2000);
      
      digitalWrite(LEDPIN, LOW);
      digitalWrite(LEDPIN1, LOW);
    
      digitalWrite(LEDPIN2, HIGH);
      delay(3000);
      digitalWrite(LEDPIN2, LOW);
      delay(500);
    
      digitalWrite(LEDPIN2, HIGH);
      delay(500);
      digitalWrite(LEDPIN2, LOW);
      delay(500);
    
      digitalWrite(LEDPIN2, HIGH);
      delay(500);
      digitalWrite(LEDPIN2, LOW);
      delay(500);
      ledStatus = 1;
        
      bot.sendMessage(chat_id, "Led is ON", "");
    }

    if (text == "/ledoff") {
      bot.sendMessage(chat_id, "Led is OFF", "");
      ledStatus = 0;
      digitalWrite(LEDPIN, LOW); // turn the LED off (LOW is the voltage level)
      digitalWrite(LEDPIN1, LOW);
      digitalWrite(LEDPIN2, LOW);
      bot.sendMessage(chat_id, "Led is OFF", "");
    }

    if (text == "/ledstatus") {
      String message = "`LED Status: ";
      if(ledStatus == 1) {
        message += "ON";
      } else {
        message += "OFF";
      }
      
      bot.sendMessage(chat_id, message, "");
    }
    if (text == "/start") {
      String welcome = "Welcome to IoT Telegram Bot, " + from_name + ".\n";
      welcome += "List of Commands:\n\n";
      welcome += "/ledon : Menyalakan Lampu Lalu Lintas\n";
      welcome += "/ledoff : Mematikan Lampu Lalu Lintas\n";
      welcome += "/ledstatus : Cek Status Lampu\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  
  pinMode(LEDPIN,OUTPUT); //merah
  pinMode(LEDPIN1,OUTPUT); //kuning
  pinMode(LEDPIN2,OUTPUT); //hijau

  // attempt to connect to Wifi network:
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Check NTP/Time, usually it is instantaneous and you can delete the code below.
  Serial.print("Retrieving time: ");
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop()
{
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    
    while (numNewMessages) {
      Serial.println("Got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}

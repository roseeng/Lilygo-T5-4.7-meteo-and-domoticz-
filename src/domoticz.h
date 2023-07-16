
/// domoticz
char *host = "192.168.2.50"; // "kronos2.roseen.local";  // your own Domoticz server IP
int httpPort = 8080;         // 80;

const int wash = 45;    // washing machine icon at 45°
const int disher = 550; // disher icon at 550W

/// list of values to print : {{"name","IDX","codeJson"},{"Prod","28","CounterToday"}, {} }  // {} --> free zone

// max values
const int max_val = 12;
// Väsby
/*const String domoticz_IDX[max_val][3] = {
  {"Ute 1","1","Data"},{},
  {"Sovrum","3","Data"},{},
  {"Ute 2","6","Data"},{},
  {"Vardagsrum", "4", "Data"},{},
  {},{}
  } ;
*/

// Gåsö
const String domoticz_IDX[max_val][3] = {
    {"Ute temp", "169", "Data"},{},{"Belysning", "21", "Data"},
    {"Storstugan", "171", "Data"},{},{"Värme", "168", "Data"},
    {"Friggebod", "233", "Data"},{},{"Värme", "167", "Data"},
    {"Strömförbrukning", "247", "Data"}
    };
String domoticz_result[max_val];

String json_domoticz(char *host, int httpPort, String idx)
{

  WiFiClient client;

  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return "connection failed";
  }

  String url = "/json.htm?type=devices&rid=" + idx;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('OK');
  Serial.println("=======DATA ===");
  return line;
}

/////////////////// get "data" in json

void get_data_domoticz()
{
  String json_data;
  const size_t capacity = 2 * JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(16) + JSON_OBJECT_SIZE(50) + 1270;
  DynamicJsonDocument doc(capacity);

  // récupération des données dans domoticz
  for (int i = 0; i <= max_val; i++)
  {

    if (domoticz_IDX[i][0] != NULL)
    {
      json_data = json_domoticz(host, httpPort, domoticz_IDX[i][1]);
      DeserializationError error = deserializeJson(doc, json_data);
      JsonObject root = doc.as<JsonObject>();
      JsonObject result0 = root["result"][0];
      const char *clean_data = result0[domoticz_IDX[i][2]];
      domoticz_result[i] = clean_data;
      //  Serial.println(domoticz_result[i]);
      Serial.println(clean_data);
    }
  }
}

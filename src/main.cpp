
//Version 05/01/2024



#include <WiFi.h>
#include <ModbusIP_ESP8266.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "Version.h"


const int HREG_BT = 100;
const int HREG_ET = 101;
const int HREG_ROR = 102;
const int HREG_QUEM = 103;
const int HREG_TAMB = 104;
const int HREG_SOPL = 105;

const char* ssid = "RoasterDR1";
const char* pass = "Roaster2021*";


char comandoFromAtmega_c[50] = "";
char ssid_w[50] = "";
char pass_w[50] = "";
char ssid_s[50] = "";
char pass_s[50] = "";
unsigned long ts;
unsigned long tiempoConexion = 0;
unsigned long actualizaciones = 0;
unsigned long cuentaArtisan = 0L;
bool statusModbus;
bool statusDelayConexion = false;
long tiempo;
bool flatWifiOFF = false;
bool flatSocketOFF = false;
bool redclaveOK = false;

StaticJsonDocument<200> doc;
StaticJsonDocument<200> doc2;
StaticJsonDocument<40> doc3;
StaticJsonDocument<40> doc4;
StaticJsonDocument<200> doc5;
WiFiServer server(80);
WebSocketsServer webSocket = WebSocketsServer(8080);
ModbusIP mb;

struct valoresOperativos{
  int16_t temp_ET = 1230;
  int16_t temp_BT = 1580;
  int16_t porcent_Quem = 100;
  int16_t porcent_Tamb = 80;
  int16_t porcent_Sopl = 60;
  int16_t RoR;
  int16_t deltaETBT;
  uint8_t canalwifi = 1;
  uint32_t versionESP = 0;
}vaOP;

struct valoresControl{
  int16_t aire;
  int16_t tambor;
  int16_t quemador;
}vaCO;

bool send_version_once = false;


void checkSerial();
void checkStatusWiFi();
void onDelayConexion();
void onModbusOverWifi();
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
void crearSocket(bool);
void checkNetwork();


void setup() {
    Serial.begin(115200);
    Serial.setTimeout(100);

    vaOP.versionESP = (completeVersion[0] - 48) * 10000000L +
                        (completeVersion[1] - 48) * 1000000L +
                        (completeVersion[2] - 48) * 100000L +
                        (completeVersion[3] - 48) * 10000L +
                        (completeVersion[4] - 48) * 1000L +
                        (completeVersion[5] - 48) * 100L +
                        (completeVersion[6] - 48) * 10L +
                        (completeVersion[7] - 48) * 1L;


    ts = millis();
    statusModbus = false;
}

void loop() {
    //uint32_t t1 = micros();
    checkSerial();
    webSocket.loop();

    if(statusDelayConexion){
        onDelayConexion();
    }
    if(statusModbus){
        
        mb.task();
        
        mb.Hreg(HREG_BT, vaOP.temp_BT);
        mb.Hreg(HREG_ET, vaOP.temp_ET);
        mb.Hreg(HREG_ROR, vaOP.RoR);
        mb.Hreg(HREG_QUEM, vaOP.porcent_Quem);
        mb.Hreg(HREG_TAMB, vaOP.porcent_Tamb);
        mb.Hreg(HREG_SOPL, vaOP.porcent_Sopl);
        
        if (millis() > ts + 2000) {
        ts = millis();
        checkStatusWiFi();
        }
    }
    delay(10);

    if(!send_version_once)
    {
        Serial.print("ESPV,");Serial.print(vaOP.versionESP);Serial.print('\0');
        send_version_once = true;
    }

    //Serial.println(micros()-t1);
}

void checkStatusWiFi(){
  if(WiFi.status() != WL_CONNECTED && !flatWifiOFF){
    flatWifiOFF = true;
    Serial.print('O');Serial.print('F');Serial.print('W');Serial.print('\0');
  }
  if(!server.isRunning() && !webSocket.isRunning() && !flatSocketOFF){
    flatSocketOFF = true;
    Serial.print('O');Serial.print('F');Serial.print('S');Serial.print('\0');
  }
}

void checkSerial(){
  int i=0;
  int caracter;
  while (Serial.available()){
    caracter = Serial.read();
    if(caracter == '\0' || caracter == '\n' || caracter == '\r'){
      comandoFromAtmega_c[i] = '\0';
      break;
    }
    if(i > 48)break;
    comandoFromAtmega_c[i] = caracter;
    i++;
    delay(2);
  }
  if(comandoFromAtmega_c[0] != '\0'){
    //Serial.print(comandoFromAtmega_c);
  }
  if(comandoFromAtmega_c[0] == 'M' && comandoFromAtmega_c[1] == 'C' && comandoFromAtmega_c[2] == 'A'){
    doc3["pushMessage"] = "startRoasting";
    String output;
    serializeJson(doc3, output);
    webSocket.sendTXT(0, output);
    //Serial.print("Json 1: ");Serial.println(output);
    //Serial.print("startRoasting");Serial.print('\0');
    memset(comandoFromAtmega_c, 0, 50);
  }
  if(comandoFromAtmega_c[0] == 'M' && comandoFromAtmega_c[1] == 'D' && comandoFromAtmega_c[2] == 'R'){
    doc4["pushMessage"] = "endRoasting";
    String output;
    serializeJson(doc4, output);
    webSocket.sendTXT(0, output);
    //Serial.print("Json 2: ");Serial.println(output);
    //Serial.print("endRoasting");Serial.print('\0');
    memset(comandoFromAtmega_c, 0, 50);
  }
  if(comandoFromAtmega_c[0] == 'M' && comandoFromAtmega_c[1] == 'F' && comandoFromAtmega_c[2] == 'C'){
    doc5["pushMessage"] = "addEvent";
    doc5["data"]["event"] = "firstCrackBeginningEvent";
    String output;
    serializeJson(doc5, output);
    webSocket.sendTXT(0, output);
    //Serial.print("Json 3: ");Serial.println(output);
    memset(comandoFromAtmega_c, 0, 50);
  }
  if(comandoFromAtmega_c[0] == 'S' && comandoFromAtmega_c[1] == 'O' && comandoFromAtmega_c[2] == 'C' && comandoFromAtmega_c[3] == 'K' && comandoFromAtmega_c[4] == 'E' && comandoFromAtmega_c[5] == 'T'){
    crearSocket(false);
    memset(comandoFromAtmega_c, 0, 50);

  }
  if(comandoFromAtmega_c[0] == 'S' && comandoFromAtmega_c[1] == ','){
    //S,Roaster,Roaster2021*,5,
    memset(ssid_s, 0, 50);
    memset(pass_s, 0, 50);

    char *lista = strtok(comandoFromAtmega_c,",");

    lista = strtok(NULL,",");
    strcpy(ssid_s, String(lista).c_str()); 

    lista = strtok(NULL,",");
    strcpy(pass_s, String(lista).c_str());

    lista = strtok(NULL,",");
    vaOP.canalwifi = (uint8_t)(String(lista).toInt());

    crearSocket(true);
    memset(comandoFromAtmega_c, 0, 50);

  }else if(comandoFromAtmega_c[0] == 'W' && comandoFromAtmega_c[1] == ','){
    //W,PR1,Pr12345678,
    int i = 2;
    int k = 0;
    memset(ssid_w, 0, 50);
    memset(pass_w, 0, 50);
    while(true){
      if(comandoFromAtmega_c[i] == ','){
        i++;
        while(true){
          if(comandoFromAtmega_c[i] == ',')break;
          if(k > 48 || i > 48)break;
          pass_w[k] = comandoFromAtmega_c[i];
          k++;
          i++;
        }
        break;
      }
      ssid_w[i-2] = comandoFromAtmega_c[i];
      i++;
      if(i > 48)break;
    }
    redclaveOK = true;
    memset(comandoFromAtmega_c, 0, 50);

  }else if(comandoFromAtmega_c[0] == 'O' && comandoFromAtmega_c[1] == 'F' && comandoFromAtmega_c[2] == 'F' && comandoFromAtmega_c[3] == 'A' && comandoFromAtmega_c[4] == 'L' && comandoFromAtmega_c[5] == 'L'){
    webSocket.close();
    server.close();
    WiFi.mode(WIFI_OFF);
    flatWifiOFF = false;
    flatSocketOFF = false;
    memset(comandoFromAtmega_c, 0, 50);


  }else if(comandoFromAtmega_c[0] == 'C' && comandoFromAtmega_c[1] == 'O' && comandoFromAtmega_c[2] == 'N' && comandoFromAtmega_c[3] == 'F' && comandoFromAtmega_c[4] == 'I' && comandoFromAtmega_c[5] == 'G'){
    Serial.print("SID,");Serial.print(ssid);Serial.print('\0');
    delay(100);
    Serial.print("SPA,");Serial.print(pass);Serial.print('\0');
    memset(comandoFromAtmega_c, 0, 50);

  //Si llega IN,230,160,1500,2000,200,600,200 --> ET, BT, Q, T, S, ROR, delta
  }else if(comandoFromAtmega_c[0] == 'I' && comandoFromAtmega_c[1] == 'N'){
    char *listaValores = strtok(comandoFromAtmega_c,",");

    listaValores = strtok(NULL,",");
    vaOP.temp_ET = String(listaValores).toInt();  // Temperatura ET

    listaValores = strtok(NULL,",");
    vaOP.temp_BT = String(listaValores).toInt();  // Temperatura BT

    listaValores = strtok(NULL,",");
    vaOP.porcent_Quem = String(listaValores).toInt();  // porcentaje quemador

    listaValores = strtok(NULL,",");
    vaOP.porcent_Tamb = String(listaValores).toInt();  // porcentaje tambor

    listaValores = strtok(NULL,",");
    vaOP.porcent_Sopl = String(listaValores).toInt();  // porcentaje soplador

    listaValores = strtok(NULL,",");
    vaOP.RoR = String(listaValores).toInt();  // RoR

    listaValores = strtok(NULL,",");
    vaOP.deltaETBT = String(listaValores).toInt();  // delta ET BT

    memset(comandoFromAtmega_c, 0, 50);

  }else{
    memset(comandoFromAtmega_c, 0, 50);
  }


  if(redclaveOK == true){
    //Serial.print(ssid_w);Serial.print(":");Serial.println(pass_w);
    Serial.flush();
    Serial.print('\0');
    Serial.print('D');Serial.print('E');Serial.print('L');Serial.print('\0');
    redclaveOK = false;
    webSocket.close();
    server.close();
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);  //Evitar que Wifi del ESP se duerma
    WiFi.begin(ssid_w, pass_w);
    statusDelayConexion = true;
    tiempoConexion = millis();
  }
}

void crearSocket(bool type){
  bool success = false;
  WiFi.mode(WIFI_OFF);
  redclaveOK = 0;
  WiFi.mode(WIFI_AP);
  checkNetwork(); // Mantiene o cambia nombre de red NO REPETIDO 
  if(type) success = WiFi.softAP(ssid_s, pass_s, vaOP.canalwifi);  //devuelve bool, si ya esta conectado --> return
  else WiFi.softAP(ssid, pass);
  server.begin();
  webSocket.begin();
  if(success && server.isRunning() && webSocket.isRunning()){
      flatSocketOFF = false;
      webSocket.onEvent(onWebSocketEvent);
      IPAddress IP = WiFi.softAPIP();
      Serial.print('\0');
      Serial.print('C');Serial.print('S');Serial.print('O');Serial.print('\0');
      Serial.print('I');Serial.print('P');Serial.print('S');Serial.print(IP);Serial.print('\0');
      Serial.print("SID,");Serial.print(ssid_s);Serial.print(",");Serial.print(pass_s);Serial.print('\0');
      Serial.flush();
  }
}

void checkNetwork(){
  uint16_t numNetworks = WiFi.scanNetworks();

  for(int i=0; i<numNetworks; i++){
      String actualName = WiFi.SSID(i);
      String str(ssid_s);

      if(str == actualName){
          char arr[str.length() + 1];  //Crear array con longitud de nombre solicitado
          strcpy(arr, str.c_str());   //Copiar nombre solicitado en el array
          
          //Si ultimo numero es cero incrementar
          if(arr[strlen(arr)-1] == '0'){  
              ssid_s[strlen(arr)-1] = '1';

          //Si ultimo numero es uno incrementar
          }else if(arr[strlen(arr)-1] == '1'){ 
              ssid_s[strlen(arr)-1] = '2';

          //Si ultimo numero es dos incrementar
          }else if(arr[strlen(arr)-1] == '2'){ 
              ssid_s[strlen(arr)-1] = '3';

          //Si ultimo numero es tres incrementar
          }else if(arr[strlen(arr)-1] == '3'){ 
              ssid_s[strlen(arr)-1] = '4';

          //Agregar un cero si es nombre repetido sin numero al final
          }else{   
              ssid_s[strlen(arr)] = ' ';
              ssid_s[strlen(arr)+1] = '0';
          }
      }
  }
}

void onDelayConexion(){
  if (millis() - tiempoConexion > 5000){
    statusDelayConexion = false;
    if(WiFi.status() == WL_CONNECTED && !statusModbus){
      flatWifiOFF = false;
      onModbusOverWifi();
      Serial.print('C');Serial.print('W');Serial.print('I');Serial.print('\0');
      delay(300);
      IPAddress IPW = WiFi.localIP();
      Serial.print('I');Serial.print('P');Serial.print('W');Serial.print(IPW);Serial.print('\0');
    }else if(WiFi.status() == WL_CONNECTED && statusModbus){
      flatWifiOFF = false;
      Serial.print('C');Serial.print('W');Serial.print('I');Serial.print('\0');
      delay(300);
      IPAddress IPW = WiFi.localIP();
      Serial.print('I');Serial.print('P');Serial.print('W');Serial.print(IPW);Serial.print('\0');
    }else if(!flatWifiOFF){
      flatWifiOFF = true;
      Serial.print('O');Serial.print('F');Serial.print('W');Serial.print('\0');
    }
  }
}

void onModbusOverWifi(){
  mb.server();
  mb.addHreg(HREG_BT);
  mb.addHreg(HREG_ET);
  mb.addHreg(HREG_ROR);
  mb.addHreg(HREG_QUEM);
  mb.addHreg(HREG_TAMB);
  mb.addHreg(HREG_SOPL);
  statusModbus = true;
}

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch(type) {

    // Client has disconnected
    case WStype_DISCONNECTED:
      //Serial.printf("[%u] Disconnected!\n", num);
      break;

    // New client has connected
    case WStype_CONNECTED:
    {
      IPAddress ip = webSocket.remoteIP(num);
      //Serial.printf("[%u] Connection from ", num);
      //Serial.println(ip.toString());
      break;
    }
    // Echo text message back to client
    case WStype_TEXT:
    {
      char* datas = (char*)payload;
      //Serial.print("request: ");Serial.println(datas);

      // Deserialize the JSON document
      DeserializationError error = deserializeJson(doc, datas);
      if (error) {
        //Serial.print("ERRORJSON");Serial.print('\0');
      }

      long Messageid;
      if (doc.containsKey("id")) Messageid = doc["id"];
      //if (doc.containsKey("messageID")) Messageid = doc["messageID"];

      //uint8_t Roasterid = doc["roasterID"];
      String comando = doc["command"];

      if (doc.containsKey("id")) doc2["id"] = Messageid;
      //if (doc.containsKey("messageID")) doc2["messageID"] = Messageid;

      doc2["data"]["aire"] = vaOP.temp_ET;
      doc2["data"]["grano"] = vaOP.temp_BT;
      doc2["data"]["ror"] = vaOP.RoR;
      doc2["data"]["quemador"] = vaOP.porcent_Quem;
      doc2["data"]["soplador"] = vaOP.porcent_Sopl;
      doc2["data"]["tambor"] = vaOP.porcent_Tamb;
      doc2["data"]["delta"] = vaOP.deltaETBT;

    //   doc2["data"]["aire"] = random(0,1500);
    //   doc2["data"]["grano"] = random(0,1500);
    //   doc2["data"]["ror"] = random(0,20);
    //   doc2["data"]["quemador"] = random(0,100);
    //   doc2["data"]["soplador"] = random(0,100);
    //   doc2["data"]["tambor"] = random(0,100);
    //   doc2["data"]["delta"] = random(0,1);
      

      if (comando == "getData") {
        String output;
        serializeJson(doc2, output);
        //Serial.print(" -> response: ");Serial.println(output);
        //Serial.println();

        webSocket.sendTXT(num, output);
        //*****Serial.print('G');Serial.print('E');Serial.print('T');Serial.print('\0'); //Avisar de solicitud de datos
        //Serial.print("Data enviada: ");Serial.print(millis()-cuentaArtisan);Serial.print(" ");Serial.println(vaOP.temp_BT);
        cuentaArtisan = millis();
      }else if(comando == "setControlParams"){
        if(doc["params"].containsKey("aire")) vaCO.aire = (uint16_t)doc["params"]["aire"];
        //if(doc["params"].containsKey("soplador")) vaCO.aire = (uint16_t)doc["params"]["soplador"];
        if(doc["params"].containsKey("tambor")) vaCO.tambor = (uint16_t)doc["params"]["tambor"];
        if(doc["params"].containsKey("quemador")) vaCO.quemador = (uint16_t)doc["params"]["quemador"];
        Serial.print("PARAM,");Serial.print(vaCO.aire);Serial.print(',');Serial.print(vaCO.tambor);Serial.print(',');Serial.print(vaCO.quemador);Serial.print(',');Serial.print('\0');
      
      }else if(comando == "startRoasting"){
        Serial.print("ONCHARGE");Serial.print('\0');
        delay(5);
        Serial.print("SOCARG");Serial.print('\0');

      }else if(comando == "endRoasting"){
        Serial.print("SODROP");Serial.print('\0');

      }else if(comando == "ready"){
        Serial.print("SREADY");Serial.print('\0');

      }else if(comando == "noready"){
        Serial.print("SNOREA");Serial.print('\0');

      }else if(comando == "identify"){
        Serial.print("IDENTIFY");Serial.print('\0');

      }else if(comando == "noidentify"){
        Serial.print("NOIDENTIFY");Serial.print('\0');
      
      }else if(comando == "getinit"){
        Serial.print("GETINIT");Serial.print('\0');
      }else if(comando == "reset")
    {
        Serial.print("RESET");Serial.print('\0');

    }else if(comando == "fcstart")
    {
        Serial.print("FCSTART");Serial.print('\0');

    }else if(comando == "oncharge")
    {
        Serial.print("ONCHARGE");Serial.print('\0');
        delay(5);
        Serial.print("SOCARG");Serial.print('\0');
        
    }

        
      break;
    }
    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
}

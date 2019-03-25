
// Include the libraries we need
    #include <OneWire.h>
    #include <DallasTemperature.h>
    #include <ArduinoJson.h>
    #include "Adafruit_FONA.h"
    #include <MemoryFree.h>

// Data wire is plugged into port 2 on the Arduino
    #define ONE_WIRE_BUS_1 2
    #define ONE_WIRE_BUS_2 3
    #define VBATPIN A9

//Setup FONA
    #define FONA_RX  9
    #define FONA_TX  8
    #define FONA_RST 4
    #define FONA_RI  7
    
    // this is a large buffer for replies
    char replybuffer[255];
    
    // We default to using software serial. If you want to use hardware serial
    // (because softserial isnt supported) comment out the following three lines 
    // and uncomment the HardwareSerial line
    #include <SoftwareSerial.h>
    SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
    SoftwareSerial *fonaSerial = &fonaSS;
    
    // Hardware serial is also possible!
    //  HardwareSerial *fonaSerial = &Serial1;
    
    // Use this for FONA 800 and 808s
    Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
    // Use this one for FONA 3G
    //Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);
    
    uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);
    uint8_t type;
       
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
    OneWire oneWire_in(ONE_WIRE_BUS_1);
    OneWire oneWire_out(ONE_WIRE_BUS_2);

// Pass our oneWire reference to Dallas Temperature. 
    DallasTemperature sensor_inbox(&oneWire_in);
    DallasTemperature sensor_outbox(&oneWire_out);

//Box ID
char id[9] = "locker2";

//Set up reed switch 
int switchReed=A5;

//Debug
int pinState = 0;

//Pressure sensor
int pressSensor=A4;

//Fail counter 
int failCount = 0;

//States
float boxTemp;
float outTemp;
bool doorStatus;
bool  boxLoaded;
float batteryLevel;

//Intervals
int intervalTimer=180000;

//Events
boolean boxClosedEventExecuted = false;
boolean boxOpenEventExecuted = false;
boolean sendFail = false;

//Debug
int passFlag = 0;

//Lamps
int gprsPin = 13;   
int failPin = 12;  
int okayPin = 11; 



void setup(void){

  pinMode(switchReed, INPUT);
  pinMode(gprsPin, OUTPUT); 
  pinMode(failPin, OUTPUT); 
  pinMode(okayPin, OUTPUT); 
  pinMode(pressSensor, INPUT);
  pinMode(FONA_RST, OUTPUT); 
   
  // Start up the library
  sensor_inbox.begin();
  sensor_outbox.begin();

  //Initialise - Fancy lights
  digitalWrite(gprsPin, HIGH);
  delay(1000);
  digitalWrite(okayPin, HIGH);
  delay(1000);
  digitalWrite(failPin, HIGH);
  delay(1000);
  digitalWrite(gprsPin, LOW);
  digitalWrite(okayPin, LOW);
  digitalWrite(failPin, LOW);
  delay(500);
  digitalWrite(gprsPin, HIGH);
  digitalWrite(okayPin, HIGH);
  digitalWrite(failPin, HIGH);
  delay(500);
  digitalWrite(gprsPin, LOW);
  digitalWrite(okayPin, LOW);
  digitalWrite(failPin, LOW);
  
  //FONA
  //while (!Serial);

  Serial.begin(115200);
  Serial.println(F("Initializing....(May take 3 seconds)"));
  digitalWrite(okayPin, HIGH);
  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }

  
  // Print module IMEI number.
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }

  // Optionally configure a GPRS APN, username, and password.
  // You might need to do this to access your network's GPRS/data
  // network.  Contact your provider for the exact APN, username,
  // and password values.  Username and password are optional and
  // can be removed, but APN is required.
  fona.setGPRSNetworkSettings(F("TM"), F(""), F(""));

  // Optionally configure HTTP gets to follow redirects over SSL.
  // Default is not to follow SSL redirects, however if you uncomment
  // the following line then redirects over SSL will be followed.
  //fona.setHTTPSRedirect(true);
  //*****************************

  while(!fona.enableGPRS(true)) {
    fona.enableGPRS(false);
      Serial.println(F("Failed to turn on GPRS. Trying again..."));
      digitalWrite(failPin, HIGH);
      digitalWrite(gprsPin, LOW);
      delay(200);
       
    if(fona.enableGPRS(true)) {
      Serial.println(F("GPRS is on.")); 
      digitalWrite(failPin, LOW);
      digitalWrite(gprsPin, HIGH);
      break;
    }
  }

}

/*
 * Main function, get and show the temperature
 */
void loop(void)
{ 

  //DEBUG
  /*Serial.println("+++DEBUG+++");
  Serial.println(pressSensor);
  pinState = digitalRead(pressSensor);
  //pinState = analogRead(pressSensor);
  Serial.println(pinState);*/
    
    //This routine checks the voltage on the battery (if it exists) and returns that value.  Should be in the 3.2-4.2 range depending upon the battery used
    float measuredvbat = analogRead(VBATPIN);
    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage
     // Serial.print("VBat: " ); Serial.println(measuredvbat);
     batteryLevel = (measuredvbat);

    //Get and set the box temperature
    sensor_inbox.requestTemperatures();
    sensor_outbox.requestTemperatures();
    float tempCin = sensor_inbox.getTempCByIndex(0);
    float tempCout = sensor_outbox.getTempCByIndex(0);
    
      boxTemp = (tempCin);
      outTemp = (tempCout);
      //Serial.println(boxTemp);
      //Serial.println(outTemp);

    //Detect failed send
    if (sendFail == true){
      //Serial.println(F("sendFail = true"));
      failCount++;
      Serial.print("Fail count: ");
      Serial.println(failCount);
      if (failCount >= 5){
        failCount = 0;
        setup();
      }
      else{
      enableGPRS();
      delay(1000);
      }
      
    }

    //Detect Door Status
    //Closed
    if (digitalRead(switchReed)==1){
      doorStatus=false;
      if (boxClosedEventExecuted == false){
        boxClosedEvent();
      };
    }
    //Open
    else{
      doorStatus=true;
      if (boxOpenEventExecuted == false){
        boxOpenEvent();
      };
    };

    //Detect if box loaded
    //Loaded
    if (digitalRead(pressSensor)==1){
      boxLoaded=true;
      digitalWrite(okayPin, HIGH);
      delay(1000);
      digitalWrite(okayPin, LOW);
    }
    //Not loaded
    else{
      boxLoaded=false;
      //digitalWrite(okayPin, LOW);
    } 

    //Send update at interval
    static unsigned long lastTime = 0;
    const long interval = intervalTimer;
    unsigned long now = millis();
  
    if ( now - lastTime > interval) {
        lastTime = now;
          Serial.println("Timed Send");
          enableGPRS();
    }

}

//When box is opened
void boxOpenEvent(){ 
  Serial.println(F("BOX IS OPEN"));
  enableGPRS();
  boxOpenEventExecuted = true;
  boxClosedEventExecuted = false;    
  digitalWrite(failPin, HIGH);


 if(!fona.enableGPRS(true)) {
  enableGPRS();
 }
 else{
  sendCurrentStatus();
 }
}

//When box is closed
void boxClosedEvent(){ 
  Serial.println(F("BOX IS CLOSED"));
  //enableGPRS();
  boxClosedEventExecuted = true;   
  boxOpenEventExecuted = false; 

  if(!fona.enableGPRS(true)) {
  enableGPRS();
 }
 else{
  sendCurrentStatus();
 }
}

//Send update at timed intervals
/*void intervalUpdate(){ 
  static unsigned long lastTime = 0;
  const long interval = intervalTimer;
  unsigned long now = millis();

  if ( now - lastTime > interval) {
      lastTime = now;
        enableGPRS();
  }

}*/

void enableGPRS(){
    digitalWrite(failPin, LOW);

   // This is supposed to check for GPRS connection and reconnect if not available
   // However it seems to turn the GRPS off and then back on every time and is causing some data send errors
   // Commenting out for now and will skip straight to sending status
   
   /*while(!fona.enableGPRS(true)) {
    fona.enableGPRS(false);
      Serial.println(F("Failed to turn on GPRS. Trying again..."));
      digitalWrite(failPin, HIGH);
      digitalWrite(gprsPin, LOW);
      delay(200);
       
    if(fona.enableGPRS(true)) {
      Serial.println(F("GPRS is on.")); 
      digitalWrite(failPin, LOW);
      digitalWrite(gprsPin, HIGH);
      break;
    }
  }*/
  sendCurrentStatus();

}


//Send all current data
void sendCurrentStatus(){
    digitalWrite(gprsPin, HIGH);
    digitalWrite(okayPin, LOW);
  //Get network time and add 20** to year 
   /*if (!fona.enableNTPTimeSync(true, F("pool.ntp.org")))
   Serial.println(F("Failed to enable"));

  char timeStamp[23];
  String timeWithYear;
  fona.getTime(timeStamp, 23);  // make sure replybuffer is at least 23 bytes!
  //Serial.print(F("Time = ")); Serial.println(timeStamp);
  timeStamp[strlen(timeStamp) - 1] = 0;

  timeWithYear = "20";
  timeWithYear += &timeStamp[1];
  Serial.println(timeWithYear);*/

  
  //Output as JSON
  //DynamicJsonBuffer  jsonBuffer(200);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["thingId"] = id;
  //root["timestamp"] = timeWithYear;
  root["batteryStatus"] = batteryLevel;
  root["intemp"] = boxTemp;
  root["outtemp"] = outTemp;
  root["lidOpen"] = doorStatus;
  root["loaded"] = boxLoaded;

  root.prettyPrintTo(Serial);
  Serial.println("");
  Serial.print(F("freeMemory()="));
  Serial.println(freeMemory());

  char jsonChar[156];
  root.printTo((char*)jsonChar, root.measureLength() + 1);

  //See the string output of root string output 
  //Serial.print(F("jsonChar: "));
  //Serial.println(jsonChar);
  
  
  // Post data to website
  uint16_t statuscode;
  int16_t length;
  char url[49]="http://labsvoc.azurewebsites.net/api/VolvoLocker";
  //char url[49]="failtosendtovolvoapi.com";
  Serial.flush();
  Serial.println(F("****"));
  //Failed to send
  if (!fona.HTTP_POST_start(url, F("application/json"), (uint8_t *) jsonChar, strlen(jsonChar), &statuscode, (uint16_t *)&length)) {
    //jsonBuffer.clear();
    Serial.println(F("Failed!"));
    //Serial.print("Length: ");
    //Serial.println(length);
    signalStrength();
    Serial.print(F("freeMemory()="));
    Serial.println(freeMemory());
    sendFail = true;
    digitalWrite(failPin, HIGH);
    delay(500);
    digitalWrite(failPin, LOW);
    delay(200);
    digitalWrite(failPin, HIGH);
    delay(500);
    digitalWrite(failPin, LOW);
    delay(500);
    digitalWrite(failPin, HIGH);
    delay(500);
    digitalWrite(failPin, HIGH);
    digitalWrite(gprsPin, LOW);

  }
  else{
  
  while (length > 0) {
    while (fona.available()) {
    char c = fona.read();

  #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
              //Wait until data register empty. 
              loop_until_bit_is_set(UCSR0A, UDRE0);
              
              UDR0 = c;
  #else
              Serial.write(c);
  #endif

      length--;
      if (! length) break;
    }
  }
  Serial.println(F("\n****"));
  fona.HTTP_POST_end();
  sendFail = false;
  //Serial.print("Length: ");
  //Serial.println(length);
  signalStrength();
  Serial.print(F("freeMemory()="));
  Serial.println(freeMemory());
  digitalWrite(failPin, LOW);
  digitalWrite(okayPin, HIGH);
  delay(500);
  digitalWrite(okayPin, LOW);
  delay(200);
  digitalWrite(okayPin, HIGH);
  delay(500);
  digitalWrite(okayPin, LOW);
  failCount = 0;
  digitalWrite(gprsPin, LOW);
  digitalWrite(okayPin, HIGH);
  }
}

void signalStrength() {
        // read the RSSI
        uint8_t n = fona.getRSSI();
        int8_t r;

        Serial.print(F("Signal Strength = ")); Serial.print(n); Serial.print(": ");
        if (n == 0) r = -115;
        if (n == 1) r = -111;
        if (n == 31) r = -52;
        if ((n >= 2) && (n <= 30)) {
          r = map(n, 2, 30, -110, -54);
        }
        Serial.print(r); Serial.println(F(" dBm"));
}


//Error reporting
//01 = Internal temp sensor
void errorReporting(int error){
  Serial.print("There's a fuck up somewhere: ");
  Serial.print(error);
}

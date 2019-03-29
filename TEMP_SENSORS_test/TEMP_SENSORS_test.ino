// Include the libraries we need
    #include <OneWire.h>
    #include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
    #define ONE_WIRE_BUS_1 2
    #define ONE_WIRE_BUS_2 3
    #define VBATPIN A9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
    OneWire oneWire_in(ONE_WIRE_BUS_1);
    OneWire oneWire_out(ONE_WIRE_BUS_2);

// Pass our oneWire reference to Dallas Temperature. 
    DallasTemperature sensor_inbox(&oneWire_in);
    DallasTemperature sensor_outbox(&oneWire_out);

//States
float boxTemp;
float outTemp;


void setup() {
  // put your setup code here, to run once:
  // Start up the library
  sensor_inbox.begin();
  sensor_outbox.begin();

  
}

void loop() {
  // put your main code here, to run repeatedly:
  //Get and get the box temperature
    sensor_inbox.requestTemperatures();
    sensor_outbox.requestTemperatures();
    float tempCin = sensor_inbox.getTempCByIndex(0);
    float tempCout = sensor_outbox.getTempCByIndex(0);
    
      boxTemp = (tempCin);
      outTemp = (tempCout);
      Serial.print("Box: ");
      Serial.println(boxTemp);  
      Serial.print("Out: ");
      Serial.println(outTemp);
      delay(1000);
}

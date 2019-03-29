//Lamps
int gprsPin = 13;   
int failPin = 12;  
int okayPin = 11; 

//Set up reed switch 
int switchReed=A5;

//Pressure sensor
int pressSensor=A4;

void setup() {
  // put your setup code here, to run once:
 pinMode(gprsPin, OUTPUT); 
  pinMode(failPin, OUTPUT); 
  pinMode(okayPin, OUTPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:

  //Check lid
  if (digitalRead(switchReed)==1){
       digitalWrite(failPin, HIGH);
       digitalWrite(okayPin, LOW);
    }
    //Open
    else{
        digitalWrite(failPin, LOW);
        digitalWrite(okayPin, HIGH);
    };

    //Check pressure sensor
    if (digitalRead(pressSensor)==1){
      digitalWrite(gprsPin, HIGH);
    }
    //Not loaded
    else{
      digitalWrite(gprsPin, LOW);
    } 
    
  

}

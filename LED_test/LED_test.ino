//Lamps
int gprsPin = 13;   
int failPin = 12;  
int okayPin = 11; 

void setup() {
  // put your setup code here, to run once:
 pinMode(gprsPin, OUTPUT); 
  pinMode(failPin, OUTPUT); 
  pinMode(okayPin, OUTPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(gprsPin, HIGH);
  digitalWrite(failPin, HIGH);
  digitalWrite(okayPin, HIGH);
}

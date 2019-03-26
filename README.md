# Box Monitor Beta

##

### Construction
A general top-tip is make sure your workspace is clear that you have all the tools and parts you need. 
Tools you will need: 
- Small flat-head screw driver (2.4mm)
- Wire cutters
- Needle nose pliers
- Hot glue gun
- Soldering iron
- Solder
- Black nasty/electrical tape
- Multimeter


#### Step 1
Start by fitting the resistors to the board. Cut the legs short to ensure a tight fit into the board. You may want to use the pliers to help push the pins into the breadboard. 
The resistors should match the colours on the diagram. If they don't, here is a handy guide: https://learn.sparkfun.com/tutorials/resistors/decoding-resistor-markings

![Step 1](https://github.com/paulaston-volvo/box-beta/blob/master/box_monitor_schematics/Step%201.png?raw=true.png)

#### Step 2
Start wiring. Try to cut the wires so they fit as snugly to the board as possible. _Use the single core wire_ for this.
Holes H1-H16 down to B2-B6 will be covered by the Fona board and unusable, wires can be run tightly under the board to the left of that.

![Step 2](https://github.com/paulaston-volvo/box-beta/blob/master/box_monitor_schematics/Step%202.png?raw=true.png)

#### Step 3
Place the Fona board into the breadboard (_Note: The pins will have to be soldered to the Fona board before insertion_)
You can skip this step and do it later if you wish.
Also remember to add the SIM card before placing the Fona board.

![Step 3](https://github.com/paulaston-volvo/box-beta/blob/master/box_monitor_schematics/Step%203.png?raw=true.png)


#### Step 4
Add the connectors from the status light LEDs

![Step 4](https://github.com/paulaston-volvo/box-beta/blob/master/box_monitor_schematics/Step%204.png?raw=true.png)

#### Step 5
Wire in the sensor cables from the box, making sure to match the colours! Also remember to run them through the hole in the box first!

![Step 5](https://github.com/paulaston-volvo/box-beta/blob/master/box_monitor_schematics/Step%205.png?raw=true.png)

#### Step 6 
Plug in the batteries and the GSM antenna and youre done!

![Step 6](https://github.com/paulaston-volvo/box-beta/blob/master/box_monitor_schematics/Step%206.png?raw=true.png)

#### Notes:

 - The small battery may not have a full charge which will affect the ability to send data, but when the board is plugged into your laptop to load the programme, it should give it enough boost.
 -  The big battery will only have a 75% charge when you buy it. Top it up from a phone charger plugged into the wall (not a laptop, it takes ages.)
 
 # Software installation

##

### Arduino IDE
First install the Arduino IDE on your machine: http://www.arduino.cc/en/Main/Software

After you have downloaded and installed the latest version of Arduino IDE, you will need to start the IDE and navigate to the Preferences menu.

![Prefs](https://cdn-learn.adafruit.com/assets/assets/000/025/281/large1024/flora_Screen_Shot_2015-05-07_at_9.07.21_AM.png?1431004060)

We will be adding a URL to the new Additional Boards Manager URLs option. The list of URLs is comma separated, and you will only have to add each URL once. New Adafruit boards and updates to existing boards will automatically be picked up by the Board Manager each time it is opened. The URLs point to index files that the Board Manager uses to build the list of available & installed boards.

To find the most up to date list of URLs you can add, you can visit the list of third party board URLs on the Arduino IDE wiki. We will only need to add one URL to the IDE in this example, but you can add multiple URLS by separating them with commas. Copy and paste the link below into the Additional Boards Manager URLs option in the Arduino IDE preferences.

https://adafruit.github.io/arduino-board-index/package_adafruit_index.json

### Libraries

There all also a number of Libraries you'll need to install that run the sensors, JSON package and other things. These are all in https://github.com/paulaston-volvo/box-beta/tree/master/Libraries

A guide to installation is here: https://www.arduino.cc/en/guide/libraries

After you've installed everything, _close and restart the IDE_.

### Using with Arduino IDE 
Since the Feather 32u4 uses an ATmega32u4 chip running at 8 MHz, you can pretty easily get it working with the Arduino IDE. 

Now that you have added the appropriate URLs to the Arduino IDE preferences, you can open the Boards Manager by navigating to the Tools->Board menu.

![Board](https://cdn-learn.adafruit.com/assets/assets/000/050/496/large1024/adafruit_products_bsp.png?1516824755)

Once the Board Manager opens, click on the category drop down menu on the top left hand side of the window and select Contributed. You will then be able to select and install the boards supplied by the URLs added to the prefrences. In the example below, we are installing support for Adafruit AVR Boards, but the same applies to all boards installed with the Board Manager.

Next, quit and reopen the Arduino IDE to ensure that all of the boards are properly installed. You should now be able to select and upload to the new boards listed in the Tools->Board menu.

![Board](https://cdn-learn.adafruit.com/assets/assets/000/028/442/large1024/microcomputers_featherselect.gif?1447995225)

### Blink
Run a test program.

Plug in the Feather 32u4 and wait for it to be recognized by the OS (just takes a few seconds). It will create a serial/COM port, you can now select it from the dropdown, it'll even be 'indicated' as Feather 32u4!

![Board](https://cdn-learn.adafruit.com/assets/assets/000/028/443/large1024/microcomputers_featherport.png?1446677577)

Go to File > New and then copy in the code below. 

```
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
```

Click to Tick icon in the top left to validate the code, when that comes back okay, click the Arrow icon to send it to the board. You'll see the blink rate on the inbuilt LED in board change. Great success!
 

## Installing the Box Box software

Download and open the "box_monitor_beta.ino" in the Arduino IDE: https://github.com/paulaston-volvo/box-beta/blob/master/box_monitor_beta/box_monitor_beta.ino

Navigate down to
```
//Box ID
char id[13] = "locker881801"
```

And change the locker ID to match the box 

- locker169872
- locker439243
- locker734714
- locker948515

Go to Tools > Serial Monitor and select. Make sure you set it to Baudrate 115200 & Both NL & CR

![monitor](https://hackster.imgix.net/uploads/attachments/235973/Baud_rates_and_BothNLCR-1.png?auto=compress%2Cformat&w=680&h=510&fit=max)

Click the arrow icon to send it to the board. 

The software will flash to the board and it will run through the setup process. 
 - connect to the GSM network
 - Enable GPRS (data connection)
    - Both of these may take a while  
 - Get first set of data from sensors
 - Process as JSON
 - Send to server
 - Get response
 - And then
    - If error, try again
    - If succesfull, set green light and wait for box to open or close
    
You can see this on the serial monitor: 

```
Failed to turn on GPRS. Trying again...
	---> AT+CIPSHUT
	<--- SHUT OK
	---> AT+CGATT=1
	<--- ERROR
	---> AT+CIPSHUT
	<--- SHUT OK
	---> AT+CGATT=1
	<--- ERROR
	---> AT+CIPSHUT
	<--- SHUT OK
	---> AT+SAPBR=0,1
	<--- ERROR
```
It will do this quite a few times. It's normal and nothing to worry about

```
Failed to turn on GPRS. Trying again...
---> AT+CIPSHUT
	<--- SHUT OK
	---> AT+CGATT=1
	<--- ERROR
	---> AT+CIPSHUT
	<--- SHUT OK
	---> AT+CGATT=1
	<--- OK
	---> AT+SAPBR=3,1,"CONTYPE","GPRS"
	<--- OK
	---> AT+SAPBR=3,1,"APN","TM"
	<--- OK
	---> AT+CSTT="TM","",""
	<--- OK
	---> AT+SAPBR=3,1,"USER",""
	<--- OK
	---> AT+SAPBR=3,1,"PWD",""
	<--- OK
	---> AT+SAPBR=1,1
	<--- OK
	---> AT+CIICR
	<--- OK
BOX IS CLOSED
	---> AT+CIPSHUT
	<--- SHUT OK
	---> AT+CGATT=1
	<--- OK
	---> AT+SAPBR=3,1,"CONTYPE","GPRS"
	<--- OK
	---> AT+SAPBR=3,1,"APN","TM"
	<--- OK
	---> AT+CSTT="TM","",""
	<--- OK
	---> AT+SAPBR=3,1,"USER",""
	<--- OK
	---> AT+SAPBR=3,1,"PWD",""
	<--- OK
	---> AT+SAPBR=1,1
	<--- ERROR
```
Succesfully booted and connected

```
{
  "thingId": "locker2",
  "batteryStatus": 6.593555,
  "intemp": 85,
  "outtemp": 85,
  "lidOpen": false,
  "loaded": false
}
```
This is the data that it sends. You can see here if the sensors are working properly. (Note: intemp & outtemp 85, means "sensors okay. No reading taken yet. This is okay)
- thingId: Locker ID
- batteryStatus: battery voltage
- intemp: temperature in the box
- outtemp: temperature outside the box
- lidopen: false = closed, true = open
- loaded: false = nothing in there, true = loaded

```
	---> AT+HTTPTERM
	<--- ERROR
	---> AT+HTTPINIT
	<--- OK
	---> AT+HTTPPARA="CID"
	<--- OK
	---> AT+HTTPPARA="UA"
	<--- OK
	---> AT+HTTPPARA="URL"
	<--- OK
	---> AT+HTTPPARA="CONTENT"
	<--- OK
	---> AT+HTTPDATA=102,10000
	<--- DOWNLOAD
	<--- OK
	---> AT+HTTPACTION=1
	<--- OK
Status: 200
```
Data succesfully sent. If it can't send, it will try again until it does. 

## Operating sequence

When the box boots, it goes through the sequence above until it can successfully send a status update to the server. If it fails to send 5 times it will reboot itself and try to reconnect. If it is stuck in the bootup sequence, press the reset button on the Adafruit board. 

![reset](https://cdn-learn.adafruit.com/assets/assets/000/030/531/large1024/adafruit_products_3027_top_ORIG.jpg?1455136926)

It's the tiny push button at the bottom left above the 3V pin. 

Once the box is set it listens for the lid to open or close. On Open it will collect the sensor data, parse it as JSON and send it to the server. When the lid closes it will do the same, if the box is loaded it will send that information. 

It will also send an update to the server every 9 minutes. 

Again, if it fails to connect at any point in this more than 5 times, it will reboot and try again. Sometimes it may take multiple reboots (a few minutes), but 95% of the time this will sort the problem. 


## The lights 

![lights](https://github.com/paulaston-volvo/box-beta/blob/master/lights.png?raw=true)

## Troubleshooting

**The box doesn't seem to work after I unplug my laptop**
Sometime unplugging the serial monitor can screw things up. Hit the reset button

**The box seems to keep cycling and never sends**
- Check that the GSM antenna is firmly attached
- It could be that the small battery is depleted - it needs this for the power spike when sending _even with the big battery plugged in_. See below.

**There is a permant red light**
 - The small battery could be depleted. 
 


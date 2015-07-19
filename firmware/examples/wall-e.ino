//----- DEFINITIONS
#define NECK A7
#define HEAD A6
#define LEFT_ARM A5
#define RIGHT_ARM A4
#define RIGHT_TRACK A0
#define LEFT_TRACK A1
#define EYES A2

#define SERVO_THRESHOLD 10
#define BLINK_THRESHOLD 5 // SECONDS

//----- INCLUDES
#include "OSCData.h"
#include "OSCMatch.h"
#include "OSCMessage.h"

//----- INIT
SYSTEM_MODE(SEMI_AUTOMATIC);

//----- SPARK CONNECTION
bool CONNECTED = false;
bool MODE_PRESSED = false;

//----- OSC COMMANDS
char OscCmd_larm[6] = "/larm";
char OscCmd_rarm[6] = "/rarm";
char OscCmd_ltrack[8] = "/ltrack";
char OscCmd_rtrack[8] = "/rtrack";
char OscCmd_head[6] = "/head";

char OscCmd_btn1[8] = "/sb/2/1";
char OscCmd_btn2[8] = "/sb/2/2";
char OscCmd_btn3[8] = "/sb/2/3";
char OscCmd_btn4[8] = "/sb/2/4";

char OscCmd_btn5[8] = "/sb/1/1";
char OscCmd_btn6[8] = "/sb/1/2";
char OscCmd_btn7[8] = "/sb/1/3";
char OscCmd_btn8[8] = "/sb/1/4";

//----- UDP
unsigned int localPort = 8000;

UDP Udp;

//----- SERVOS
Servo neck;
Servo head;
Servo rightArm;
Servo leftArm;
Servo rightTrack;
Servo leftTrack;

//----- EYSE
unsigned long blinkMillis;

//----- METHODS
void handleHead(OSCMessage &msg)
{
    // 0 = y (float)
    // 1 = x (float)
    if(msg.isFloat(0) && msg.isFloat(1))
    {
        mapServo(head, HEAD, msg.getFloat(0), -100, 100, 70, 160);
        mapServo(neck, NECK, msg.getFloat(1), -100, 100, 5, 100);
    }
}

void handleLeftArm(OSCMessage &msg)
{
    // 0 = val (float) 
    if(msg.isFloat(0))
    {
        mapServo(leftArm, LEFT_ARM, msg.getFloat(0), 0, 100, 10, 140);
    }
}

void handleRightArm(OSCMessage &msg)
{
    // 0 = val (float) 
    if(msg.isFloat(0))
    {
        mapServo(rightArm, RIGHT_ARM, msg.getFloat(0), 0, 100, 125, 15);
    }
}

void handleLeftTrack(OSCMessage &msg)
{
    // 0 = val (float) 
    if(msg.isFloat(0))
    {
        mapServo(leftTrack, LEFT_TRACK, msg.getFloat(0), -100, 100, 180, 0);
    }
}

void handleRightTrack(OSCMessage &msg)
{
    // 0 = val (float) 
    if(msg.isFloat(0))
    {
        mapServo(rightTrack, RIGHT_TRACK, msg.getFloat(0), -100, 100, 0, 180);
    }
}

void mapServo(Servo &servo, int servoPin, float value, long fromMin, long fromMax, long toMin, long toMax)
{
    if(abs(value) >= SERVO_THRESHOLD)
    {
        if(!servo.attached()) {
            servo.attach(servoPin);   
        }
        
        int tv = map(value, fromMin, fromMax, toMin, toMax);
        
        Serial.println(tv);
        
        servo.write(tv);
    }
    else
    {
        if(servo.attached())
        {
            servo.detach();
        }
    }
}

void handleButton1(OSCMessage &msg)
{
    if(msg.isFloat(0) && msg.getFloat(0) == 1.00)
    {
        Serial.println("Sound 1");
        triggerSound(D0);
    }
}

void handleButton2(OSCMessage &msg)
{
    if(msg.isFloat(0) && msg.getFloat(0) == 1.00)
    {
        Serial.println("Sound 2");
        triggerSound(D1);
    }
}

void handleButton3(OSCMessage &msg)
{
    if(msg.isFloat(0) && msg.getFloat(0) == 1.00)
    {
        Serial.println("Sound 3");
        triggerSound(D2);
    }
}

void handleButton4(OSCMessage &msg)
{
    if(msg.isFloat(0) && msg.getFloat(0) == 1.00)
    {
        Serial.println("Sound 4");
        triggerSound(D3);
    }
}

void handleButton5(OSCMessage &msg)
{
    if(msg.isFloat(0) && msg.getFloat(0) == 1.00)
    {
        Serial.println("Sound 5");
        triggerSound(D4);
    }
}

void handleButton6(OSCMessage &msg)
{
    if(msg.isFloat(0) && msg.getFloat(0) == 1.00)
    {
        Serial.println("Sound 6");
        triggerSound(D5);
    }
}

void handleButton7(OSCMessage &msg)
{
    if(msg.isFloat(0) && msg.getFloat(0) == 1.00)
    {
        Serial.println("Sound 7");
        triggerSound(D6);
    }
}

void handleButton8(OSCMessage &msg)
{
    if(msg.isFloat(0) && msg.getFloat(0) == 1.00)
    {
        Serial.println("Release servos");
        
        head.detach();
        neck.detach();
        leftArm.detach();
        rightArm.detach();
        leftTrack.detach();
        rightTrack.detach();
    }
}

void triggerSound(int pin)
{
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
    delay(175);
    pinMode(pin, INPUT);
}

void blink()
{
    digitalWrite(EYES, LOW);
    delay(150);
    digitalWrite(EYES, HIGH);
}

bool modeBtnPressed() 
{
    if(BUTTON_GetDebouncedTime(BUTTON1) > 100) 
    {
        // Detect mode pressed only once
        if(!MODE_PRESSED) 
        { 
            MODE_PRESSED = true;
            return 1;
        }
        
        // wait until button is released before indicating it's pressed again
        return 0;
    }
    else 
    {
        MODE_PRESSED = false;
        return 0;
    }
}

//----- MAIN
void setup()                                                                                                                                                                                          
{
    // Declare handler functions (Compiler seems to need this)
    void handleHead(OSCMessage &msg);
    void handleLeftArm(OSCMessage &msg);
    void handleRightArm(OSCMessage &msg);
    void handleLeftTrack(OSCMessage &msg);
    void handleRightTrack(OSCMessage &msg);
    void handleButton1(OSCMessage &msg);
    void handleButton2(OSCMessage &msg);
    void handleButton3(OSCMessage &msg);
    void handleButton4(OSCMessage &msg);
    void handleButton5(OSCMessage &msg);
    void handleButton6(OSCMessage &msg);
    void handleButton7(OSCMessage &msg);
    void handleButton8(OSCMessage &msg);
    
    // Open serial over USB
    Serial.begin(9600);   
    
    // Set sound pins
    pinMode(D0, INPUT);
    pinMode(D1, INPUT);
    pinMode(D2, INPUT);
    pinMode(D3, INPUT);
    pinMode(D4, INPUT);
    pinMode(D5, INPUT);
    pinMode(D6, INPUT);
    
    // Turn on eyes
    pinMode(EYES, OUTPUT);
    digitalWrite(EYES, HIGH);
    blinkMillis = millis();
    
    // Start WiFi
    WiFi.on();
    WiFi.connect();
    while (!WiFi.ready()) SPARK_WLAN_Loop();
    
    // Start the UDP
    Udp.begin(localPort);
}

void loop() 
{
    if (modeBtnPressed()) 
    {
        if(CONNECTED == false) 
        {
            // Stop the udp server
            Udp.stop();
            
            // Ready the Hyperdrive!
            RGB.control(true);
            for(uint8_t x=0; x<2; x++) 
            { 
                RGB.color(255,0,0); //red
                delay(50);
                RGB.color(255,100,0); //orange
                delay(50);
                RGB.color(255,255,0); //yellow
                delay(50);
                RGB.color(0,255,0); //green
                delay(50);
                RGB.color(0,255,255); //cyan
                delay(50);
                RGB.color(0,0,255); //blue
                delay(50);
                RGB.color(255,0,255); //magenta
                delay(50);
                RGB.color(255,255,255); //white
                delay(50);
            }
            RGB.control(false);
            
            // Engage Hyperdrive!
            Spark.connect(); 
            
            // Update flag
            CONNECTED = true;
        }
        else
        {
            // Power down the Hyperdrive...
            Spark.disconnect(); 
            
            // Start the UDP up again
            Udp.begin(localPort);
            
            // Update flag
            CONNECTED = false;
        }
    }
    else
    {
        if(CONNECTED == false) 
        {
            // Receiving and dispatching an OSC Message
        	OSCMessage msgReceived;
        
        	int bytesToRead = Udp.parsePacket(); // how many bytes are available via UDP
        	if (bytesToRead > 0) 
        	{
        		while(bytesToRead--) 
        		{
        			msgReceived.fill(Udp.read()); // filling the OSCMessage with the incoming data
        		}
        
        		if(!msgReceived.hasError()) // if the address corresponds to a command, we dispatch it to the corresponding function
        		{ 
        			msgReceived.dispatch(OscCmd_larm , handleLeftArm);
        			msgReceived.dispatch(OscCmd_rarm , handleRightArm);
        			msgReceived.dispatch(OscCmd_ltrack , handleLeftTrack);
        			msgReceived.dispatch(OscCmd_rtrack , handleRightTrack);
        			msgReceived.dispatch(OscCmd_head , handleHead);
        			msgReceived.dispatch(OscCmd_btn1 , handleButton1);
        			msgReceived.dispatch(OscCmd_btn2 , handleButton2);
        			msgReceived.dispatch(OscCmd_btn3 , handleButton3);
        			msgReceived.dispatch(OscCmd_btn4 , handleButton4);
        			msgReceived.dispatch(OscCmd_btn5 , handleButton5);
        			msgReceived.dispatch(OscCmd_btn6 , handleButton6);
        			msgReceived.dispatch(OscCmd_btn7 , handleButton7);
        			msgReceived.dispatch(OscCmd_btn8 , handleButton8);
        		}  
        		else 
        		{
        		    Serial.println("OSC Error!");   
        		}
        	}
        }
    } 
    
    // Do blink
    if((millis() - blinkMillis) > (BLINK_THRESHOLD * 1000))
    {
        blinkMillis = millis();
        blink();
    }
}

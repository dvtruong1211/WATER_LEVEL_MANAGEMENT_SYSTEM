#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SimpleKalmanFilter.h>
#include<TimerOne.h>


LiquidCrystal_I2C lcd(0x27,16,2);
SimpleKalmanFilter deb(3,3,0.09);

int inputKeypad();

const int trig = 10;
const int echo = 11;
const int led = 2;
const int relay = 12;
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = 
{
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int ledState = 0;

String inputString;
long inputInt;
unsigned long Time = 0;
float high = 0 , prehigh =0.5;
int menu=0;

int maxlimit = 8, minlimit = 5; 
float htank = 20.3;

void setup(){
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  lcd.init(); 
  lcd.backlight(); 
  inputString.reserve (4);
  Timer1.initialize(150000);
  Timer1.attachInterrupt(Led);noInterrupts();
  
}
void loop()
{
	interrupts();
	char key = keypad.getKey();
	if(key=='#')
	{
		menu++;digitalWrite(led,1);
	}
	if(key=='0')
		menu = 0;
	if(key=='*')
		menu = menu*10;
	switch(menu){
		case 0: 
			 digitalWrite(trig,0);
			 delayMicroseconds(2); 
			 digitalWrite(trig,1); 
			 delayMicroseconds(15);
			 digitalWrite(trig,0);
			 Time = pulseIn (echo, 1);
			high = htank - (0.0342*(Time / 2 ));
			high = deb.updateEstimate(high);
		if(maxlimit>minlimit)
		{
			Relay_(); delay(30);
			lcd.setCursor(0,0);
			lcd.print("C.Cao");
			lcd.print(" Tren");
			lcd.print(" Duoi");
			lcd.setCursor(13,1);
			lcd.print(" cm");
			lcd.setCursor(0,1);
			lcd.print(high);
			if(maxlimit<10)
			{
				lcd.setCursor(8,1); 
				lcd.print(" "); 
			}
			lcd.setCursor(6,1);
			lcd.print(" ");
			lcd.print(maxlimit);
			lcd.setCursor(10,1);
			lcd.print("  ");
			lcd.print(minlimit);
			if(high<100)
			{
				lcd.setCursor(5,1); lcd.print(" "); 
			}  
			if(high<10)
			{
				lcd.setCursor(4,1); lcd.print(" "); 
			}
		}
		else 
		{ 
			delay(100);
			lcd.clear();
			lcd.setCursor(3,0); 
			lcd.print("XAY RA LOI");
			lcd.setCursor(0,1); 
			lcd.print("HAY CAI DAT LAI!");
			digitalWrite(led,ledState);
			digitalWrite(relay,0);
		}
		break;
		case 1:
			lcd.setCursor(0,0); 
			lcd.print(">Gioi Han Tren  ");
			lcd.setCursor(0,1); 
			lcd.print(" Gioi Han Duoi  ");
			break;
		case 2:
			lcd.setCursor(0,0); 
			lcd.print(" Gioi Han Tren  ");
			lcd.setCursor(0,1); 
			lcd.print(">Gioi Han Duoi  ");
			break;
		case 3:
			lcd.setCursor(0,0); 
			lcd.print(">Chieu Cao Be   ");
			lcd.setCursor(0,1); 
			lcd.print("                ");
			break;
		case 4: 
			menu=1; 
			break;
		
		case 10:
			lcd.setCursor(0,1); 
			lcd.print(inputString);
			lcd.print("               ");
			lcd.setCursor(0,0); 
			lcd.print("  Cai Dat GT:   ");
			inputKeypad(); 
			maxlimit=inputInt; 
			break;
		case 20:
			lcd.setCursor(0,1); 
			lcd.print(inputString);
			lcd.print("               ");
			lcd.setCursor(0,0); 
			lcd.print("  Cai Dat GT:   ");
			inputKeypad(); 
			minlimit=inputInt; 
			break;
		case 30:
			lcd.setCursor(0,1); 
			lcd.print(inputString);
			lcd.print("               ");
			lcd.setCursor(0,0); 
			lcd.print("  Cai Dat GT:   ");
			inputKeypad(); 
			htank=inputInt; break;
	} 
	prehigh=high;
}
void Led(void)
{
	ledState = ledState == 0 ? 1:0;
}
void Relay_()
{
if(high<minlimit)
{ 
	digitalWrite(relay,1);}
	if(digitalRead(relay)==1) 
		digitalWrite(led,ledState); 
	else 
		digitalWrite(led,1);
	if(maxlimit<high)
	{ 
		digitalWrite(relay,0);
		digitalWrite(led,1);
	}
	if(digitalRead(relay==1)&&prehigh>=high) 
		digitalWrite(relay,0);
}
int inputKeypad()
{
	char key = keypad.getKey();
    if (key) 
	{
		if (key >= '0' && key <= '9') 
		{
			inputString += key;
		} 
        else if (key == '*') 
		{
			if (inputString.length() > 0) 
			{
				inputInt = inputString.toInt();
				inputString = "";
				if(menu==10)
					menu=1; 
				if(menu==20)
					menu=2; 
				if(menu==30)
					menu=3;
			}
		} 
        else if (key == '#') 
		{
			inputString = "";
		}
		
}
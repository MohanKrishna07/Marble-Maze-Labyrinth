#include <Servo.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);
Servo myServoX;          // define servo motor for X-axis 
Servo myServoY;          // define servo motor for Y-axis 
int ServoXPin = 8;       // define  X-axis pin
int ServoYPin = 9;       // define  Y-axis pin
int ServoXHomePos =90;   // set home position for servos
int ServoYHomePos =90; 
int ServoXPos =103;
int ServoYPos =135; 
int XAxlePin = A0;       // define  X-axis pin control for joystick A0
int YAxlePin = A1;       // define  Y-axis pin control for joystick A1
int XAxleValue = 0;      // set start up value for joystick
int YAxleValue = 0;
int Direction = 0;
int range = 12;          // output range of X or Y movement
int center = range/2;    // resting position value
int threshold = range/4; // resting threshold

const int endpiezo=A2,tappiezo=A3;                         //"endpiezo" is for the piezo sensor kept at the end point and "tappiezo" is for the sensor which is used as a start button
int tap=0,a=0,force=0;
int thres1=100,thres2=550;                       //threshold values for the piezo sensors.
int score=0,highscore=0,b=0;                    //variables for storing score and highscore.
unsigned long  int startTime=0,end_time=0;             //variables for storing time at which the game started and ended.
const int buzzer = 10,led1=11,led2=12;                  //defining pins for buzzer and LED's
int ledstate=HIGH;
const float songSpeed =1.5;                 // Change to 0.5 for a slower version of the song, 2 for a faster version

                                                                   
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494                                             // Defining frequency of each music note
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_B5 988

int notes[] = {
    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,                          //Notes must be kept in a certain order for the required music. 
    NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
    NOTE_A4, NOTE_G4, NOTE_A4, 0,

    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
    NOTE_A4, NOTE_G4, NOTE_A4, 0,

    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
    NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0,
    NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
    NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,

    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_D5, NOTE_E5, NOTE_A4, 0,
    NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
    NOTE_C5, NOTE_A4, NOTE_B4, 0,

    NOTE_A4, NOTE_A4,
    
    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
    NOTE_A4, NOTE_G4, NOTE_A4, 0,

    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
    NOTE_A4, NOTE_G4, NOTE_A4, 0,

    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
    NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0,
    NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
    NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,

    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_D5, NOTE_E5, NOTE_A4, 0,
    NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
    NOTE_C5, NOTE_A4, NOTE_B4, 0,
    

    NOTE_E5, 0, 0, NOTE_F5, 0, 0,
    NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
    NOTE_D5, 0, 0, NOTE_C5, 0, 0,
    NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4,

    NOTE_E5, 0, 0, NOTE_F5, 0, 0,
    NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
    NOTE_D5, 0, 0, NOTE_C5, 0, 0,
    NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4
    };

                                                                   
int durations[] = {
    125, 125, 250, 125, 125,                                                          // Durations (in ms) of each music note of the song
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 125,

    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 125,

    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 125, 250, 125,

    125, 125, 250, 125, 125,
    250, 125, 250, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 375,

    250, 125,
   
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 125,

    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 125,

    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 125, 250, 125,

    125, 125, 250, 125, 125,
    250, 125, 250, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 375,
   

    250, 125, 375, 250, 125, 375,
    125, 125, 125, 125, 125, 125, 125, 125, 375,
    250, 125, 375, 250, 125, 375,
    125, 125, 125, 125, 125, 500,

    250, 125, 375, 250, 125, 375,
    125, 125, 125, 125, 125, 125, 125, 125, 375,
    250, 125, 375, 250, 125, 375,
    125, 125, 125, 125, 125, 500
    };
 const int totalNotes = sizeof(notes) / sizeof(int);
 int i=0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int readAxis(int thisAxis) {                                        //function used for taking input from joystick and returning the 'distance' which was used further.
// read the analog input: 
int reading = analogRead(thisAxis);
// map the reading from the analog input range to the output range:
reading = map(reading, 0, 1023, 0, range);
// if the output reading is outside from the
// rest position threshold, use it:
int distance = reading - center;
if (abs(distance) < threshold) {
distance = 0;
}
// return the distance for this axis:
return distance;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
myServoX.attach(ServoXPin);                                           // attaching servo X 
myServoY.attach(ServoYPin);                                           // attaching servo Y
ServoXPos = ServoXHomePos;                                            // update ServoXPos with home position as startup
ServoYPos = ServoYHomePos;                                            // update ServoYPos with home position as startup
myServoX.write(ServoXPos);
myServoY.write(ServoYPos);
lcd.init();                                                           //initailising the LCD
lcd.backlight();                                                      
Serial.begin(9600);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
    XAxleValue = readAxis(XAxlePin);                   //These two lines are for taking the input from the joystick
    YAxleValue = readAxis(YAxlePin);                   
    int j;
    if(a==1) j=3;                           //These two if statments are for controlling the speed of servos.We need more speed while playing because the music is creating a delay. 
    if(a==0 || a==2) j=1;                   //We have used a variable 'a' which will be equal to '1' while playing and '2' while the play is stopped
    
  if (XAxleValue>0) { ServoXPos+=j; myServoX.write(ServoXPos); delay(0.001*(7-XAxleValue)); }
  if (XAxleValue<0) { ServoXPos-=j; myServoX.write(ServoXPos); delay(0.001*(7+XAxleValue)); }
  if (YAxleValue>0) { ServoYPos+=j; myServoY.write(ServoYPos); delay(0.001*(7-YAxleValue)); }                       
  if (YAxleValue<0) { ServoYPos-=j; myServoY.write(ServoYPos); delay(0.001*(7+YAxleValue)); }
  
  if (ServoXPos>ServoXHomePos+40) { ServoXPos=ServoXHomePos+40; }
  if (ServoXPos<ServoXHomePos-40) { ServoXPos= ServoXHomePos-40; }
  if (ServoYPos>ServoYHomePos+40) { ServoYPos=ServoYHomePos+40; }
  if (ServoYPos<ServoYHomePos-40) { ServoYPos= ServoYHomePos-40; }
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  tap=analogRead(tappiezo);         //taking input from the sensor which is used as a start button
  
            lcd.setCursor(0,0);
            lcd.print("Tap sensor to ");
            lcd.setCursor(0,1);                 //Displaying the msg "Tap sensor to start the game" in the LCD untill he taps
            lcd.print("start the game");     
                   
   if(tap>thres1 || a==1)                 //When the tap reading is greater than threshold, then the timer will begin.
   { 
      a=1;                               //As told above, a will be equal to '1' during the game.
      force=analogRead(endpiezo);        //taking input from the sensor which kept at the end point
      
      if(force<thres2 && a==1)               
      {
        if(b==0) {
                  lcd.clear(); lcd.setCursor(0,0); lcd.print("Max Time:100sec");
                  delay(2000); lcd.setCursor(0,1); lcd.print("Start.."); delay(1000); lcd.print("3..");    //Displaying the max time given and "Start..3..2..1"
                  delay(1000); lcd.print("2.."); delay(1000); lcd.print("1");
                  startTime=millis();                                                                      //storing the time at which the game has started
                  lcd.clear();
                 }
        b=1;                                           //changing the value of 'b' ,so that "startTime" will not be updated every time the loop iterates.
        
        lcd.setCursor(0,0); lcd.print("Time:");
        lcd.setCursor(0,1);
        lcd.print((millis()-startTime)/1000);          //Displaying the time taken from the start of the game   
        lcd.setCursor(3,1);
        lcd.print("seconds");
       ////////////////////////////////////////////////
        const int currentNote = notes[i];                               //this snippet is for playing music
        float wait = durations[i] / songSpeed;
         if(currentNote != 0)
             {
                  tone(buzzer, notes[i], wait);                                                     // tone(pin, frequency, duration) 
             }
         else  noTone(buzzer); 
    
       delay(wait);
       i++;
       if(i==totalNotes) i=0;                                          //when all the notes were played,then 'i' will be updated to 0. So,that the music will again start from beginning.
      //////////////////////////////////////////////////
       if(i%8==0){ 
                 ledstate = !ledstate;                                     //changing the state of led every 1sec to 2secs for blinking effect
                 digitalWrite(led1,ledstate);
                 digitalWrite(led2,ledstate);
                }
                
      } 
           
      if(force>thres2)
      {
        digitalWrite(led1,HIGH);
        digitalWrite(led2,HIGH);
        a=2; i=0; b=0;           
        lcd.clear(); lcd.setCursor(0,0); lcd.print("Time taken:");
        lcd.setCursor(0,1);
        
        end_time=millis();                           //storing the time at which game was ended                                              
        int timetaken=(end_time-startTime)/1000;     //Diff between start and end times will be the total time taken
        
        lcd.print(timetaken); lcd.setCursor(3,1); lcd.print("seconds"); delay(2000); //Displays the timetaken for 2 seconds
        
        score=5000-50*timetaken;                                 //relating the timetaken with score
        
        if(score>0)                                                                //if time taken is less than 100sec,then score will be >0 and prints the score accordingly
        {
                 lcd.clear(); lcd.setCursor(0,0); lcd.print("Score:");
                 lcd.setCursor(6,0); lcd.print(score); delay(3000); 
        }
        
        if(score<=0)                                                               //if time taken is more than 100 sec,then the game is lost and prints accordingly
        {
          lcd.clear(); lcd.setCursor(0,0); lcd.print("Lost the game!!"); delay(2000);
          lcd.setCursor(0,0); lcd.print("Better luck next");  lcd.setCursor(0,1); lcd.print("time"); delay(3000); 
        }
        
        if(score>highscore && highscore!=0)                                       //if the current score is greater than the highscore,then
        {
         highscore=score;                                                         //highscore will be updated
         lcd.clear();  lcd.setCursor(0,0); lcd.print("Yayy!High score");          // and prints some msgs "Yayy!High score beaten"
         lcd.setCursor(0,1); lcd.print("beaten"); delay(2000);
        }
        
        if(highscore==0 && score>0)                                                //This if statment is only for the first game.As there will be no highscore before that,
        {
          lcd.clear(); lcd.setCursor(0,0); lcd.print("High Score:");
          lcd.setCursor(11,1); lcd.print(score);
          highscore=score;                                                         //the score we get in the first game will be the highscore for now.
          delay(2000);
        }
        
        if(highscore!=0 && score>0)                                             //This is for all other games except the first one
        {
          lcd.clear(); lcd.setCursor(0,0); lcd.print("High Score:");                      //Displaying the highscore
          lcd.setCursor(12,0); lcd.print(highscore); delay(2000);
        }
          
      }
   }   
}

// librarie versiune 3.0.0.
// curioasa chestie, sa mearga in fata: left ii dai BACKWARD, right ii dai FORWARD
//!!!! PROBLEMA ERA CA SE SCHIMBA PATH UL LA FIECARE ACCES CRED, ASTA ERA SIGUR UN BUG THO YUPPI

#include <QTRSensors.h>
#include <AFMotor.h>
#include <string.h> 

AF_DCMotor motorL(4); //stanga
AF_DCMotor motorR(1); //dreapta

#define Kp 0.15  // 
#define Kd 1 // 
// Note: Kp < Kd  

#define MAX_SPEED 100 // max speed of the robot
#define BASE_SPEED 65 // this is the speed at which the motors should spin when the robot is perfectly on the line 65
#define NUM_SENSORS 6     // number of sensors used
#define TIMEOUT 2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN     // emitter is controlled by digital pin 2

#define leapTime 100
#define replayTime 7000

// sensors 1 through 6 are connected to analog pins 0 through 5, respectively
QTRSensorsRC qtrrc((unsigned char[]) {19, 18, 17, 16, 15, 14},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 

unsigned int sensorValues[NUM_SENSORS];
unsigned int position = 0;
unsigned long timp0;
unsigned long timp2;
int lastError = 0;
char path[120] = {};
int pathLenght = 0;
int readLenght = 0;


void setup()
{
  pinMode(13, OUTPUT);
  delay(500);    // turn on Arduino's LED to indicate we are in calibration mode
  digitalWrite(13, HIGH);
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  } 
  digitalWrite(13, LOW);
  delay(1500);   
  Serial.begin(9600);
}


void loop()
{
  
     position = qtrrc.readLine(sensorValues);
     
     while(sensorValues[0] < 150 && sensorValues[5] < 150 && (sensorValues[2] > 100 || sensorValues[3] > 100))
     {
        PID();
     }

     if(sensorValues[0] > 150)
     {
         /*motorL.setSpeed(BASE_SPEED);
         motorR.setSpeed(BASE_SPEED);
         motorL.run(FORWARD);
         motorR.run(FORWARD);
         delay(120);
         position = qtrrc.readLine(sensorValues);
     
         if(sensorValues[1] > 300 && sensorValues[2] > 300 && sensorValues[3] > 300 && sensorValues[4] > 300)
          {
            path[pathLenght] = 'G';
            gata();
          }
          */
         timp0 = millis();
         timp2 = timp0;

         while((sensorValues[0] > 100 || sensorValues[5] > 100) && (timp2 - timp0 < 230))
         {
              timp2 = millis();
              position = qtrrc.readLine(sensorValues);
              motorL.setSpeed(BASE_SPEED);
              motorR.setSpeed(BASE_SPEED);
              motorL.run(FORWARD);
              motorR.run(FORWARD);
         }
         
         if(timp2 - timp0 >= 230)
         {
            path[pathLenght] = 'G';
            gata();
         }
         
         else 
         { 
            turnLeft();
            path[pathLenght] = 'L';
            pathLenght ++;
         }
     }
     else if(sensorValues[5] > 200)
     {
        motorL.setSpeed(BASE_SPEED);
        motorR.setSpeed(BASE_SPEED);
        motorL.run(FORWARD);
        motorR.run(FORWARD);
        delay(25);
        position = qtrrc.readLine(sensorValues);

        if(sensorValues[0] < 200)
        {
            while(sensorValues[4] > 200)
            {
               position = qtrrc.readLine(sensorValues);
               motorL.setSpeed(BASE_SPEED);
               motorR.setSpeed(BASE_SPEED);
               motorL.run(FORWARD);
               motorR.run(FORWARD);
            }
            if(sensorValues[2] > 200 || sensorValues[3] > 200)
            {

               path[pathLenght] = 'S';
               pathLenght ++; 
            }
            else
            {
               turnRight();
               path[pathLenght] = 'R';
               pathLenght ++;
            }
        }
        
     }
     else if(sensorValues[2] < 100 && sensorValues[3] < 100)
     {
        
          
         turnBack();
         path[pathLenght] = 'B';
         pathLenght ++;
     }
    
}

void PID()
{
  position = qtrrc.readLine(sensorValues);
  int error = position - 2500;

  int motorSpeed = Kp * error + Kd * (error - lastError);
  lastError = error;

  int rightMotorSpeed = BASE_SPEED - motorSpeed;
  int leftMotorSpeed = BASE_SPEED + motorSpeed;
  if (rightMotorSpeed > MAX_SPEED ) rightMotorSpeed = MAX_SPEED; // prevent the motor from going beyond max speed
  if (leftMotorSpeed > MAX_SPEED ) leftMotorSpeed = MAX_SPEED; // prevent the motor from going beyond max speed
  if (rightMotorSpeed < 0) rightMotorSpeed = 0; // keep the motor speed positive
  if (leftMotorSpeed < 0) leftMotorSpeed = 0; // keep the motor speed positive

  motorL.setSpeed(leftMotorSpeed);
  motorR.setSpeed(rightMotorSpeed);
  motorL.run(FORWARD);
  motorR.run(FORWARD);
}

void turnLeft()
{
  digitalWrite(13, HIGH);
  
  position = qtrrc.readLine(sensorValues);
  
  while(sensorValues[2] > 150)
  {
     position = qtrrc.readLine(sensorValues);
     motorR.setSpeed(80);
     motorL.setSpeed(70);
     motorR.run(FORWARD);
     motorL.run(BACKWARD); 
  }
  
  while(sensorValues[0] < 150)
  {
     position = qtrrc.readLine(sensorValues);
     motorR.setSpeed(80);
     motorL.setSpeed(70);
     motorR.run(FORWARD);
     motorL.run(BACKWARD); 
  }
  while(sensorValues[2] < 150)
  {
     position = qtrrc.readLine(sensorValues);
     motorR.setSpeed(70);
     motorL.setSpeed(62);
     motorR.run(FORWARD);
     motorL.run(BACKWARD); 
  }
  /*while(sensorValues[3] < 300)
  {
     position = qtrrc.readLine(sensorValues);
     motorR.setSpeed(0);
     motorL.setSpeed(100);
     motorR.run(FORWARD);
     motorL.run(BACKWARD); 
  }*/
  
  digitalWrite(13, LOW);
  

}

void turnRight()
{

position = qtrrc.readLine(sensorValues);

  //E bine dapunemc stanga
 /* while(sensorValues[4] < 500)
  {
  position = qtrrc.readLine(sensorValues);
  motorR.setSpeed(70);
  motorL.setSpeed(70);
  motorR.run(BACKWARD);
  motorL.run(FORWARD); 
  }
  while(sensorValues[3] < 500)
  {
  position = qtrrc.readLine(sensorValues);
  motorR.setSpeed(70);
  motorL.setSpeed(70);
  motorR.run(BACKWARD);
  motorL.run(FORWARD); 
  }
  while(sensorValues[2] < 500)
  {
  position = qtrrc.readLine(sensorValues);
  motorR.setSpeed(70);
  motorL.setSpeed(70);
  motorR.run(BACKWARD);
  motorL.run(FORWARD); 
  }
  while(sensorValues[1] < 500)
  {
  position = qtrrc.readLine(sensorValues);
  motorR.setSpeed(70);
  motorL.setSpeed(70);
  motorR.run(BACKWARD);
  motorL.run(FORWARD); 
  }
  
  while(sensorValues[3] < 500)
  {
  position = qtrrc.readLine(sensorValues);
  motorR.setSpeed(70);
  motorL.setSpeed(70);
  motorR.run(FORWARD);
  motorL.run(BACKWARD); 
  }*/

  while(sensorValues[3] > 150)
  {
     position = qtrrc.readLine(sensorValues);
     motorR.setSpeed(70);
     motorL.setSpeed(80);
     motorR.run(BACKWARD);
     motorL.run(FORWARD); 
  }
  while(sensorValues[3] < 150)
  {
     position = qtrrc.readLine(sensorValues);
     motorR.setSpeed(70);
     motorL.setSpeed(80);
     motorR.run(BACKWARD);
     motorL.run(FORWARD);  
  }
}

void turnBack()
{

  position = qtrrc.readLine(sensorValues);
  
 /* while(sensorValues[1] < 400)
  {
  position = qtrrc.readLine(sensorValues);
  motorR.setSpeed(70);
  motorL.setSpeed(70);
  motorR.run(FORWARD);
  motorL.run(BACKWARD); 
  }
  */

  motorL.setSpeed(70);
  motorR.setSpeed(70);
  motorL.run(FORWARD);
  motorR.run(FORWARD);
  delay(80);
  position = qtrrc.readLine(sensorValues);
   while(sensorValues[2] > 150)
  {
     position = qtrrc.readLine(sensorValues);
     motorR.setSpeed(45);
     motorL.setSpeed(90);
     motorR.run(FORWARD);
     motorL.run(BACKWARD); 
  }
  
  while(sensorValues[0] < 150)
  {
     position = qtrrc.readLine(sensorValues);
     motorR.setSpeed(35);
     motorL.setSpeed(70);
     motorR.run(FORWARD);
     motorL.run(BACKWARD); 
  }
  while(sensorValues[2] < 150)
  {
     position = qtrrc.readLine(sensorValues);
     motorR.setSpeed(10);
     motorL.setSpeed(30);
     motorR.run(FORWARD);
     motorL.run(BACKWARD); 
  }
  position = qtrrc.readLine(sensorValues);
    

  /*motorL.setSpeed(70);
  motorR.setSpeed(70);
  motorL.run(BACKWARD);
  motorR.run(BACKWARD);
  delay(90);*/

}

void gata()
{
  motorL.setSpeed(0);
  motorR.setSpeed(0);
  delay(replayTime);
  shortenPath();
  replay();
}

void replay()
{
  while(true)
  {
   position = qtrrc.readLine(sensorValues);
    while(sensorValues[0] < 150 && sensorValues[5] < 150)
    {
      PID();
    }
    
    position = qtrrc.readLine(sensorValues);
    if(sensorValues[0] > 150 || sensorValues[5] > 150)
    {
     /* motorL.setSpeed(70);
      motorR.setSpeed(70);
      motorL.run(FORWARD);
      motorR.run(FORWARD);
      delay(100);*/
      
      
      if(path[readLenght] == 'L') turnLeft();
      else if(path[readLenght] == 'R') 
      {
        while(sensorValues[5] > 200)
        {
               position = qtrrc.readLine(sensorValues);
               motorL.setSpeed(BASE_SPEED);
               motorR.setSpeed(BASE_SPEED);
               motorL.run(FORWARD);
               motorR.run(FORWARD);
         }
        turnRight();
      }
      else if(path[readLenght] == 'G') oprire();
      else if(path[readLenght] == 'S') 
      {
        while(sensorValues[0] > 150 || sensorValues[5] > 150)
        {
            position = qtrrc.readLine(sensorValues);

            motorL.setSpeed(BASE_SPEED);
            motorR.setSpeed(BASE_SPEED);
            motorL.run(FORWARD);
            motorR.run(FORWARD);
        }
      }
      
      readLenght ++;
    }
  }
}

void shortenPath()
{
    char a[120];
    strcpy(a, path); //labirintul
    
    char b[120];
    char t[120];
    
    while(strcmp(a, b) != 0) {
    strcpy(b, a); //sa verific daca se schimba sirul, daca nu inseamna ca a ajuns la minim  
    for(int i = 0; i < strlen(a) - 2; i++)
    {
        if(a[i] == 'L' && a[i + 1] == 'B' && a[i + 2] == 'R') //if urile astea cauta combinatii de cate 3 miscari sa optimizezi drumul
        {
            a[i] = 'B';
            strcpy(t, a + i + 3);
            strcpy(a + i + 1, t);
        }
        
        if(a[i] == 'L' && a[i + 1] == 'B' && a[i + 2] == 'S')
        {
             a[i] = 'R';
             strcpy(t, a + i + 3);
             strcpy(a + i + 1, t);
        }
        
        if(a[i] == 'L' && a[i + 1] == 'B' && a[i + 2] == 'L')
        {
             a[i] = 'S';
             strcpy(t, a + i + 3);
             strcpy(a + i + 1, t);   
        }
        
        if(a[i] == 'S' && a[i + 1] == 'B' && a[i + 2] == 'L')
        {
             a[i] = 'R';
             strcpy(t, a + i + 3);
             strcpy(a + i + 1, t);
        }
        
        if(a[i] == 'S' && a[i + 1] == 'B' && a[i + 2] == 'S')
        {
             a[i] = 'B';
             strcpy(t, a + i + 3);
             strcpy(a + i + 1, t);
        }
        
        if(a[i] == 'R' && a[i + 1] == 'B' && a[i + 2] == 'L')
        {
             a[i] = 'B';
             strcpy(t, a + i + 3);
             strcpy(a + i + 1, t);
        }
    }
     
    }

    strcpy(path, a);
   
}

void oprire()
{
  motorR.setSpeed(0);
  motorL.setSpeed(0);
  delay(1000);
  oprire();
}

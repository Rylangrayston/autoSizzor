#define FWD  HIGH
#define REV  LOW

int numberOfCuts = 0;
int mmCutLength = 1;

// to caluclate this take the curent stepsPermm and multiply it by the requeste length to get the number of steps per length... 
// divide stepsPerLength by the actual measured lenght in mm to .. enter the result as the new stepsPermm
int stepsPermm = 47;   //  40 for heat shrink......... 52 for red wire... 47 for black wire

int stepsPerCut = 1;
int retractBeforStartSteps = 200;
boolean firstRunSinceZeroingMaterial = false;


int motorEnablePin = 8;

int delayBetweenSteps = 100;
int delayBetweenMoves = delayBetweenSteps * 5;

int mmRetractDistance = 5;
int stepsPerRetract = stepsPermm * mmRetractDistance;

int motStepPin = 10;
int motDirPin = 11;

int cutterActuatorMoveTime = 50;
int cutterPulseTime = 50;
int closeCutterPin = 4;
int openCutterPin = 3;


 
void moveMotor(int steps, boolean dir)
{
  digitalWrite(motDirPin,dir);
  delayMicroseconds(delayBetweenMoves);
  for (int stepCount = 0; stepCount < steps; stepCount++)
  {
    digitalWrite(motStepPin,HIGH);
    delayMicroseconds(delayBetweenSteps);
    digitalWrite(motStepPin,LOW);
    delayMicroseconds(delayBetweenSteps);
    //Serial.println(stepCount);
  }
}


void retract()
{
  moveMotor(stepsPerRetract, REV);
  moveMotor(stepsPerRetract, FWD);
}

void advance()
{
  moveMotor(stepsPerCut, FWD);
}


void closeCutter()
{
 digitalWrite(closeCutterPin, HIGH);
 delay(cutterPulseTime);
 digitalWrite(closeCutterPin, LOW);
 delay(cutterActuatorMoveTime);  
}

void openCutter()
{
  digitalWrite(openCutterPin, HIGH);
  delay(cutterPulseTime);
  digitalWrite(openCutterPin, LOW);
  delay(cutterActuatorMoveTime);
} 




void cut()
{
  closeCutter();
  openCutter();
}


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.setTimeout(2147483147);
pinMode( motStepPin,OUTPUT);
pinMode( motDirPin,OUTPUT);
pinMode( openCutterPin,OUTPUT);
pinMode( closeCutterPin,OUTPUT);
pinMode( motorEnablePin,OUTPUT);
digitalWrite(motorEnablePin, HIGH);




}



void runJob()
{
 
  digitalWrite(motorEnablePin, LOW);
  
  openCutter();
  moveMotor(retractBeforStartSteps, FWD);
  
  for (int cutCount = 0; cutCount < numberOfCuts; cutCount++ )
  {
    retract();
    advance();
    cut();
    Serial.print("cut Lenght in mm = ");
    Serial.println(mmCutLength);
    Serial.print(cutCount);
    Serial.print(" Out of ");
    Serial.println(numberOfCuts);
    
   
  }
  
  moveMotor(retractBeforStartSteps, REV);
  closeCutter();
  digitalWrite(motorEnablePin, HIGH);
  Serial.println("done!");
}

void loadMaterial()
{
  Serial.println("turn the nob to adjust the material starting point");
  Serial.println("hit q when done");
  firstRunSinceZeroingMaterial = false;

  //delay(200);
  //Serial.flush();
  digitalWrite(motorEnablePin, LOW);
  while (true) 
  {
    if (Serial.read() == 113)
      {
        if (firstRunSinceZeroingMaterial)
        {
          moveMotor(retractBeforStartSteps, REV);
        }
        break;
      }
    int nobValue =  analogRead(A0);
    delay(5);
    if (nobValue > 700)
    {
      moveMotor(1,FWD);
      firstRunSinceZeroingMaterial = true;
    }
    if (nobValue < 300)
    {
      moveMotor(1,REV);
      firstRunSinceZeroingMaterial = true;
    }
  }
  digitalWrite(motorEnablePin, HIGH);
}

void loop() {
 


Serial.println("How many pices do you want to cut?");
numberOfCuts = Serial.parseInt();
Serial.println("What Length do you want to cut in mm");
mmCutLength = Serial.parseInt();
stepsPerCut = stepsPermm * mmCutLength;

loadMaterial();

Serial.println("ok type 99 to start running job or any other number to re enter values");
int goOrReset = Serial.parseInt();

if ( goOrReset == 99)
{
  runJob();  
}




}

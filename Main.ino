int RelayMaster = 22;
int RelayOperation = 23;
int RelayLeft = 24;
int RelayRight = 25;
int RelayHB = 26;
int RelayBrake = 27;
int RelayHorn = 28;
int RelayStart = 29;
//
int LightN = 31;
int LightRed = 30;
int LightRight = 32;
int LightLeft = 33;

int StarterOut = 34;

int StarterIn = 47; //Gray
int starterIn;
int SwitchRight = 46; //White
int switchRight;
int SwitchLeft = 45; //Black
int switchLeft;
int SwitchHB = 44; //Brown
int switchHB;
int ButtonHorn = 43; //Red
int buttonHorn;
int SwitchHazard = 42; //Orange ------
int switchHazard;
int LockToggleSwitch = 41; //Yellow ------
int lockToggleSwitch;

int SensorKS = 40; //Green
int sensorKS;
int SensorEP=39; //Blue ------
int sensorEP;
int SensorN = 38; //Purple ------
int sensorN;
int SensorFrontBrake = 37; //Gray
int sensorFrontBrake;
int SensorRearBrake=36; //White
int sensorRearBrake;
int SensorKill=35; //Black on edge ot tape
int sensorKill;

long hazardTicker;
long turnTicker;
long hornLightTicker;
long sensorEPLightTicker;
int ledflashspeed;
bool coilsOn=0;
long coilsOnTicker;
bool engineOn=0;
long engineOnTicker;
bool turnOn=0;
bool hazardOn=0;
bool operationOn=0;
bool unlock=0;
String string1;
String string2;

void setup() {

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  //OUTPUTS-----------------------
  //RELAYS
  pinMode(RelayMaster, OUTPUT);
  pinMode(RelayOperation, OUTPUT);
  pinMode(RelayRight, OUTPUT);
  pinMode(RelayLeft, OUTPUT);
  pinMode(RelayHB, OUTPUT);
  pinMode(RelayBrake, OUTPUT);
  pinMode(RelayHorn, OUTPUT);
  pinMode(RelayStart, OUTPUT);
  //LED Lights
  pinMode(LightN, OUTPUT);
  pinMode(LightRed, OUTPUT);
  pinMode(LightRight, OUTPUT);
  pinMode(LightLeft, OUTPUT);
//  Starter Trigger
  pinMode(StarterOut, OUTPUT);
//  Built in LED
  pinMode(13, OUTPUT);

  //  INPUTS --------------------------
  // Buttons and Switches
  pinMode(StarterIn, INPUT);
  pinMode(SwitchRight, INPUT);
  pinMode(SwitchLeft, INPUT);
  pinMode(SwitchHB, INPUT);
  pinMode(ButtonHorn, INPUT);
  pinMode(SwitchHazard, INPUT);
  pinMode(LockToggleSwitch, INPUT);

  // Sensors
  pinMode(SensorKS, INPUT);
  pinMode(SensorEP, INPUT);
  pinMode(SensorN, INPUT);
  pinMode(SensorFrontBrake, INPUT);
  pinMode(SensorRearBrake, INPUT);
  pinMode(SensorKill, INPUT);


  
  //-------------------------------------
  //Setup Actions
  
  //Set Non-Running State
  StateOff();
}

void loop() {
  ReadAll();

  //State Off
  if (sensorKill == LOW){
    StateOff();
    return;
  }else{
    //Run loop logic

    //Master Access
    if (unlock == 1){
      digitalWrite(RelayMaster, LOW);
      coilsOn = 1;
    }else{
      digitalWrite(RelayMaster, HIGH);
      coilsOn = 0;
      //Reset coilsOnTicker to zero so new millis can be set to it on next fireup
      coilsOnTicker = 0;
    }
    
    //Master Access Timeout
    if (coilsOnTicker != 0 && (millis() - coilsOnTicker > 120000) && operationOn == 0){
      //the coils have been energized for 2 minutes since last unlock, but the engine is seemingly not running
      //Kill master relay
      StateOff();
      Serial.println("Timeout");
      coilsOn = 0;
      //Reset coilsOnTicker to zero so new millis can be set to it on next fireup
      coilsOnTicker = 0;
    }
   

    //LED Display
    if (unlock == 1 && sensorN == HIGH){
      digitalWrite(LightN, HIGH);
    }else{
      digitalWrite(LightN, LOW);
    }
//    if (lockToggleSwitch == HIGH && sensorEP == LOW){
//      sensorEPLightTicker = BlinkLight(LightN, 0, LightEP, 0, 50, sensorEPLightTicker);
//    }else{
//      digitalWrite(LightEP, LOW);
//    }

    //Horn
    if (unlock == 1 && buttonHorn == HIGH){
      digitalWrite(RelayHorn, LOW);
//      hornLightTicker = BlinkLight(RelayLeft, RelayRight, LightLeft, LightRight, 300, hornLightTicker);
      Serial.println("Horn Activated");
    }else{
      digitalWrite(RelayHorn, HIGH);
    }

    //HighBeam
    if (unlock == 1 && switchHB == HIGH){
      digitalWrite(RelayHB, LOW);
    }else{
      digitalWrite(RelayHB, HIGH);
    }

    //Operation
    if(unlock == 1 && starterIn == HIGH && sensorKS == HIGH){
      //Turn on Operation stage
      digitalWrite(RelayOperation, LOW);
      if (operationOn == 0){ //then this is the first turn-on of the operation
        starterIn = LOW;
        delay(2000); //Delay to avoid operation turn-on before starter
        operationOn = 1; //The operation will remain on until the StateOff() resets it to zero
      }
    }
    
    //Starter
    if(unlock == 1 && starterIn == HIGH && sensorKS == HIGH && sensorN == HIGH && operationOn == 1){
      //Bike is not locked, starter is pressed, the kickstand is up, and the bike is in Neutral
      //Run current to Starter Solenoid
      Serial.println("Starter");
      digitalWrite(RelayStart, LOW);
    }else{
      //Turn off starter
      digitalWrite(RelayStart, HIGH);
    }

    //Brake
    if (unlock == 1 && (sensorFrontBrake == HIGH || sensorRearBrake == HIGH)){
      digitalWrite(RelayBrake, LOW);
    }else{
      digitalWrite(RelayBrake, HIGH);
    }

//    //Hazards
//   if(unlock == 1 && switchHazard == HIGH && hazardOn == 0 && switchRight == LOW && switchLeft == LOW){
//     //Bike is not locked and hazard is switched on
//     //benchmark the start of the "hazard on" period with hazardTicker
//     hazardTicker = millis();
//     //Deny access to this block, initiate hazard state
//     hazardOn = 1;
//      digitalWrite(RelayRight, LOW);
//      digitalWrite(LightRight, HIGH);
//      digitalWrite(RelayLeft, LOW);
//      digitalWrite(LightLeft, HIGH);
//   }else if (lockToggleSwitch == HIGH && switchHazard == LOW && switchRight == LOW && switchLeft == LOW){
//    //Bike is not locked, and the hazard is switched off, and other turn signals are not running
//    hazardOn = 0;
//    digitalWrite(RelayRight, HIGH);
//    digitalWrite(LightRight, LOW);
//    digitalWrite(RelayLeft, HIGH);
//    digitalWrite(LightLeft, LOW);
//   }else if (lockToggleSwitch == HIGH && switchHazard == HIGH && switchRight == LOW && switchLeft == LOW){
//    //Bike is not locked, the hazard is switched on, and the other turn signals are not running
//    hazardTicker = BlinkLight(RelayRight, RelayLeft, LightRight, LightLeft, 300, hazardTicker);
//   }
  }

    //Right Turn
   if(unlock == 1 && switchRight == HIGH && turnOn == 0 && switchHazard == LOW && switchLeft == LOW){
     //Bike is not locked and right turn is just now switched on
     //benchmark the start of the "turn on" period with turnTicker
     turnTicker = millis();
     //Deny access to this block, initiates right turn state
     turnOn = 1;
     digitalWrite(RelayRight, LOW);
     digitalWrite(LightRight, HIGH);
     digitalWrite(RelayLeft, HIGH);
     digitalWrite(LightLeft, LOW);
   }else if (unlock == 1 && switchRight == LOW && switchHazard == LOW && switchLeft == LOW){
    //Bike is not locked, and the hazard is switched off, and other turn signals are not running
    turnOn = 0;
    digitalWrite(RelayRight, HIGH);
    digitalWrite(LightRight, LOW);
    digitalWrite(RelayLeft, HIGH);
    digitalWrite(LightLeft, LOW);
   }else if(unlock == 1 && switchRight == HIGH){
    digitalWrite(RelayLeft, HIGH);
    digitalWrite(LightLeft, LOW);
    turnTicker = BlinkLight(RelayRight, 0, LightRight, 0, 300, turnTicker);
   }
     
   //Left Turn
   if(unlock == 1 && switchLeft == HIGH && turnOn == 0 && switchHazard == LOW && switchRight == LOW){
     //Bike is not locked and right turn is just now switched on
     //benchmark the start of the "turn on" period with turnTicker
     turnTicker = millis();
     //Deny access to this block, initiates right turn state
     turnOn = 1;
     digitalWrite(RelayRight, HIGH);
     digitalWrite(LightRight, LOW);
     digitalWrite(RelayLeft, LOW);
     digitalWrite(LightLeft, HIGH);
   }else if (unlock == 1 && switchLeft == LOW && switchHazard == LOW && switchRight == LOW){
    //Bike is not locked, and the hazard is switched off, and other turn signals are not running
    turnOn = 0;
    digitalWrite(RelayRight, HIGH);
    digitalWrite(LightRight, LOW);
    digitalWrite(RelayLeft, HIGH);
    digitalWrite(LightLeft, LOW);
   }else if(unlock == 1 && switchLeft == HIGH){
    digitalWrite(RelayRight, HIGH);
    digitalWrite(LightRight, LOW);
    turnTicker = BlinkLight(RelayLeft, 0, LightLeft, 0, 300, turnTicker);
   }  
}


void ReadAll(){
//Method for read of all sensors
  starterIn = digitalRead(StarterIn);
  string1 = "Start:";
  string2 = string1 + starterIn;
  Serial.print(string2);
  
  switchRight = digitalRead(SwitchRight);
  string1 = "  R:";
  string2 = string1 + switchRight;
  Serial.print(string2);
  
  switchLeft = digitalRead(SwitchLeft);
  string1 = "  L:";
  string2 = string1 + switchLeft;
  Serial.print(string2);
  
  switchHB = digitalRead(SwitchHB);
  string1 = "  HB:";
  string2 = string1 + switchHB;
  Serial.print(string2);
  
  buttonHorn = digitalRead(ButtonHorn);
  string1 = "  Horn:";
  string2 = string1 + buttonHorn;
  Serial.print(string2);
  
//  switchHazard = digitalRead(SwitchHazard);
//  string1 = "  Hzrd:";
//  string2 = string1 + switchHazard;
//  Serial.print(string2);
  
  lockToggleSwitch = HIGH; //==digitalRead(LockToggleSwitch);
  string1 = "  LockToggle:";
  string2 = string1 + lockToggleSwitch;
  Serial.print(string2); 
  
  sensorKS = HIGH;// digitalRead(SensorKS);
  string1 = "   KS:";
  string2 = string1 + sensorKS;
  Serial.print(string2);
  
  sensorEP = digitalRead(SensorEP);
  string1 = "  EP:";
  string2 = string1 + sensorEP;
  Serial.print(string2);
  
  sensorN = HIGH; //digitalRead(SensorN);
  string1 = "   N:";
  string2 = string1 + sensorN;
  Serial.print(string2);
  
  sensorFrontBrake = digitalRead(SensorFrontBrake);
  string1 = "  FBrk:";
  string2 = string1 + sensorFrontBrake;
  Serial.print(string2);
  
  sensorRearBrake = digitalRead(SensorRearBrake);
  string1 = "  RBrk:";
  string2 = string1 + sensorRearBrake;
  Serial.print(string2);
  
  sensorKill = digitalRead(SensorKill);
  string1 = "  On/Off:";
  string2 = string1 + sensorKill;
  Serial.print(string2);

  if (unlock == 0){
    Serial.print("Check");
    CheckUnlockCode();
  }else{
    Serial.println("  Unlock: 1");
  }
}

//Function for unlock sequence     With Toggle on:      Combo:   Horn --1s--> FrontBrake --1s--> Both Hold
                                  //Edited code: High Beam on, hold horn and starter buttons for 1 second
void CheckUnlockCode(){ 
  if(lockToggleSwitch == HIGH && digitalRead(SwitchHB)==HIGH && digitalRead(ButtonHorn)==HIGH && digitalRead(StarterIn) == HIGH){
    Serial.print(" Unlock Stage 1");
    digitalWrite(13, HIGH);
    delay(300);
    if(lockToggleSwitch == HIGH && digitalRead(SwitchHB)==HIGH && digitalRead(ButtonHorn)==HIGH && digitalRead(StarterIn) == HIGH){
      Serial.print(" Unlock Stage 2");
      digitalWrite(13, LOW);
      delay(300);
      if(lockToggleSwitch == HIGH && digitalRead(SwitchHB)==HIGH && digitalRead(ButtonHorn)==HIGH && digitalRead(StarterIn) == HIGH){
        Serial.print(" Unlock Stage 3");
        digitalWrite(13, HIGH);
        delay(300);
        if(lockToggleSwitch == HIGH && digitalRead(SwitchHB)==HIGH && digitalRead(ButtonHorn)==HIGH && digitalRead(StarterIn) == HIGH){
          unlock = 1;
          buttonHorn = LOW;
          digitalWrite(RelayMaster, HIGH);
          digitalWrite(RelayOperation, HIGH);

          ledflashspeed = 65;

          digitalWrite(LightRed, HIGH);
          delay(ledflashspeed);
          digitalWrite(LightN, HIGH);
          delay(ledflashspeed);
          digitalWrite(LightRight, HIGH);
          delay(ledflashspeed);
          digitalWrite(LightLeft, HIGH);
          delay(ledflashspeed);
          
          digitalWrite(LightRed, LOW);
          delay(ledflashspeed);
          digitalWrite(LightN, LOW);
          delay(ledflashspeed);
          digitalWrite(LightRight, LOW);
          delay(ledflashspeed);
          digitalWrite(LightLeft, LOW);
          delay(ledflashspeed*2);

          digitalWrite(LightRed, HIGH);
          delay(ledflashspeed);
          digitalWrite(LightN, HIGH);
          delay(ledflashspeed);
          digitalWrite(LightRight, HIGH);
          delay(ledflashspeed);
          digitalWrite(LightLeft, HIGH);
          delay(ledflashspeed);
          
          digitalWrite(LightRed, LOW);
          delay(ledflashspeed);
          digitalWrite(LightN, LOW);
          delay(ledflashspeed);
          digitalWrite(LightRight, LOW);
          delay(ledflashspeed);
          digitalWrite(LightLeft, LOW);
          delay(150);

          digitalWrite(RelayOperation, HIGH);
          digitalWrite(RelayMaster, LOW);
          starterIn = LOW;
          buttonHorn= LOW;

          //The bike was just now unlocked, the coilsOnTicker will be set to zero
          coilsOnTicker = millis(); //benchmark current millis
        }
      }
    }
  }else{
    Serial.print("  all fail ");
    digitalWrite(13,LOW);
    unlock = 0;
  }
  
  string1 = "  Unlock:";
  string2 = string1 + unlock;
  Serial.println(string2);
}

//Function for blinking
long BlinkLight(int Relay1, int Relay2, int Light1, int Light2, int Delay, long Ticker){
//  Serial.print(Delay);
//  Serial.print(" & ");
//  Serial.print(Ticker);
//  Serial.print(" = ");
//  Serial.print(Delay + Ticker);
//  Serial.print("            ");
//  Serial.print(millis());
//  Serial.print("            ");
  if(millis() - Ticker < Delay){
    if (Relay1 != 0 && Relay2 != 0){
      digitalWrite(Relay1, LOW);
      digitalWrite(Light1, HIGH);
      digitalWrite(Relay2, LOW);
      digitalWrite(Light2, HIGH);
    }else{
      digitalWrite(Relay1, LOW);
      digitalWrite(Light1, HIGH);
    }
//    Serial.println("Turned On");
  }
  else if(millis() - Ticker >= Delay){ 
    if (Relay1 != 0 && Relay2 != 0){
      digitalWrite(Relay1, HIGH);
      digitalWrite(Light1, LOW);
      digitalWrite(Relay2, HIGH);
      digitalWrite(Light2, LOW);
    }else{
      digitalWrite(Relay1, HIGH);
      digitalWrite(Light1, LOW);
    }
//    Serial.println("Turned Off");
    if(millis() - Ticker >= 2*Delay){
      Ticker = millis();
    }
  }
  return Ticker;
}

//Method for setting all outputs appropriately for off state.
void StateOff(){
  //Block Master Access Initially
  digitalWrite(RelayMaster, HIGH);

  //Turn off all Relays, Lights, and Starter
  digitalWrite(RelayOperation, HIGH);
  digitalWrite(RelayRight, HIGH);
  digitalWrite(RelayLeft, HIGH);
  digitalWrite(RelayHB, HIGH);
  digitalWrite(RelayBrake, HIGH);
  digitalWrite(RelayHorn, HIGH);
  digitalWrite(RelayStart, HIGH);
  digitalWrite(LightN, LOW);
  digitalWrite(LightRed, LOW);
  digitalWrite(LightRight, LOW);
  digitalWrite(LightLeft, LOW);
  operationOn = 0; //This forces the 1000 millisecond delay when the operation is triggered on
  unlock = 0; //Lock bike on shutdown
}

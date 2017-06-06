//AKHIL Bandi

#define PIN_GLOBAL_SYNC        2        // starts board 1, and sends out the signal to the board 2

#define PIN_EXTERNAL_TRIGGER  13        // for single pulse
#define PIN_EXTERNAL_TRIGGER2 12        // for single pulse

#define PIN_TRIGGER_PULSE     11        // for pulse train
#define PIN_TRIGGER_PULSE2     10

#define SINGLEPULSE            0
#define PULSETRAIN             1
#define INTENSITYONE           2
#define INTENSITYTWO           3
#define SINGLE_FREQ            4
/*--------------------------
 variables
 --------------------------*/

char control_char;
int count = 0;

unsigned long timestamp;
unsigned long pulse_start;
boolean onSignal = false;
boolean onPulse = false;

int single_pulse_count = 0;

int pulse_count = 0;
int t_duration = 20; // longer than 10msec
//int freqArray[21] = {1,2,3,5,6,8,10,12,14,16,18,20,22,24,26,28,30,35,40,45,50} ; for mouse 1,2
int freqArray[6] = {1,3,7,10,15,20};
int freq;
int dur_count = 0;
int signalChange = 0;
int freq_count=0;
int loop_count = 0;
int freq_flag = 0;

/*-------------------------
 functions
 -------------------------*/

void setup() {

  pinMode(PIN_GLOBAL_SYNC, OUTPUT);
  pinMode(PIN_EXTERNAL_TRIGGER, OUTPUT);
  pinMode(PIN_EXTERNAL_TRIGGER2, OUTPUT);
  pinMode(PIN_TRIGGER_PULSE, OUTPUT);
  pinMode(PIN_TRIGGER_PULSE2, OUTPUT);

  digitalWrite(PIN_GLOBAL_SYNC, LOW);
  digitalWrite(PIN_EXTERNAL_TRIGGER, LOW);
  digitalWrite(PIN_EXTERNAL_TRIGGER2, LOW);
  digitalWrite(PIN_TRIGGER_PULSE, LOW);
  digitalWrite(PIN_TRIGGER_PULSE2, LOW);

  Serial.begin(9600);

  Serial.println("Enter s to start duration trials");
  Serial.println("Enter t to start frequency trials");
  Serial.println("Enter b to start stim-frequency trials");
  Serial.println("Enter i to start 15 50msec pulses for intensity stimulation");
  Serial.println("Enter o to start 8 50msec pulses for intensity stimulation");
  Serial.println("You can enter q(quit) during recording or p (pause)");
  Serial.println("Enter q to quit recording");

  onSignal = false;

}


void loop() {

  while (Serial.available() > 0) {
    control_char = Serial.read();

    if ( control_char == 't') {

      // output for the sync pulse to control light stim/ camera
      //  delay(100);
      //  digitalWrite(PIN_GLOBAL_SYNC, HIGH);
      onSignal = true;
      Serial.println("pulse train mode");
      signalChange = 1;
      count = 0;
      //count = 1;
      // timestamp = millis();
      // delay(100);
      //  digitalWrite(PIN_GLOBAL_SYNC, LOW);

    } 
    else if (control_char == 's') {


      onSignal = true;
      Serial.println("single pulse mode");
      signalChange = 0;

    } 
    else if (control_char == 'b'){
      onSignal = true;
      Serial.println("Stim-Frequency Trial");
      signalChange = 4;
      count = 0;
    }
    else if (control_char == 'i'){

      Serial.println("15 50msec pulses for intensity mode");
      onSignal = true;
      signalChange = 2;

    }
    else if (control_char == 'o'){

      Serial.println("8 50msec pulse for intensity mode");
      onSignal = true;
      signalChange = 3;

    }
    else if (control_char == 'q') {
      digitalWrite(PIN_GLOBAL_SYNC, LOW);
      onSignal = false;
      count = 0;
      break;
    } 
    else {
      Serial.println("Enter either s, t, p or q");
    }

  }

  while (onSignal)
  {
    if (Serial.available() > 0) {
      control_char = Serial.read();
      if (control_char == 'q') {
        digitalWrite(PIN_GLOBAL_SYNC, LOW);
        digitalWrite(PIN_TRIGGER_PULSE, LOW);
        onSignal = false;
        Serial.println("Trigger function stopped");
        count = 0;
        break;
      } 
      else if (control_char == 'p') {
        digitalWrite(PIN_GLOBAL_SYNC, LOW);
        digitalWrite(PIN_TRIGGER_PULSE, LOW);
        onSignal = false;
        Serial.println("Trigger function paused");
        break;
      }
    } 
    else {

      switch (signalChange) {
      case SINGLEPULSE:  //for single pulse with duration change

        delay(500);

        if (count < 8){
          if (single_pulse_count < 8){
            
            if (single_pulse_count == 0){

              Serial.print("Trial #:\t");
              Serial.println(count);
            }

            //digitalWrite(PIN_GLOBAL_SYNC, HIGH);
            delay(1000);
            switch(single_pulse_count) {
              case 0:
                Serial.print(1);
                Serial.println("msec duration");
                trigger(1);
                break;
              case 1:
                Serial.print(3);
                Serial.println("msec duration");
                trigger(3);
                break;
              case 2:
                Serial.print(5);
                Serial.println("msec duration");
                trigger(5);
                break;
              case 3:          
                Serial.print(10);
                Serial.println("msec duration");
                trigger(10);
                break;
              case 4:
                Serial.print(20);
                Serial.println("msec duration");
                trigger(20);
                break;
              case 5:
                Serial.print(50);
                Serial.println("msec duration");
                trigger(50);
                break;
              case 6:
                Serial.print(100);
                Serial.println("msec duration");
                trigger(100);
                break;
              case 7:
                Serial.print(1000);
                Serial.println ("msec duration");
                trigger(1000);
                break;
            }
            
            //delay(2000);
            //digitalWrite(PIN_GLOBAL_SYNC, LOW);
            delay(5000); 
            single_pulse_count ++;
          } 
          
          if (single_pulse_count == 8) {
            count++;
            single_pulse_count = 0;
          }

        }
        else if (count = 15) {
          if (single_pulse_count < 3){
              
              if (single_pulse_count == 0){
  
                Serial.print("Trial #:\t");
                Serial.println(count);
              }
  
              //digitalWrite(PIN_GLOBAL_SYNC, HIGH);
              //delay(1000);
              switch(single_pulse_count) {
                case 0:
                  trigger(1);
                  Serial.print(1);
                  Serial.println("msec duration");
                  break;
                case 1:
                  trigger(3);
                  Serial.print(3);
                  Serial.println("msec duration");
                  break;
//                case 2:
//                  trigger(20);
//                  Serial.print(20);
//                  Serial.println("msec duration");
//                  break;
              }
              
              //delay(2000);
              //digitalWrite(PIN_GLOBAL_SYNC, LOW);
              delay(5000); 
              single_pulse_count ++;
            } 
            
            if (single_pulse_count == 3) {
              count++;
              single_pulse_count = 0;
            }
  
            if(count == 15){
              count = 0;
              onSignal = 0;
              digitalWrite(PIN_GLOBAL_SYNC, LOW);
              single_pulse_count = 0;
              Serial.println("Duration Trial complete");
            }
        } 

        break;

      case PULSETRAIN:

        if(count<6){

          if (loop_count <8){

            if (freq_count == 0){

              freq = freqArray[count];
              Serial.print("Frequency:");
              Serial.print(freq);
              Serial.println("Hz");
              Serial.print("Trial#:");
              Serial.println(loop_count+1);

              digitalWrite(PIN_GLOBAL_SYNC, HIGH);
              delay(1000);

            }

            if (freq_count <freq){

              //            Serial.println(freq_count+1);
              digitalWrite(PIN_TRIGGER_PULSE, HIGH);
              digitalWrite(PIN_TRIGGER_PULSE2, HIGH);
              delay(t_duration);
              digitalWrite(PIN_TRIGGER_PULSE, LOW);
              digitalWrite(PIN_TRIGGER_PULSE2, LOW);
              delay((1000/freq)-t_duration);

              freq_count++;

            }
            else{

              freq_count = 0;
              delay(2000);
              digitalWrite(PIN_GLOBAL_SYNC, LOW);
              loop_count++;
              delay(5000);
            }
          } 
          else{
            count++;
            loop_count = 0;
            digitalWrite(PIN_GLOBAL_SYNC, LOW);
            digitalWrite(PIN_TRIGGER_PULSE, LOW);
            digitalWrite(PIN_TRIGGER_PULSE2, LOW);
          }
        }
        else{

          // count = 0;
          digitalWrite(PIN_GLOBAL_SYNC, LOW);
          digitalWrite(PIN_TRIGGER_PULSE, LOW);
          digitalWrite(PIN_TRIGGER_PULSE2, LOW);
          onSignal = 0;

        }


        break;

      case INTENSITYONE:
        while (count < 15) {
          digitalWrite(PIN_GLOBAL_SYNC, HIGH);
          delay(1000);
          digitalWrite(PIN_TRIGGER_PULSE, HIGH);
          delay(40);
          digitalWrite(PIN_TRIGGER_PULSE, LOW);
          delay(1000);
          digitalWrite(PIN_GLOBAL_SYNC, LOW);
          digitalWrite(PIN_TRIGGER_PULSE, LOW);
          Serial.println(count);
          delay(40000);
          count++;
        }
        onSignal = 0;
        count = 0;
        Serial.println("15 Trials complete");

        break;
        
      case INTENSITYTWO:
        while (count < 8) {
          digitalWrite(PIN_GLOBAL_SYNC, HIGH);
          delay(500);
          digitalWrite(PIN_TRIGGER_PULSE, HIGH);
          delay(50);
          digitalWrite(PIN_TRIGGER_PULSE, LOW);
          delay(1500);
          digitalWrite(PIN_GLOBAL_SYNC, LOW);
          digitalWrite(PIN_TRIGGER_PULSE, LOW);
          Serial.println(count);
          delay(5000);
          count++;
        }
        onSignal = 0;
        count = 0;
        Serial.println("8 Trials complete");

        break;

      case SINGLE_FREQ:
      
        if(count<6){

          if (loop_count <8){

            if (freq_count == 0){

              freq = freqArray[count];
              Serial.print("Frequency:");
              Serial.print(freq);
              Serial.print("Hz");
              Serial.print("  Trial#:");
              Serial.println(loop_count+1);

              digitalWrite(PIN_GLOBAL_SYNC, HIGH);
              delay(1000);
              digitalWrite(PIN_TRIGGER_PULSE, HIGH);
              digitalWrite(PIN_TRIGGER_PULSE2, HIGH);
              delay(50);
              digitalWrite(PIN_TRIGGER_PULSE, LOW);
              digitalWrite(PIN_TRIGGER_PULSE2, LOW);
              delay(1000);

            }

            if (freq_count <freq){

              //            Serial.println(freq_count+1);
              digitalWrite(PIN_TRIGGER_PULSE, HIGH);
              digitalWrite(PIN_TRIGGER_PULSE2, HIGH);
              delay(t_duration);
              digitalWrite(PIN_TRIGGER_PULSE, LOW);
              digitalWrite(PIN_TRIGGER_PULSE2, LOW);
              delay((1000/freq)-t_duration);

              freq_count++;

            }
            else{

              freq_count = 0;
              delay(950);
              digitalWrite(PIN_GLOBAL_SYNC, LOW);
              loop_count++;
              delay(8000);
            }
          } 
          else{
            count++;
            loop_count = 0;
            digitalWrite(PIN_GLOBAL_SYNC, LOW);
            digitalWrite(PIN_TRIGGER_PULSE, LOW);
            digitalWrite(PIN_TRIGGER_PULSE2, LOW);
          }
        }
        else{

          // count = 0;
          digitalWrite(PIN_GLOBAL_SYNC, LOW);
          digitalWrite(PIN_TRIGGER_PULSE, LOW);
          digitalWrite(PIN_TRIGGER_PULSE2, LOW);
          onSignal = 0;

        }


        break;
       
      }//end of case

    }//end of else

  } //end of while


}      //end of loop()

void trigger (int duration){ 
  digitalWrite(PIN_GLOBAL_SYNC, HIGH);
   delay(1000);
//  Serial.println("Activating Light One");
  digitalWrite(PIN_EXTERNAL_TRIGGER, HIGH);
  // digitalWrite(PIN_EXTERNAL_TRIGGER2, HIGH);
  digitalWrite(PIN_TRIGGER_PULSE, HIGH);
  delay(duration);
  //  digitalWrite(PIN_EXTERNAL_TRIGGER2, LOW);
  digitalWrite(PIN_EXTERNAL_TRIGGER, LOW);
  digitalWrite(PIN_TRIGGER_PULSE, LOW);
  delay(2000-duration);
  digitalWrite(PIN_GLOBAL_SYNC, LOW);
  delay(5000);
  
//  delay(5000); 
  
//  digitalWrite(PIN_GLOBAL_SYNC, HIGH);
//  delay(1000);
//  Serial.println("Activating Light Two");
//  digitalWrite(PIN_EXTERNAL_TRIGGER2, HIGH);
//  // digitalWrite(PIN_EXTERNAL_TRIGGER2, HIGH);
//  digitalWrite(PIN_TRIGGER_PULSE2, HIGH);
//  delay(duration);
//  //  digitalWrite(PIN_EXTERNAL_TRIGGER2, LOW);
//  digitalWrite(PIN_EXTERNAL_TRIGGER2, LOW);
//  digitalWrite(PIN_TRIGGER_PULSE2, LOW);
//  //delay(100);
//  delay(2000-duration);
//  digitalWrite(PIN_GLOBAL_SYNC, LOW);
}

void pulse(int duration){

  // digitalWrite(PIN_TRIGGER_PULSE, HIGH);

  while (millis() - pulse_start < duration) {
    digitalWrite(PIN_TRIGGER_PULSE, HIGH);
  }
  digitalWrite(PIN_TRIGGER_PULSE, LOW);
  pulse_start = millis();
}

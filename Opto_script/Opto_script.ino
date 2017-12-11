//Sunmee Park 2/17/2016
//AKHIL Bandi
//Tom Vajtay revisions 6/2017, 12/2017

#define CAMERA_PIN             2        // starts board 1, and sends out the signal to the board 2

#define PIN_TRIGGER_PULSE      11        // TTL for Light 1
#define PIN_TRIGGER_PULSE2     10        // TTL for Light 2

//Definitions for Switch/Case
#define SINGLEPULSE            0
#define FREQUENCY              1
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
bool onSignal = false;
bool onPulse = false;

int single_pulse_count = 0;

int pulse_count = 0;
int t_duration = 5; // longer than 10msec
int freqArray[6] = {1, 3, 7, 10, 15, 20}; //Array containing the frequency values for FREQUENCY trials in Hz
int stimfreqarray[2] = {10, 100};          //Array containing the frequency values for Stim-Frequency trials in Hz
int pulseArray[8] = {1, 3, 5, 10, 20, 50, 100, 1000}; //Array containing the different single stimuli durations
int freq;                            //In C/C++/Arduino array indices begin with position 0
int dur_count = 0;
int signalChange = 0;
int freq_count = 0;
int loop_count = 0;
int freq_flag = 0;
int ISI = 1000;    //Inter stimuli interval used in Stim-Frequency trial, this is time interval between the initial stimuli and the frequency stimuli
int ITI = 10000;  //Inter Trial Interval, time between recording of trials in ms
          /*-------------------------
            functions
            -------------------------*/

void setup() {

  pinMode(CAMERA_PIN, OUTPUT);
  pinMode(PIN_TRIGGER_PULSE, OUTPUT);
  pinMode(PIN_TRIGGER_PULSE2, OUTPUT);

  digitalWrite(CAMERA_PIN, LOW);
  digitalWrite(PIN_TRIGGER_PULSE, LOW);
  digitalWrite(PIN_TRIGGER_PULSE2, LOW);

  Serial.begin(9600);

  Serial.println("Enter s to start duration trials");
  Serial.println("Enter t to start frequency trials");
  Serial.println("Enter b to start stim-frequency trials");
  Serial.println("Enter i to start 15 x 50msec pulses for intensity stimulation");
  Serial.println("Enter o to start 8 x 50msec pulses for intensity stimulation");
  Serial.println("You can enter q(quit) or p(pause) during recording");

  onSignal = false;

}


void loop() {

  while (Serial.available()) {
    control_char = Serial.read();


    if (control_char == 's') {
      onSignal = true;
      Serial.println("Single pulse mode");
      timestamp = millis();
      signalChange = 0;
      count = 0;
    }
    else if ( control_char == 't') {
      onSignal = true;
      Serial.println("Frequency pulse mode");
      timestamp = millis();
      loop_count = 0;
      signalChange = 1;
      count = 0;
    }
    else if (control_char == 'b') {
      onSignal = true;
      Serial.println("Stim-Frequency Trial");
      timestamp = millis();
      signalChange = 4;
      count = 0;
    }
    else if (control_char == 'i') {
      Serial.println("15 x 50msec pulses for intensity mode");
      onSignal = true;
      timestamp = millis();
      signalChange = 2;
      count = 0;
    }
    else if (control_char == 'o') {
      Serial.println("8 x 50msec pulse for intensity mode");
      onSignal = true;
      timestamp = millis();
      signalChange = 3;
      count = 0;
    }
    else if (control_char == 'q') {
      digitalWrite(CAMERA_PIN, LOW);
      onSignal = false;
      count = 0;
      delay(500);
      asm volatile ("  jmp 0");   // Makes the Arduino soft reset
    }
    else {
      Serial.println("Enter either s, t, b, i, or o");
    }
  }

  while (onSignal) {   //During the experiment this part loops for every unique trial allowing for pause/quit in the middle of a run
    e_stop();

    switch (signalChange) {
      case SINGLEPULSE:  //for single pulse with duration change
        while (count < 8) {
          if (single_pulse_count < 8) {
            if (single_pulse_count == 0) {
              Serial.print("Trial #:");
              Serial.print(count + 1);
              Serial.print("\t");
              Serial.print((millis() - timestamp)/1000);
              Serial.println(" seconds since start");
            }
            Serial.print(pulseArray[single_pulse_count]);
            Serial.println(" msec duration");

            Serial.print("Activating Light One ");
            Serial.println((millis() - timestamp)/1000);
            pulse(pulseArray[single_pulse_count]);
            iti_delay(ITI);

            Serial.print("Activating Light Two ");
            Serial.println((millis() - timestamp)/1000);
            Serial.println();
            pulse2(pulseArray[single_pulse_count]);
            iti_delay(ITI);

            single_pulse_count ++;
          }
          else if (single_pulse_count == 8) {
            count++;
            single_pulse_count = 0;
          }
        }
        onSignal = 0;
        count = 0;
        Serial.println("Duration Trial complete");
        Serial.print((millis() - timestamp)/1000);
        Serial.println(" seconds since start");
        Serial.println();
        break;

      case FREQUENCY:
        while (count < 6) {
          if (loop_count < 8) {
            if (loop_count == 0) {

              freq = freqArray[count];
              Serial.print("Frequency:");
              Serial.print(freq);
              Serial.print("Hz");
              Serial.print("\t");
              Serial.print((millis() - timestamp)/1000);
              Serial.println(" seconds since start");
            }
            Serial.println(loop_count + 1);
            digitalWrite(CAMERA_PIN, HIGH);
            delay(1000);
            while (freq_count < freq) {     //Loop for first light
              digitalWrite(PIN_TRIGGER_PULSE, HIGH);
              //digitalWrite(PIN_TRIGGER_PULSE2, HIGH);
              delay(t_duration);
              digitalWrite(PIN_TRIGGER_PULSE, LOW);
              //digitalWrite(PIN_TRIGGER_PULSE2, LOW);
              delay((1000 / freq) - t_duration);
              freq_count++;
            }
            freq_count = 0;
            delay(1000);
            digitalWrite(CAMERA_PIN, LOW);

            iti_delay(ITI);

            Serial.println(loop_count + 1);
            digitalWrite(CAMERA_PIN, HIGH);
            delay(1000);
            while (freq_count < freq) {       //Loop for second light
              //digitalWrite(PIN_TRIGGER_PULSE, HIGH);
              digitalWrite(PIN_TRIGGER_PULSE2, HIGH);
              delay(t_duration);
              //digitalWrite(PIN_TRIGGER_PULSE, LOW);
              digitalWrite(PIN_TRIGGER_PULSE2, LOW);
              delay((1000 / freq) - t_duration);
              freq_count++;
            }
            freq_count = 0;
            delay(1000);
            digitalWrite(CAMERA_PIN, LOW);
            loop_count++;
            iti_delay(ITI);
          }
          else {
            count++;
            loop_count = 0;
            digitalWrite(CAMERA_PIN, LOW);
            digitalWrite(PIN_TRIGGER_PULSE, LOW);
            digitalWrite(PIN_TRIGGER_PULSE2, LOW);
          }
        }
        onSignal = 0;
        count = 0;
        Serial.println("Frequency Trial complete");
        Serial.print((millis() - timestamp)/1000);
        Serial.println(" seconds since start");
        Serial.println();
        break;

      case INTENSITYONE:
        while (count < 15) {
          pulse(50);
          Serial.print("Light 1 ")
          Serial.println(count + 1);
          iti_delay(ITI);

          pulse2(50);
          Serial.print("Light 2 ")
          Serial.println(count + 1);
          iti_delay(ITI);
          count++;
        }
        onSignal = 0;
        count = 0;
        Serial.println("15 Trials complete");
        Serial.print((millis() - timestamp)/1000);
        Serial.println(" seconds since start");
        Serial.println();

        break;

      case INTENSITYTWO:
        while (count < 8) {
          pulse(50);
          Serial.print("Light 1 ")
          Serial.println(count + 1);
          iti_delay(ITI);

          pulse2(50);
          Serial.print("Light 2 ")
          Serial.println(count + 1);
          iti_delay(ITI);
          count++;
        }
        onSignal = 0;
        count = 0;
        Serial.println("8 Trials complete");
        Serial.print((millis() - timestamp)/1000);
        Serial.println(" seconds since start");
        Serial.println();

        break;
    }//end of case
    delay(500);
    asm volatile ("  jmp 0");   // Makes the Arduino soft reset at the end of experiment
  }//end of while
}//end of loop()

void pulse(int duration) {
  double delay_start = millis();
  while (millis() - delay_start < 3000) {
    digitalWrite(CAMERA_PIN, HIGH);
    e_stop();
    if (millis() - delay_start > 1000 && millis() - delay_start < duration + 1000) {
      digitalWrite(PIN_TRIGGER_PULSE, HIGH);
    }
    else if (millis() - delay_start > duration + 1000) {
      digitalWrite(PIN_TRIGGER_PULSE, LOW);
    }
  }
  digitalWrite(CAMERA_PIN, LOW);
}

void pulse2(int duration) {
  double delay_start = millis();
  while (millis() - delay_start < 3000) {
    digitalWrite(CAMERA_PIN, HIGH);
    e_stop();
    if (millis() - delay_start > 1000 && millis() - delay_start < duration + 1000) {
      digitalWrite(PIN_TRIGGER_PULSE2, HIGH);
    }
    else if (millis() - delay_start > duration + 1000) {
      digitalWrite(PIN_TRIGGER_PULSE2, LOW);
    }
  }
  digitalWrite(CAMERA_PIN, LOW);
}

void iti_delay(int duration) {
  double ITI_start = millis();
  while (millis() - ITI_start < duration ){
    e_stop();
  }
}

void e_stop() {
  if (Serial.available()) {
    control_char = Serial.read();
    if (control_char == 'q') {
      digitalWrite(CAMERA_PIN, LOW);
      digitalWrite(PIN_TRIGGER_PULSE, LOW);
      digitalWrite(PIN_TRIGGER_PULSE2, LOW);
      onSignal = false;
      Serial.println("Trigger function stopped");
      count = 0;
      delay(500);
      asm volatile ("  jmp 0");   // Makes the Arduino soft reset
    }
    else if (control_char == 'p') {
      digitalWrite(CAMERA_PIN, LOW);
      digitalWrite(PIN_TRIGGER_PULSE, LOW);
      digitalWrite(PIN_TRIGGER_PULSE2, LOW);
      double pause_start = millis();
      Serial.println("Trigger function paused, enter 'r' to resume");
      while (Serial.available()) {
        control_char = Serial.read();
        if (control_char == 'r') {
          Serial.print("Paused for ");
          Serial.print((pause_start - millis())/1000);
          Serial.println(" seconds");
          break;
        }
        else {
          Serial.println("That wasn't 'r' ");
        }
      } // while for resume
    } //control_char if, else if
  } // If Serial.available
} //Function end

// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
// 
// Part 2-6: Feedback (Echo) Delay

///////////////////////////////////
// copy the Design Tool code here
///////////////////////////////////

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=105.70001220703125,617.2000255584717
AudioEffectChorus        chorus2; //xy=856.3999633789062,586.7999877929688
AudioMixer4              mixer3;         //xy=864.7713928222656,206.12857818603516
AudioMixer4              mixer4; //xy=863.6999359130859,691.2000007629395
AudioEffectDelay         delay2; //xy=863.1999397277832,914.1999435424805
AudioEffectDelay         delay1;         //xy=866.5570983886719,375
AudioEffectChorus        chorus1;        //xy=872.3999824523926,94.7999906539917
AudioEffectWaveshaper    waveshape2; //xy=882.9857559204102,1061.4858093261719
AudioEffectWaveshaper    waveshape1;     //xy=1355.6288299560547,494.3428535461426
AudioMixer4              mixer2;         //xy=1727.7001304626465,904.9142761230469
AudioMixer4              mixer1;         //xy=1740.2715454101562,284.4142951965332
AudioOutputI2S           i2s2;           //xy=1938.842960357666,534.4857711791992
AudioConnection          patchCord1(i2s1, 0, waveshape1, 0);
AudioConnection          patchCord2(i2s1, 0, mixer1, 0);
AudioConnection          patchCord3(i2s1, 0, mixer3, 0);
AudioConnection          patchCord4(i2s1, 0, chorus1, 0);
AudioConnection          patchCord5(i2s1, 1, waveshape2, 0);
AudioConnection          patchCord6(i2s1, 1, mixer2, 0);
AudioConnection          patchCord7(i2s1, 1, mixer4, 0);
AudioConnection          patchCord8(i2s1, 1, chorus2, 0);
AudioConnection          patchCord9(chorus2, 0, mixer2, 1);
AudioConnection          patchCord10(mixer3, delay1);
AudioConnection          patchCord11(mixer4, delay2);
AudioConnection          patchCord12(delay2, 0, mixer2, 2);
AudioConnection          patchCord13(delay2, 0, mixer4, 1);
AudioConnection          patchCord14(delay1, 0, mixer1, 2);
AudioConnection          patchCord15(delay1, 0, mixer3, 1);
AudioConnection          patchCord16(chorus1, 0, mixer1, 1);
AudioConnection          patchCord17(waveshape2, 0, mixer2, 3);
AudioConnection          patchCord18(waveshape1, 0, mixer1, 3);
AudioConnection          patchCord19(mixer2, 0, i2s2, 1);
AudioConnection          patchCord20(mixer1, 0, i2s2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=573.4858093261719,1317.0571613311768
// GUItool: end automatically generated code



#define CHORUS_DELAY_LENGTH (16*AUDIO_BLOCK_SAMPLES)
short delayline[CHORUS_DELAY_LENGTH];
int n_chorus = 2;

#include <Bounce.h>

Bounce button0 = Bounce(0, 5);

void setup() {
  Serial.begin(9600);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  //pinMode(A2, INPUT);
  //pinMode(A3, INPUT);
  AudioMemory(160);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.55);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(36);
  mixer1.gain(0, 1);
  mixer2.gain(0, 1);
  mixer3.gain(0,0.5);
  mixer4.gain(0,0.5);
  mixer3.gain(1,0.5);
  mixer4.gain(1,0.5);
  chorus1.begin(delayline, CHORUS_DELAY_LENGTH, n_chorus);
  chorus2.begin(delayline, CHORUS_DELAY_LENGTH, n_chorus);
  delay1.delay(0, 1200);
  delay(1000);
}

void loop() {

  int knob = analogRead(A3);
  float effect = (float)knob / 1050.0;

  int knob2 = analogRead(A2);
  float dry = (float)knob2 / 1050;

  int state = 2*digitalRead(1)+digitalRead(0);

  switch (state){
    case 0:
      //No effect
      mixer1.gain(0, 1);
      mixer2.gain(0, 1);
      mixer1.gain(1, 0);
      mixer2.gain(1, 0);
      mixer1.gain(2, 0);
      mixer2.gain(2, 0);
      mixer1.gain(3, 0);
      mixer2.gain(3, 0);
      break;
    case 1:
      //Chorus
      mixer1.gain(0, dry);
      mixer2.gain(0, dry);
      mixer1.gain(1, 1-dry);
      mixer2.gain(1, 1-dry);
      mixer1.gain(2, 0);
      mixer2.gain(2, 0);
      mixer1.gain(3, 0);
      mixer2.gain(3, 0);

      n_chorus = (int)(effect * 10);
      chorus1.voices(n_chorus);
      break;
    case 2:
      //Delay
      mixer1.gain(0, dry);
      mixer2.gain(0, dry);
      mixer1.gain(1, 0);
      mixer2.gain(1, 0);
      mixer1.gain(2, 1-dry);
      mixer2.gain(2, 1-dry);
      mixer1.gain(3, 0);
      mixer2.gain(3, 0);

      mixer3.gain(1, effect);
      mixer4.gain(1, effect);
      break;
    case 3:
      //Distortion
      mixer1.gain(0, dry);
      mixer2.gain(0, dry);
      mixer1.gain(1, 0);
      mixer2.gain(1, 0);
      mixer1.gain(2, 0);
      mixer2.gain(2, 0);
      mixer1.gain(3, 1-dry);
      mixer2.gain(3, 1-dry);
      break;
  }
}

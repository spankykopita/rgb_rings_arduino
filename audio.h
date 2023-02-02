// Analog
#define MICROPHONE_PIN A2

#define SAMPLES_COUNT 300
#define SAMPLING_FREQUENCY 10000 //Hz, must be less than 10000 due to ADC

// 10000 hertz of sampling is 100 microseconds per sample
// 300 samples at 100 microseconds apiece => sampling over 30 milliseconds
unsigned int samplingPeriodMicro = round(1000000*(1.0/SAMPLING_FREQUENCY));

unsigned int smoothedAmplitude = 100;
float minAmplitude = 1024;
float maxAmplitude = 0;

void recordAmplitude() {
  // Correct
  // int micSample = analogRead(MICROPHONE_PIN); // Same as analogRead(2)
  // Serial.print("Sample1:");
  // Serial.println(micSample);

  // Flatline? This is almost definitely the LED A0 pin
  // int micSample2 = analogRead(0);
  // Serial.print("Sample2:");
  // Serial.println(micSample2);

  // Reacts to A1 touch
  // int micSample3 = analogRead(1);
  // Serial.print("Sample3:");
  // Serial.println(micSample3);

  int minSample = 1024;
  int maxSample = 0;
  for (int i = 0; i < SAMPLES_COUNT; i++) {
    unsigned long microseconds = micros();
    int sample = analogRead(MICROPHONE_PIN);
    maxSample = max(maxSample, sample);
    minSample = min(minSample, sample);
    while (micros() < (microseconds + samplingPeriodMicro)) { }
  }
  Serial.print("Amplitude:");
  Serial.println(maxSample - minSample);

  smoothedAmplitude = smoothedAmplitude * 0.4 + (maxSample - minSample) * 0.6;
  Serial.print("SmoothedAmplitude:");
  Serial.println(smoothedAmplitude);

  minAmplitude = min(minAmplitude, smoothedAmplitude) * 1.01;
  maxAmplitude = max(maxAmplitude, smoothedAmplitude) * 0.99;
  Serial.print("MinAmplitude:");
  Serial.println(minAmplitude);
  Serial.print("MaxAmplitude:");
  Serial.println(maxAmplitude);
}
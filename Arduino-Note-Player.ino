#include "FreqGen.hpp"
#include "music.hpp"


// Audio note player example program

FreqGen fg;
unsigned long run_time_ms = 0;

void setup() {
  // Plays a midi file through pin A1, modify by changing the pinMode and the #defines in FreqGen.cpp and its header file.
  fg.Init();

  // audio array, length of the array, starting at index.
  fg.PlayMidi(midi_array, midi_data_length, 0);

  // To play another file, just call StopMidi() then PlayMidi for another array.
  Serial.begin(9600);
}

void loop() {
  // For pausing/stopping
  if (run_time_ms == 10000) {
    fg.StopMidi();
    Serial.println("stopped");
  }

  // For resuming
  else if (run_time_ms == 20000) {
    fg.ResumeMidi();
    Serial.println("resumed");
  }
  
  // Read the audio file
  fg.UpdateMidi();
  run_time_ms++;
  delay(1);
}

ISR(TIMER2_COMPA_vect) { // This gets called 80 000 times every second
  // Update the state machine for the frequency generator
  fg.Update();
}

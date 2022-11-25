#include "FreqGen.hpp"
#include "music.hpp"


FreqGen fg;

void setup() {
  // Plays a midi file through pin A1, modify by changing the pinMode and the #defines in FreqGen.cpp and its header file.
  fg.Init();
  // audio array, length of the array, starting at index.
  fg.PlayMidi(midi_array, midi_data_length, 0);
}

void loop() {
  // Read the audio file
  fg.UpdateMidi(millis());
}

ISR(TIMER2_COMPA_vect) {
  // Update the state machine for the frequency generator
  fg.Update();
}

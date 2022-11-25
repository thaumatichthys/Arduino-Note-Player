#include "Arduino.h"


#define OUTPUT_LOW    (PORTC &= 0b11111101)
#define OUTPUT_HIGH   (PORTC |= 0b00000010)

class FreqGen {
  static const uint16_t bit_length = 650 * 8;
  const uint32_t update_freq = 80000;
  
  volatile uint16_t bit_index = 0;
  volatile uint8_t buffer_ptr[bit_length / 8];
  volatile bool enabled = false;

  uint32_t *midi_src;
  uint32_t midi_time_offset = 0;
  uint32_t midi_src_len; // length as in number of elements, not bytes
  uint32_t midi_read_index = 0;
  uint32_t midi_pause_time = 0;
  bool playing = false;
public:
  void Init();
  void Update();
  void ChangeFreq(uint16_t freq, bool add);
  void SetMidiNote(uint8_t note, bool on);
  void PlayMidi(uint32_t *src, uint32_t src_len, uint32_t index);
  void UpdateMidi();
  void ResumeMidi();
  void StopMidi();
};

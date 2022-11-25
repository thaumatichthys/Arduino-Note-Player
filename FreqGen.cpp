#include "FreqGen.hpp"


void FreqGen::Init() {
  pinMode(A1, OUTPUT);
  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;
  OCR2A = 24;
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS21);   
  TIMSK2 |= (1 << OCIE2A);
  sei();
  for (int i = 0; i < this->bit_length / 8; i++) {
    this->buffer_ptr[i] = 0;
  }
}

void FreqGen::PlayMidi(uint32_t *src, uint32_t src_len, uint32_t index) {
  this->midi_read_index = index;
  this->midi_src = src;
  this->midi_src_len = src_len;
  this->midi_prev_ms = 0;
  this->playing = true;
}

void FreqGen::StopMidi() {
  this->enabled = false;
  this->playing = false;
}

void FreqGen::ResumeMidi() {
  this->enabled = true;
  this->playing = true;
}

void FreqGen::UpdateMidi(uint32_t milliseconds) {
  if ((!this->playing) || (this->midi_read_index > this->midi_src_len))
    return;
  uint32_t note_data = pgm_read_dword(&this->midi_src[this->midi_read_index]);
  uint32_t note_time = note_data & 0x00FFFFFF;
  uint8_t note = (note_data >> 24) & 0x7F;
  bool note_action = note_data & 0x80000000;
  if ((milliseconds >= note_time)) {
    this->SetMidiNote(note, note_action);
    this->midi_read_index++;
  }
}

void FreqGen::SetMidiNote(uint8_t note, bool on) {
  uint16_t freq = (uint16_t) (440 * pow(2, (note - 69) / 12.0f));
  this->ChangeFreq(freq, on);
}

void FreqGen::ChangeFreq(uint16_t freq, bool add) {
  const uint8_t on_samples = 5;
  this->enabled = false;
  cli();
  uint16_t period = (this->update_freq / freq);
  for (uint16_t i = 0; i < this->bit_length; i++) {
    if ((i % period) < 5) {
      if (add)
        this->buffer_ptr[i / 8] |= (uint8_t) 1 << (i % 8); // the bit order is backwards but since it is also read backwards it doesnt actually matter
      else // remove the frequency instead
        this->buffer_ptr[i / 8] &= ~((uint8_t) 1 << (i % 8));
    }
    else 
      i += period - on_samples; // this is an important optimization; it skips past the off sections.
  }
  sei();
  this->enabled = true;
}

void FreqGen::Update() {
  if (!this->enabled)
    return;
  uint16_t bit_index_copy = this->bit_index;
  uint8_t within_byte = bit_index_copy % 8;
  uint16_t byte_index = (bit_index_copy - within_byte) / 8;
  bool state = this->buffer_ptr[byte_index] & ((uint8_t) 1 << within_byte);
  if (state)
    OUTPUT_HIGH;
  else
    OUTPUT_LOW;
  this->bit_index++;
  if (this->bit_index >= this->bit_length)
    this->bit_index = 0;
}

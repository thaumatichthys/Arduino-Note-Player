import mido
import numpy as np
import os.path


# embarrassingly bad python code

# Dependencies: numpy, mido

# Output format: an array of 32 bit integers.
# MSB: determines whether the note is to turn on or off
# 7 bits right of MSB: the MIDI note
# The rest of the bits: a 24 bit time code, relative to the start of the track, in milliseconds

# Usage: edit the config section, run the program, and paste the output into your header file

# --------- CONFIG START --------- #
midi_input_path = 'C:/example/midi/file.mid'
text_output_path = 'C:/example/output/text.txt'

# the arduino has really limited flash, so this option lets you cut off the rest of the track
max_output_size_bytes = 30720
# ---------- CONFIG END ---------- #

mid = mido.MidiFile(midi_input_path)

output_array = []
time_ac = 0  # time accumulation
length = 0

# get the midi data
for msg in mid:
    time_ac += msg.time
    if msg.type == 'note_on' or msg.type == 'note_off':
        value = int(round(time_ac * 1000))
        type = 1  # on
        if msg.type == 'note_off' or (msg.type == 'note_on' and msg.velocity == 0):
            type = 0
        value &= 0x00FFFFFF
        value |= (type << 31)       # add the command
        value |= (msg.note << 24)   # add the note number
        output_array.append(value)
        length += 1
        if length >= (max_output_size_bytes / 4):
            break

# write the midi data into the file
output_str = ""
output_str += "#include \"Arduino.h\"\n\n"
output_str += "// Written by hand in " + str(np.random.randint(1, 100)) + "days, https://github.com" \
                                                                          "/thaumatichthys\n\nconst uint32_t " \
                                                                          "midi_array[] PROGMEM= { "
output_str += " // '" + os.path.basename(midi_input_path) + "'"
for i in range(length - 1):
    if not i % 99:
        output_str += "\n   "
    output_str += str(output_array[i]) + ", "
output_str += str(output_array[length - 1]) + "\n};"
output_str += "\n\nconst uint32_t midi_data_length = " + str(length) + ";\n"

with open(text_output_path, 'w') as f:
    f.write(output_str)

output_str += "\n// Data size: " + str(int(length * 4 / 100) / 10) + "kb (approx)\n"
print(output_str)

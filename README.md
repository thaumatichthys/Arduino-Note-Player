# Arduino-Note-Player
Some basic code to convert and play a MIDI file on ATMega328P based Arduino boards

An arbitrary number of notes can be played simultaneously, and from my testing, it uses approximately 40% of the CPU's runtime to generate the frequencies.

It works by creating an output buffer, and when a note is to be played, it 'OR's in the new frequency. From the timer ISR, it clocks out the buffer one bit at a time. This allows it to play an arbitrary number of different frequencies.

## Usage
Open the python program in some sort of IDE, install the dependencies, and edit the config section.

If your MIDI file is more than around 30kb minus the rest of your Arduino code, it won't fit on the Arduino and you will need to reduce the number of notes by either just cutting off the rest of the file, or making it less detailed.

After running the python program, paste the contents of the output into the header file.

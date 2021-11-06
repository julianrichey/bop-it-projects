# Frets On Fire

System
Sender uses an MSP430FR5994 dev board, which sends a single byte to the receiver via a CC1101+. This is the one doing the energy harvesting.
Receiver uses an MSP-EXP430FR5969 dev board (just what was on hand at the time) along with another CC1101+.
Receiver connects to an Arduino Leonardo which acts as a keyboard (no low power requirements, easy/reliable).
Receiver sends interrupts (polled in reality, not enough interrupts on the Leonardo) via level shifters, though these shouldn't actually needed (3.6V still 'high' to the 5V Leonardo).

Use
Press and release of four physical buttons (frets) are mapped to the press and release of keys 1, 2, 3, and 4.
Each button press must generate sufficient energy to power the mcu and radio for a one-byte transmission; this can be achieved by pressing and releasing the buttons with sufficient speed.
Subsequent actions (press of any button, release of any button) must be made after the system has consumed all available energy and shut off. This varies with energy harvested; let's call it ~0ms to ~200ms. Energy is consumed after transmission by spinning and turning on an led (more could be done here: brighter led? more energy intense spinning?). As a consequence, multiple keys cannot be pressed simultaneously (i.e. chords cannot be played) and care must be taken to not release a button too quickly after it was pressed. The Leonardo releases before pressing the corresponding key again, so this just results in a temporary graphical error.
A strum happens automatically after each of these keys is pressed, this is mapped to key 0.
So, pressing and the keys in order results in the sequence 10203040 (regardless of whether any are released).

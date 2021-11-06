#include <Keyboard.h>

// On Arduino Leonardo, can use interrupts with only digital pins 0, 1, 2, 3, 7
// So, fake it by polling for digital pin posedges
bool button_states[8];
bool sample;

void setup()
{
  pinMode(13, OUTPUT);            //LED
  for (int i = 0; i < 8; i++)
  {
    button_states[i] = false;
    pinMode(i+2, INPUT);          //'interrupt' lines
  }
  Keyboard.begin();
  delay(500);                     //prints dont work for a bit - somewhere between 350 and 400 ms
}

void loop()
{
  for (int i = 0; i < 8; i++)
  {
    sample = digitalRead(i+2);
    if (sample && !button_states[i])
    {
      digitalWrite(13, !digitalRead(13));
      if (i < 4)
      {
        Keyboard.release(i+49);   //Just in case the release was not sent (e.g. release too soon after press)
        Keyboard.press(i+49);     //INT {0, 1, 2, 3} -> ASCII {1, 2, 3, 4}
        Keyboard.print('0');      //Strum. No delay needed between release+press+print?
      }
      else
        Keyboard.release(i+45);   //INT {4, 5, 6, 7} -> ASCII {1, 2, 3, 4}
    }
    button_states[i] = sample;
  }
}

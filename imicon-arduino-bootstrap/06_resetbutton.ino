#include <ezButton.h>

ezButton button(4); // create ezButton object that attach to pin 4;
int btnState;

void resetbuttonLoop(){

}
void resetbuttonSetup(){
  btnState = 1;
  button.setDebounceTime(50); // set debounce time to 50 milliseconds
  button.loop();              // MUST call the loop() function first
  btnState = button.getState();
  Serial.println(btnState);

  
}
bool resetbuttonispress(){
  if (btnState == 1) // SET
  {
   return true;
  }
  else // RUN MODE
  {
    return false;
   
  }
}


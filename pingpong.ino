#include <LedControl.h>

LedControl lc=LedControl(12,10,11,1);

byte left[8] = {0b00000000, 
                0b00000000, 
                0b00000000, 
                0b00100000, 
                0b01000000, 
                0b10000000, 
                0b01000000, 
                0b00100000};

byte right[8] = {0b00000000, 
                 0b00000000, 
                 0b00000000, 
                 0b00000100, 
                 0b00000010, 
                 0b00000001, 
                 0b00000010, 
                 0b00000100};

#define fupb 7
#define fdownb 6
#define supb 5
#define sdownb 4
#define LED_PIN     13 

boolean buttonWasUp1u = true; 
boolean buttonWasUp1d = true; 
boolean buttonWasUp2u = true; 
boolean buttonWasUp2d = true;

int iters = 0;

void setup() {
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,15);
  /* and clear the display */
  lc.clearDisplay(0);
}

int pad1[3] = {2,3,4};
int pad2[3] = {3,4,5};

int xball = 3;
int yball = 3;
int xstate = 1;
int ystate = 2;

void loop() {
  setPads();
  bool  buttonIsUp1u = digitalRead(fupb);
  bool  buttonIsUp1d = digitalRead(fdownb);
  bool  buttonIsUp2u = digitalRead(supb);
  bool  buttonIsUp2d = digitalRead(sdownb);
  if (buttonWasUp1u && !buttonIsUp1u) {
    delay(10);
    buttonIsUp1u = digitalRead(fupb);
    if (!buttonIsUp1u) {
      if(!(pad1[2] == 7)){
        for(int i = 0; i<3; i++) {
          pad1[i]++;
        }
        lc.clearDisplay(0); 
      } 
    }
  } 
  if (buttonWasUp1d && !buttonIsUp1d) {
    delay(10);
    buttonIsUp1d = digitalRead(fdownb);
    if (!buttonIsUp1d) { 
      if(!(pad1[0] == 0)){ 
        for(int i = 0; i<3; i++) {
          pad1[i]--;
        } 
        lc.clearDisplay(0);
      } 
    }
  } 
  if (buttonWasUp2u && !buttonIsUp2u) {
    delay(10);
    buttonIsUp2u = digitalRead(supb);
    if (!buttonIsUp2u) { 
      if(!(pad2[2] == 7)){ 
        for(int i = 0; i<3; i++) {
          pad2[i]++;
        }
        lc.clearDisplay(0);
      }   
    }
  } 
  if (buttonWasUp2d && !buttonIsUp2d) {
   delay(10);
    buttonIsUp2d = digitalRead(sdownb);
    if (!buttonIsUp2d) { 
      if(!(pad2[0] == 0)){
        for(int i = 0; i<3; i++) {
          pad2[i]--;
        }  
        lc.clearDisplay(0);
      }
    }
  }
  buttonWasUp1u = buttonIsUp1u; 
  buttonWasUp1d = buttonIsUp1d; 
  buttonWasUp2u = buttonIsUp2u; 
  buttonWasUp2d = buttonIsUp2d; 
  setPads();
  setBall();
  if(iters > 40) {
    if (xball == 1){
      if (!((yball==pad1[0])or(yball==pad1[1])or(yball==pad1[2])or((yball==(pad1[0]-1))&&((ystate == 0)or((ystate == 1)&&(xstate == 1))))or((yball==(pad1[2]+1))&&((ystate == 1)or((ystate == 0)&&(xstate == 1)))))){
        moveBall(xstate, ystate);
        setBall();
        delay(300);
        printWinRight();
      } 
    } else if (xball == 6) {
      if (!((yball==pad2[0])or(yball==pad2[1])or(yball==pad2[2])or((yball==(pad2[0]-1))&&((ystate == 0)or((ystate == 1)&&(xstate == 0)))or((yball==(pad2[2]+1))&&((ystate == 1)or((ystate == 0)&&(xstate == 0))))))){
        moveBall(xstate, ystate);
        setBall();
        delay(300);
        printWinLeft();
      }   
    }
    if((yball==(pad1[0]-1))&&(xball == 1)){
      xstate = 1; 
      ystate = 1;
    } else if ((yball==(pad1[2]+1))&&(xball == 1)) {
      xstate = 1;
      ystate = 0;
    } else if ((yball==(pad2[0]-1))&&(xball == 6)) {
      xstate = 0;
      ystate = 1;
    } else if ((yball==(pad2[2]+1))&&(xball == 6)) {
      xstate = 0;
      ystate = 0;
    } else {
      checkBounce();
    }
    moveBall(xstate, ystate);
    iters = 0;
  }
  iters++;
}

void setPads() {
  for(int i = 0; i<3; i++){
    lc.setLed(0,0, pad1[i], true);  
  }
  for(int i = 0; i<3; i++){
    lc.setLed(0,7, pad2[i], true);  
  }
}

void setBall() {
  lc.clearDisplay(0);
  lc.setLed(0, xball, yball, true);
}

void moveBall(int dirX, int dirY) {
  if (dirX == 0) {
    if (dirY == 0) {
      xball--;
      yball++;
    }  else if (dirY == 1) {
      xball--;
      yball--;  
    } else if (dirY == 2) {
      xball--;  
    }
  } else { 
  if (dirY == 0) {
      xball++;
      yball++;
    }  else if (dirY == 1) {
      xball++;
      yball--;  
    } else if (dirY == 2) {
      xball++;  
    }  
  }
}
//dirX: 0 - left, 1 - right; dirY: 0 - up, 1 - down, 2 - straight
void checkBounce() {
  if (xball == 1) {
    if (yball == 0) {
      xstate = 1;
      ystate = 0;
    } else if (yball == 7) {
      xstate = 1;
      ystate = 1;
    } else {
      xstate = 1;
      if (yball == pad1[0]){
        if(ystate == 0) {
          ystate = 2;
        } else {
          ystate = 1;
        }
      } else if (yball == pad1[2]) {
        if (ystate == 1) {
          ystate = 2;    
        } else if (ystate == 2) {
          ystate = 0;
        } else {
          ystate = 0;
        }
      } else if (yball == pad2[0]) {
        if (ystate == 0) {
          ystate = 2;
        } else {
          ystate = 1;  
        }
      } else if (yball == pad2[2]) {
        if (ystate == 1) {
          ystate = 2;    
        } else if (ystate == 2) {
          ystate = 0;
        } else {
          ystate = 0;
        }  
      }
    }
  } else if (xball == 6) {
    if (yball == 0) {
      xstate = 0;
      ystate = 0;
    } else if (yball == 7) {
      xstate = 0;
      ystate = 1;
    } else {
      xstate = 0;
      if (yball == pad2[0]){
        if(ystate == 0) {
          ystate == 2;
        } else {
          ystate == 1;
        } 
      } else if (yball == pad2[2]) {
        if (ystate == 1) {
          ystate = 2;    
        } else {
          ystate = 0;
        }
      }
    }  
  } else if (yball == 0) {
    ystate = 0;
  } else if (yball == 7) {
    ystate = 1;
  }
}

void printWinLeft() {
  for (int i = 0; i < 8; i++) {
    lc.setColumn(0, i, left[i]); 
  }
  while (true) {;}
}

void printWinRight() {
  for (int i = 0; i < 8; i++) {
    lc.setColumn(0, i, right[i]); 
  }
  while (true) {;}
}


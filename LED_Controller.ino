#define IR_SMALLD_NEC
#include <IRsmallDecoder.h>

// Initialize IR decoder on digital pin 2
IRsmallDecoder irDecoder(2);
irSmallD_t irData;

// Define RGB LED pins
const uint8_t ROT_PIN = 1;
const uint8_t BLAU_PIN = 4;
const uint8_t GRUEN_PIN = 0;

// Variable to store the last IR command
byte last_command = 0x00;

// Function to find the maximum value among three numbers
uint8_t max_y(uint8_t a, uint8_t b, uint8_t c) {
  uint8_t maxVal = a;
  if (b > maxVal) {
    maxVal = b;
  }
  if (c > maxVal) {
    maxVal = c;
  }
  return maxVal;
};

// Structure to represent an RGB color and associated functions
struct RGB {
  uint8_t red;
  uint8_t blue;
  uint8_t green;
  bool first_time_flag = true;
  bool fade_indicator = false;
  uint32_t start_time;
  uint8_t fade_step_counter;

  // Function to set RGB color and save the changed colors in the given variables
  void set(bool reset, uint8_t r, uint8_t g, uint8_t b) {
    first_time_flag = reset;
    red = r;
    green = g;
    blue = b;
    analogWrite(ROT_PIN, red);
    analogWrite(GRUEN_PIN, green);
    analogWrite(BLAU_PIN, blue);
  }

//Setting the colors without saving the change
  void analogWriteRGB(int8_t r, uint8_t g, uint8_t b){
      analogWrite(ROT_PIN, r);
      analogWrite(GRUEN_PIN, g);
      analogWrite(BLAU_PIN, b);
    }

//Function to fade colorfulle fading the LEDs 
  void fade(uint16_t delay_time) {
  if (first_time_flag) {
    first_time_flag = false;
    start_time = millis();
    fade_indicator = false;
    red = 255;
    blue = 0;
    green = 0;
    delay_time=0;
  }
  
  uint64_t current_time = millis();
  if (current_time - start_time >= delay_time) {
    if (green == 0 && red == 255 && !fade_indicator) {
      blue = blue + 1;
      start_time = millis();
      set(false, red, green, blue);
    }

    if (blue == 255 && green == 0 && !fade_indicator) {
      red = red - 1;
      start_time = millis();
      set(false, red, 0, blue);
    };

    if (red == 0 && blue == 255 && !fade_indicator) {
      green += 1;
      start_time = millis();
      set(false, red, green, blue);
    }

    if (green == 255 && red == 0 && !fade_indicator) {
      blue -= 1;
      start_time = millis();
      set(false, red, green, blue);
      if (blue == 0) {
        fade_indicator = true;
      };
    }
    if (green == 255 && blue == 0 && fade_indicator) {
      red += 1;
      start_time = millis();
      set(false, red, green, blue);
    }
    if (blue == 0 && red == 255 && fade_indicator) {
      green -= 1;
      start_time = millis();
      set(false, red, green, blue);

      if (green == 0) { fade_indicator = false; }
    }
  }
}

//Fading in a specific color range  
  void fade_red(){
    uint32_t start_time;
  if (first_time_flag){
      first_time_flag=false;
      start_time=millis();
      red=255;
      blue=0;
      green=0;
      fade_indicator=false;
  }
  uint32_t current_time=millis();
  if(current_time-start_time>=50){
    if(!fade_indicator){
    blue+=1;
    green+=1;
    start_time=millis();
    set(false,red,green,blue);
      if(blue ==255 && green==255){
        fade_indicator=true;
        }
    }
    if(fade_indicator){
      blue-=1;
      green-=1;
      start_time=millis();
      set(false,red,green,blue);
        if(blue ==0 && green==0){
        fade_indicator=false;
         }
        }
      }
    }

  //Fading in a specific color range  
  void fade_green(){
    uint32_t start_time;
  if (first_time_flag){
      first_time_flag=false;
      start_time=millis();
      fade_indicator=false;
      red=0;
      blue=0;
      green=255;
  }
  uint32_t current_time=millis();
  if(current_time-start_time>=50){
    if(!fade_indicator){
    blue+=1;
    red+=1;
    start_time=millis();
    set(false,red,green,blue);
      if(blue ==255 && red==255){
        fade_indicator=true;
        }
    }
    if(fade_indicator){
      blue-=1;
      red-=1;
      start_time=millis();
      set(false,red,green,blue);
        if(blue ==0 && red==0){
        fade_indicator=false;
          }
        }
      }
    }
  
  //Fading in a specific color range  
  void fade_blue(){
    uint32_t start_time;
      if (first_time_flag){
          first_time_flag=false;
          start_time=millis();
          fade_indicator=false;
          red=0;
          blue=255;
          green=0;
      }
    uint32_t current_time=millis();
    if(current_time-start_time>=50){
      if(!fade_indicator){
      green+=1;
      red+=1;
      start_time=millis();
      set(false,red,green,blue);
        if(green ==255 && red==255){
          fade_indicator=true;
        }
      }
      if(fade_indicator){
        green-=1;
        red-=1;
        start_time=millis();
        set(false,red,green,blue);
          if(green ==0 && red==0){
          fade_indicator=false;
            }
          }
        }
      }


  //Dimming the brightness of the LED
  void dim(){
    red=red-(0.25*red);
    green=green-(0.25*green);
    blue=blue-(0.25*blue);
    set(true,red,green,blue);
    last_command=0x00;
    };
  
  
// Function to make fade smoothly to the desired color
void fadeToColor(uint8_t targetRed, uint8_t targetGreen, uint8_t targetBlue, uint16_t duration) {
  if(red==0 && blue ==0 && green==0){
    set(true,targetRed,targetGreen,targetBlue);}
  else{
  // Get the current color
  uint8_t currentRed = red; 
  uint8_t currentGreen = green;
  uint8_t currentBlue = blue;

  // Calculate the color step sizes
  float redStep = (float)targetRed - currentRed;
  float greenStep = (float)targetGreen - currentGreen;
  float blueStep = (float)targetBlue - currentBlue;

  // Calculate the delay between each step based on the desired duration
  uint8_t numSteps = max_y(abs(redStep),abs(greenStep),abs(blueStep));
  uint8_t stepDelay = duration / numSteps;

  // Perform the fade
    for (uint16_t i = 0; i <= numSteps; i++) {
      uint8_t newRed = uint8_t(currentRed + (redStep * i) / numSteps);
      uint8_t newGreen =uint8_t( currentGreen + (greenStep * i) / numSteps);
      uint8_t newBlue =uint8_t(currentBlue + (blueStep * i) / numSteps);
      
      set(false, newRed, newGreen, newBlue); // Replace with your own function to set the LED color
      delay(stepDelay);
    }
  // Set the final color to ensure accuracy
  set(true, targetRed, targetGreen, targetBlue);

  }
}

//Fade function that doesnt exclude white phases (255,255,255)
void three_way_fade(uint16_t delay_time){


  if (first_time_flag){
      first_time_flag=false;
      start_time=millis();
      red=255;
      blue=0;
      green=0;
      delay_time=0;
      fade_step_counter=0;
      }

  uint32_t current_time=millis();
  if(current_time-start_time>=delay_time){
    if(fade_step_counter==0){
    red-=1;
    blue+=1;
    start_time=millis();

    set(false, red, green, blue);
    if(blue==255 && red==0){
      fade_step_counter+=1;
      }
    };

    if(fade_step_counter==1){
      green+=1;
      blue-=1;
  
    start_time=millis();
    set(false,red,green,blue);
      if (green==255 && blue==0){
        fade_step_counter+=1;
      }
    };

    if(fade_step_counter==2){
      red+=1;
      green-=1;

      start_time=millis();
      set(false,red,green,blue);
      if (red==255 && green==0)
      {fade_step_counter=0;
        }
      }
    }
  }

  // Function to make the LEDs pulse or if  bright_time and dimmed_time are small enough even strobe
  void flash(float dimfactor,uint16_t bright_time, uint16_t dimmed_time){
    
    if(first_time_flag){
      start_time=millis();
    };
    uint32_t current_time=millis();
    first_time_flag=false;
    
    
    if(current_time-start_time>=dimmed_time && fade_indicator){
    start_time=millis();
    fade_indicator=false;
    set(false, red, green, blue);
    }
      if   (current_time-start_time>=bright_time && !fade_indicator){
        start_time=millis();
        analogWriteRGB(red*dimfactor, green*dimfactor, blue*dimfactor);
        fade_indicator=true;
      }
    } 
  };
RGB color;

void setup(){


};


void loop(){

  //Check if the IR-Sensor received Data
  irDecoder.dataAvailable(irData);

  //Make sure that the functions requiring a loop pass
  // This ensures that the LED Controller directly react to each Command of the remote control
  if (last_command!=irData.cmd || irData.cmd==0x1A||irData.cmd==0x4D||irData.cmd==0x9||irData.cmd==0xD||irData.cmd==0xC||irData.cmd==0x0){
  last_command=irData.cmd;

  //Execute the function corresponding to the command
  switch(irData.cmd){

  case 0x1A:
    color.fade(25);
  break;
  
  case 0x1D: color.dim();
  break;

  case 0x4D: color.flash(0.3, 200, 700);
  break;

  case 0x9: color.fade_red(); // fade_red()
  break;

  case 0xD: color.fade_green();
  break;

  case 0xC: color.fade_blue();
  break;

  case 0x0: color.flash(0,50,50);
  break;

  //Einfache Farben
  case 0x1F: color.three_way_fade(50);
  break;
  case 0x19: color.fadeToColor(255, 0, 0, 1000);
  
  break;
  case 0x1B: color.fadeToColor(0,255,0,1000);
  break;
  case 0x11:
  color.fadeToColor(0,0,255,1000);
  break;
  case 0x15: 
  color.fadeToColor(255,255,255,1000);
  break;
  case 0x17: 
  color.fadeToColor(255,25,0,1000);
  break;
  case 0x12: 
  color.fadeToColor(154,250,0,1000);
  break;
  case 0x16: 
  color.fadeToColor(85,0,255,1000);
  break;
  case 0x40:
  color.fadeToColor(255,55,0,1000);
  break;
  case 0x4C:
  color.fadeToColor(0,255,154,1000);
  break;
  case 0x4: 
  color.fadeToColor(255,0,50,1000);
  break;
  case 0xA: 
  color.fadeToColor(255,135,0,1000);
  break;
  case 0x1E:
  color.fadeToColor(0,255,255,1000);
  break;
  case 0xE:color.fadeToColor(255,0,100,1000);
  break;
  case 0x1C:color.fadeToColor(255,255,0,1000);
  break;
  case 0x14:color.fadeToColor(64,255,208,1000);
  break;
  case 0xF:color.fadeToColor(255,0,255,1000);
  break;
  break;
    };
  }
} 

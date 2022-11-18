#include "pico/stdlib.h"
#include <stdio.h>
#include "stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include <math.h>
#include "neopixel.h"  


#define QTPY_BOOT_PIN 21
void recording(bool array[],int secs,char sdir)
{
    stdio_init_all();                                           //Initialization functions
    int z=0;
    neopixel_set_rgb(0x00ff00ff);
    int div;
    int speed;                                                 //speedx slower, if speed=4, it will play 4x slower
    switch(sdir){
      case 'a'  : //Mode 0
      speed = 1;
      div=1;
      break; 
	
      case 'b'  : //Mode 1
      speed = 2;
      div=1;
      break; 

      case 'c'  : //Mode 2
      speed = 4;
      div=1;
      break; 

      case 'd'  : //Mode 3
      speed = 1;
      div=2;
      break; 

      case 'e'  : //Mode 4
      speed = 1;
      div=4;
      break; 
    
   default : 
    speed = 1;
    div=1;
    }
    while(true)
    {   
        if(!(gpio_get(QTPY_BOOT_PIN))){
            array[z]=true;
            
            sleep_ms(floor((5*speed)/div));
        }
        else{                                                   // If BOOT button is not pressed
        array[z]=false;
        sleep_ms(floor((5*speed)/div));
        }
        z=z+1;
        if(z>=((secs*1000)-2))
        {
            z=0;
            neopixel_set_rgb(0x00000000);
            break;
        }
    }
}


void play_recording(bool array[], int secs, char rgbw, char sdir)
{
  stdio_init_all();                                            
    uint32_t color;

    switch(rgbw){
            case 'r':
                color = 0x00ff0000;
                break;
            case 'g':
                color = 0x0000ff00;
                break;
            case 'b':
                color = 0x000000ff;
                break;
                default : 
                color = 0x000000ff;
    
    }
    int div;
    int speed;                                                 //speedx slower, if speed=4, it will play 4x slower
    switch(sdir){
      case 'a'  : //Mode 0
      speed = 1;
      div=1;
      break; 
	
      case 'b'  : //Mode 1
      speed = 2;
      div=1;
      break; 

      case 'c'  : //Mode 2
      speed = 4;
      div=1;
      break; 

      case 'd'  : //Mode 3
      speed = 1;
      div=2;
      break; 

      case 'e'  : //Mode 4
      speed = 1;
      div=4;
      break; 
    
   default : 
    speed = 1;
    div=1;
    }

    for(int j=0;j<=secs*1000;j=j+1)                             
            {
                if(array[j]==true)
                {
                     neopixel_set_rgb(color);
                    sleep_ms(floor((5*speed)/div));
                }

                else
                {
                    neopixel_set_rgb(0x00000000);
                    sleep_ms(floor((5*speed)/div));
                }
            }

neopixel_set_rgb(0x00000000);
}


void looping(bool array[], int secs, char rgbw, char sdir)
{
  stdio_init_all();                                            
    uint32_t color;
    char y = 0;
    switch(rgbw){
            case 'r':
                color = 0x00ff0000;
                break;
            case 'g':
                color = 0x0000ff00;
                break;
            case 'b':
                color = 0x000000ff;
                break;
                default : 
                color = 0x000000ff;
    
    }
    int div;
    int speed;                                                 //speedx slower, if speed=4, it will play 4x slower
    switch(sdir){
      case 'a'  : //Mode 0
      speed = 1;
      div=1;
      break; 
	
      case 'b'  : //Mode 1
      speed = 2;
      div=1;
      break; 

      case 'c'  : //Mode 2
      speed = 4;
      div=1;
      break; 

      case 'd'  : //Mode 3
      speed = 1;
      div=2;
      break; 

      case 'e'  : //Mode 4
      speed = 1;
      div=4;
      break; 
    
   default : 
    speed = 1;
    div=1;
    }

    int x = 0;

    for(int x=0;x<2;x=x+1)     
    { 
    for(int j=0;j<=secs*1000;j=j+1)                             
            {
            
                if(array[j]==true)
                {
                     neopixel_set_rgb(color);
                    sleep_ms(floor((5*speed)/div));
                }

                else
                {
                    neopixel_set_rgb(0x00000000);
                    sleep_ms(floor((5*speed)/div));
                }
            }
        
    }

neopixel_set_rgb(0x00000000);
}
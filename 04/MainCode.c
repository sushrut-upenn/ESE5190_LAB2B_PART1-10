#include "hardware/pio.h"                                                               
#include "pico/stdlib.h"                                                                
#include <stdio.h>                                                                      
#include "stdlib.h"                                                                     
#include "hardware/gpio.h"                                                              
#include "hardware/structs/sio.h"                                                       
#include "boot_button.h"                                                             
#include "data_transfer.h"   
#include "neopixel.h" 
#include "ws2812.pio.h" 
#include "registers.h"
#define PIO         pio0
#define SM          0
#define FREQ        800000
#define PIN         12
#define POWER_PIN   11
#define IS_RGBW     true   
#define QTPY_BOOT_PIN 21


void neopixel_init() {
    // power up WS2812 on Qt Py, using pin defs from board header file
    // pico-sdk/src/boards/include/boards/adafruit_qtpy_rp2040.h
    gpio_init(POWER_PIN);
    gpio_set_dir(POWER_PIN, GPIO_OUT);
    gpio_put(POWER_PIN, 1);

    // program PIO using helper function from ws2812.pio
    uint offset = pio_add_program(PIO, &ws2812_program);
    ws2812_program_init(PIO, SM, offset, PIN, FREQ, IS_RGBW);
}

void neopixel_set_rgb(uint32_t rgb) {
    // convert RGB to GRB
    uint32_t grb = ((rgb & 0xFF0000) >> 8) | ((rgb & 0x00FF00) << 8) | (rgb & 0x0000FF);
    pio_sm_put_blocking(pio0, 0, grb << 8u);
}


int main(){
 
    stdio_init_all();     
    gpio_init(QTPY_BOOT_PIN);
    gpio_set_dir(QTPY_BOOT_PIN, GPIO_IN);
    neopixel_init();                                                             
    char mode;                                                                     
    char color_rgbw;
    int sdir;                                                                         
    bool sequence[5001] ={0};                                                          
    while (!stdio_usb_connected()) {
      printf(".");
      sleep_ms(500);
    }
    while(true){
        printf("\nSelect the mode:\n");                                                            
        sleep_ms(10);
        scanf(" %c",&mode);                                                        
        switch(mode){                                                              
            case 'r':                                                                  
                sleep_ms(10);
                printf("At what speed do you want recording to play?\n");
                scanf(" %c",&sdir);  
                printf("Sequence is being recorded\n");
                recording(sequence,3,sdir);                                                   
                printf("\nThe recording session has concluded\n");
                break;
            case 'p':                                                                 
                printf("Play mode\n");
                sleep_ms(10);
                printf("Select a colour of your choice:\n");
                scanf(" %c",&color_rgbw);       
                printf("At what speed do you want recording to play?\n");
                scanf(" %c",&sdir);                                           
                printf("Recording is being played\n");
                play_recording(sequence,3,color_rgbw,sdir);                               
                printf("The playing session has ended\n");
                break;
            case 's':                                                                  
                printf("Save the recording\n");
                sleep_ms(10);
                encode(sequence);                                                     
                sleep_ms(20);
                printf("The recording has been saved\n");
                break;
            case 'l':
                printf("Loop the recording\n");
                sleep_ms(10);
                printf("Select a colour of your choice:\n");
                scanf(" %c",&color_rgbw);           
                printf("At what speed do you want recording to play?\n");
                scanf("%c",&sdir);                                        
                printf("Recording is being looped\n");
                looping(sequence,3,color_rgbw,sdir);                               
                printf("The looping has ended\n");
                break;
            case 'u':                                                                  
                printf("Upload the recording\n");
                char upload_sequence[1000];
                sleep_ms(500);
                printf("Recording is being uploaded\n");
                scanf(" %s",&upload_sequence);                                                  
                printf("%s",upload_sequence);
                decode(sequence,upload_sequence);                                             
                printf("%s",upload_sequence,"is the recorded sequence\n");
                break;
            case 'i':
                printf("\nEnter a string that you want to play\n");
                char command[100];
	             while (!stdio_usb_connected()){
		             sleep_ms(500);
	                }
	                scanf("%s", &command);
                    printf("%s",command);
                    for(int i=0; i<100; i++){
		        sleep_ms(500);
		        if(command[i] == '1'){
			    neopixel_set_rgb(0x00FF00FF);
    
			    sleep_ms(100);
		        } else{
			        neopixel_set_rgb(0x00000000);
			
			    sleep_ms(100);
			        }
		        }
	        neopixel_set_rgb(0x00FF00FF);
	        sleep_ms(10);
                    break;
            default:
                printf("Wrong command has been entered, please re-enter command\n");
        }
        sleep_ms(500);
    }
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <boards/adafruit_qtpy_rp2040.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW true
#define NUM_PIXELS 150
#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
// default to pin 2 if the board doesn't have a default WS2812 pin defined
#define WS2812_PIN 2
#endif
#define PICO_DEFAULT_WS2812_POWER_PIN 11
#define BOOT_PIN 21

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

int main() {  
    stdio_init_all();
    PIO pio = pio0;  // todo get free sm
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    int loop_play[300]={};
    for (int i=0; i<300; i++) {
        loop_play[i]=0;
    }

    const uint gpio = PICO_DEFAULT_WS2812_POWER_PIN;
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_OUT);
    
    while (1) {
        if(!(gpio_get(BOOT_PIN))==1){
            sleep_ms(300);
            for (int y=0; y<300; y++){
            printf("Enter the loop to be played\n");
            uint bootpinvalue = !(gpio_get(BOOT_PIN));
            sleep_ms(50);
                if (bootpinvalue==1){
                    loop_play[y]=1;
                }
                else{
                    loop_play[y]=0;
                }
            }

            sleep_ms(300);

            for (int x=0; x<300; x++){
                printf("The loop is being played\n");
                gpio_put(gpio, loop_play[x]);
                put_pixel(urgb_u32(0xff, 0, 0));
                sleep_ms(50);
            }
        }
    }
}

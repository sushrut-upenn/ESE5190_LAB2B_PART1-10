#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "registers.h"
#include <stdio.h>
#define QTPY_BOOT_PIN 21
#define Length 8

char input[Length + 1];
void read(void){
    uint x = 0;
    while(1){
        char y = getchar_timeout_us(200);

        if (y == 0x0D){
            return;
        }
        
        if ((y >= 0x20) && (y <= 0x7E)){
            printf("%c", y);
            if (x < Length){
                input[x++] = y;
            }
        }
    }
}


int main() {
    ADDRESS user_value1;
    ADDRESS user_value2;
    ADDRESS user_value3;
    ADDRESS user_value4;
    stdio_init_all();
    while(stdio_usb_connected()!=true);
    gpio_init(QTPY_BOOT_PIN);
    gpio_set_dir(QTPY_BOOT_PIN, GPIO_IN);
    
   
    printf("Press BOOT PIN to store value to the address\n");
    
    while(1){
        printf("\nRead Mode Enabled: Enter a valid adddress");
        printf("\nInput: \t");
        read();
        user_value1 = (long int)strtoul(input, NULL, 16);
        printf("\nThe address is %x", user_value1);
        ADDRESS results = register_read(user_value1);
        printf("\nThe Value of this address is ");
        printf("%x\n", results);
        if (gpio_get(QTPY_BOOT_PIN) == 1)
            break;
    }
    
    while(1){
        printf("\nWRITE Mode Enabled: Enter a valid adddress");
        printf("\nInput: \t");
        read();
        user_value2 = (long int)strtoul(input, NULL, 16);
        printf("\nThe address is %x", user_value2);
        ADDRESS results = register_read(user_value2);
        printf("\nThe Value of this address is ");
        printf("%x\n", results);
           
        printf("\nEnter a value to be stored in this address \nInput value: \t");
        read();
        user_value3 = (long int)strtoul(input, NULL, 16);
        register_write(user_value2, user_value3);
        ADDRESS results1 = register_read(user_value2);
        printf("\nAfter value is written: %x", results1);
        
        printf("\nEnter a MASK \nInput value: \t");
        read();
        user_value4 = (long int)strtoul(input, NULL, 16);
        uint32_t results2 = (uint32_t)results1 & (uint32_t)user_value4;
        printf("\nAfter masking action is completed: %x", results2);
        
        sleep_ms(1000);
    }
    return 0;
}
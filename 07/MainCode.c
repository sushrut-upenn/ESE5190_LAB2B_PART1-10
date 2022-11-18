/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// PIO logic analyser example
//
// This program captures samples from a group of pins, at a fixed rate, once a
// trigger condition is detected (level condition on one pin). The samples are
// transferred to a capture buffer using the system DMA.
//
// 1 to 32 pins can be captured, at a sample rate no greater than system clock
// frequency.

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/structs/bus_ctrl.h"
#include "registers.h"
#include "hardware/structs/pwm.h"
#include "pio_seq.pio.h"
#include "neopixel.h"
#include <math.h>

const uint CAPTURE_PIN_BASE = 21;
const uint CAPTURE_PIN_COUNT = 1;
const uint CAPTURE_N_SAMPLES = 7500;

void recording(bool array[], int secs, int sdir, char rgbw)
{
  pixel_init();
  stdio_init_all();
  neopixel_set_rgb(0x00ff00ff);
  gpio_init(23);
  gpio_set_dir(23,GPIO_OUT);                                        //Initialization functions
     /*
    Mode 0 - Normal Speed
    Mode 1 - 2x Slower Speed
    Mode 2 - 4x Slower Speed
    Mode 3 - 2x Faster Speed
    Mode 4 - 4x Faster Speed
    */
    uint32_t light_color;

    switch(rgbw){
            case 'r':
                light_color = 0x00ff0000;
                break;
            case 'g':
                light_color = 0x0000ff00;
                break;
            case 'b':
                light_color = 0x000000ff;
                break;
                default : 
                light_color = 0x000000ff;
    
    }

    int div;
    int speed;                                                 //speedx slower, if speed=4, it will play 4x slower
    switch(sdir){
      case 0  : //Mode 0
      speed = 1;
      div=1;
      break; 
	
      case -2  : //Mode 1
      speed = 2;
      div=1;
      break; 

      case -4  : //Mode 2
      speed = 4;
      div=1;
      break; 

      case 2  : //Mode 3
      speed = 1;
      div=2;
      break; 

      case 4  : //Mode 4
      speed = 1;
      div=4;
      break; 
  
   default : 
    speed = 1;
    div=1;

    }

for(int j=0;j<=7500;j=j+1)                              //Replaying the Recording
            {
                if(arr[j]==true)
                {
                    neopixel_set(light_color);
                    //pio_sm_put_blocking(pio0, 1,1);
                    gpio_put(23,1);
                    sleep_us(500);
                }

                else
                {
                    neopixel_set(0x00000000);
                    gpio_put(23,0);
                    sleep_us(500);
                }
            }

neopixel_set(0x00000000);
gpio_put(23,0);
}
static inline uint bits_packed_per_word(uint pin_count) {
    // If the number of pins to be sampled divides the shift register size, we
    // can use the full SR and FIFO width, and push when the input shift count
    // exactly reaches 32. If not, we have to push earlier, so we use the FIFO
    // a little less efficiently.
    const uint SHIFT_REG_WIDTH = 32;
    return SHIFT_REG_WIDTH - (SHIFT_REG_WIDTH % pin_count);
}

void logic_analyser_init(PIO pio, uint sm, uint pin_base, uint pin_count, float div) {
    // Load a program to capture n pins. This is just a single `in pins, n`
    // instruction with a wrap.
    uint16_t capture_prog_instr = pio_encode_in(pio_pins, pin_count);
    struct pio_program capture_prog = {
            .instructions = &capture_prog_instr,
            .length = 1,
            .origin = -1
    };
    uint offset = pio_add_program(pio, &capture_prog);

    // Configure state machine to loop over this `in` instruction forever,
    // with autopush enabled.
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_in_pins(&c, pin_base);
    sm_config_set_wrap(&c, offset, offset);
    sm_config_set_clkdiv(&c, div);
    // Note that we may push at a < 32 bit threshold if pin_count does not
    // divide 32. We are using shift-to-right, so the sample data ends up
    // left-justified in the FIFO in this case, with some zeroes at the LSBs.
    sm_config_set_in_shift(&c, true, true, bits_packed_per_word(pin_count));
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_RX);
    pio_sm_init(pio, sm, offset, &c);
}

void logic_analyser_arm(PIO pio, uint sm, uint dma_chan, uint32_t *capture_buf, size_t capture_size_words,
                        uint trigger_pin, bool trigger_level) {
    pio_sm_set_enabled(pio, sm, false);
    // Need to clear _input shift counter_, as well as FIFO, because there may be
    // partial ISR contents left over from a previous run. sm_restart does this.
    pio_sm_clear_fifos(pio, sm);
    pio_sm_restart(pio, sm);

    dma_channel_config c = dma_channel_get_default_config(dma_chan);
    channel_config_set_read_increment(&c, false);
    channel_config_set_write_increment(&c, true);
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm, false));

    dma_channel_configure(dma_chan, &c,
        capture_buf,        // Destination pointer
        &pio->rxf[sm],      // Source pointer
        capture_size_words, // Number of transfers
        true                // Start immediately
    );

    pio_sm_exec(pio, sm, pio_encode_wait_gpio(trigger_level, trigger_pin));
    pio_sm_set_enabled(pio, sm, true);
}

void print_capture_buf(const uint32_t *buf, uint pin_base, uint pin_count, uint32_t n_samples, uint state_m) {
    // Display the capture buffer in text form, like this:
    // 00: __--__--__--__--__--__--
    // 01: ____----____----____----
    printf("Capture:\n");
    bool buffer[10000];
    int count = 0;
    // Each FIFO record may be only partially filled with bits, depending on
    // whether pin_count is a factor of 32.
    uint record_size_bits = bits_packed_per_word(pin_count);
    for (int pin = 0; pin < pin_count; ++pin) {
        printf("%02d: ", pin + pin_base);
        for (int sample = 0; sample < n_samples; ++sample) {
            uint bit_index = pin + sample * pin_count;
            uint word_index = bit_index / record_size_bits;
            // Data is left-justified in each FIFO entry, hence the (32 - record_size_bits) offset
            uint word_mask = 1u << (bit_index % record_size_bits + 32 - record_size_bits);
            printf(buf[word_index] & word_mask ? "-" : "_");
            buffer[count] = !(buf[word_index] & word_mask);
            count++;
            //pio_sm_put_blocking(pio1, state_m, buf[word_index]);
        }
        printf("\n");
    }
    play(buffer,4,0,'r');
}

int main() {
    stdio_init_all();
    printf("PIO logic analyser example:\n");

    // We're going to capture into a u32 buffer, for best DMA efficiency. Need
    // to be careful of rounding in case the number of pins being sampled
    // isn't a power of 2.
    uint total_sample_bits = CAPTURE_N_SAMPLES * CAPTURE_PIN_COUNT;
    total_sample_bits += bits_packed_per_word(CAPTURE_PIN_COUNT) - 1;
    uint buf_size_words = total_sample_bits / bits_packed_per_word(CAPTURE_PIN_COUNT);
    uint32_t *capture_buf = malloc(buf_size_words * sizeof(uint32_t));
    hard_assert(capture_buf);

    // Grant high bus priority to the DMA, so it can shove the processors out
    // of the way. This should only be needed if you are pushing things up to
    // >16bits/clk here, i.e. if you need to saturate the bus completely.
    bus_ctrl_hw->priority = BUSCTRL_BUS_PRIORITY_DMA_W_BITS | BUSCTRL_BUS_PRIORITY_DMA_R_BITS;

    PIO pio = pio0;
    uint sm = 0;
    uint dma_chan = 0;

    logic_analyser_init(pio, sm, CAPTURE_PIN_BASE, CAPTURE_PIN_COUNT, 65536.f);

    printf("Arming trigger\n");
    
    /*/ PWM example: -----------------------------------------------------------
    /gpio_set_function(CAPTURE_PIN_BASE, GPIO_FUNC_PWM);
    gpio_set_function(CAPTURE_PIN_BASE + 1, GPIO_FUNC_PWM);
    // Topmost value of 3: count from 0 to 3 and then wrap, so period is 4 cycles
    pwm_hw->slice[0].top = 3;
    // Divide frequency by two to slow things down a little
    pwm_hw->slice[0].div = 4 << PWM_CH0_DIV_INT_LSB;
    // Set channel A to be high for 1 cycle each period (duty cycle 1/4) and
    // channel B for 3 cycles (duty cycle 3/4)
    pwm_hw->slice[0].cc =
            (1 << PWM_CH0_CC_A_LSB) |
            (3 << PWM_CH0_CC_B_LSB);
    // Enable this PWM slice
    pwm_hw->slice[0].csr = PWM_CH0_CSR_EN_BITS;*/
    // ------------------------------------------------------------------------

    // The logic analyser should have started capturing as soon as it saw the
    // first transition. Wait until the last sample comes in from the DMA.
    

    //PIO PIO1 = pio1;

    // Our assembled program needs to be loaded into this PIO's instruction
    // memory. This SDK function will find a location (offset) in the
    // instruction memory where there is enough space for our program. We need
    // to remember this location!
    uint offset1 = pio_add_program(pio1, &pio_seq_program);

    // Find a free state machine on our chosen PIO (erroring if there are
    // none). Configure it to run our program, and start it, using the
    // helper function we included in our .pio file.
    uint sm1 = 1;//pio_claim_unused_sm(pio1, true);
    pio_seq_init(pio0, sm1, offset1, 23);
    while(true){
    //printf("PIO logic analyser example:%d\n",clock_get_hz(0));
    VALUE a = register_read((ADDRESS)0xd0000004);
    //pixel_init();
    //gpio_init(22);
    
    //gpio_set_dir(22,GPIO_IN);
    sleep_ms(1);
    //gpio_put(23,1);
    if(!(1<<21 & a)){
    //pixel_set(0x000000ff);
    printf("Arming trigger\n");
    logic_analyser_arm(pio, sm, dma_chan, capture_buf, buf_size_words, CAPTURE_PIN_BASE, true);
    dma_channel_wait_for_finish_blocking(dma_chan);
    logic_analyser_arm(pio, sm, dma_chan, capture_buf, buf_size_words, CAPTURE_PIN_BASE, false);
    //gpio_put(23,0);
    //sleep_ms(2000);
    print_capture_buf(capture_buf, CAPTURE_PIN_BASE, CAPTURE_PIN_COUNT, CAPTURE_N_SAMPLES,sm1);
    }}
    //printf("Starting PWM example\n");
    
}
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "pico/platform.h"
// Inital
const uint LED_PIN = 25;
const uint CLK_PIN = 21;
const uint DATA_PIN = 20;
const uint BUTTON_PIN = 19;

//There is quite a number of functions available for this board, however we only need a few for basic operation.
#define XBOX_SYNC 		0x004	//Tell the RF module to sync with external controllers, and play the sync animation on the LEDs
#define XBOX_BOOTANIM	0x085	//Initalize the LEDs and play the Xbox 360 'power on' animation on the LEDs. An 'init' function (which this instruction is) must be run before the LEDs can be used.
#define XBOX_GREEN_ALL	0x0AF	//Turn on all green LEDs


void send_to_rf(uint16_t data) {
    // Bring the data pin low
    gpio_put(DATA_PIN, 0);
    // Send the data
    uint8_t index = 0;
    while(index < 10) 
    {
        while(gpio_get(CLK_PIN) != 0) {;} //Wait for clock line to go low
        // Send the data bit
        if((data & (0x200 >> index)) == 0) {
            gpio_put(DATA_PIN, 0);
        } else {
            gpio_put(DATA_PIN, 1);
        }
        while(gpio_get(CLK_PIN) == 0) {;} //Wait for clock line to go high
        index++;
    }
    // Bring the data pin high
    gpio_put(DATA_PIN, 1);
    return;
}

int main() {
    bi_decl(bi_program_description("This is a test binary."));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(CLK_PIN);
    gpio_set_dir(CLK_PIN, GPIO_IN);

    gpio_init(DATA_PIN);
    gpio_set_dir(DATA_PIN, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    sleep_ms(3000);
    send_to_rf(XBOX_BOOTANIM);

    while (1) {
        if (gpio_get(BUTTON_PIN) == 0) {
            gpio_put(LED_PIN, 1);
            send_to_rf(XBOX_SYNC);
            sleep_ms(5000);
        } else {
            gpio_put(LED_PIN, 0);
        }
    }
}
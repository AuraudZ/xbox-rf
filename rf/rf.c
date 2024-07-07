#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "pico/platform.h"

// Pin definitions
const uint LED_PIN = 25; // On-board LED
const uint CLK_PIN = 21; // Clock line for the RF module
const uint DATA_PIN = 20; // Data line for the RF module
const uint BUTTON_PIN = 19; // Button to trigger sync function

int start_cmd[10] = {0,0,0,0,0,1,0,0,1,0}; //Start Module (Needed for the sync led animation)
int power_cmd[10] = {0,0,1,0,0,0,0,1,0,1}; //Makes the startup animation on the ring of light.
int sync_cmd[11] = {0,0,0,0,0,0,0,1,0,0,1}; //Initiates the sync process.



void send_to_rf(int data[],int size) {
    // Bring the data pin low
    gpio_put(DATA_PIN, 0);
    // Send the data
    uint8_t index = 0;
    int prev = 1;
    for(int i = 0; i < size; i++) {
    while(gpio_get(CLK_PIN) == prev) {;}
    prev = gpio_get(CLK_PIN);
    sleep_us(1000);
    gpio_put(DATA_PIN, data[i]);
    while(gpio_get(CLK_PIN) == prev) {;}
    prev = gpio_get(CLK_PIN);
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
    send_to_rf(start_cmd,10);
    sleep_ms(100);
    send_to_rf(power_cmd,10);
    sleep_ms(100);

    while (1) {
        if (gpio_get(BUTTON_PIN) == 0) {
            gpio_put(LED_PIN, 0);
           // send_to_rf(XBOX_SYNC);
           // sleep_ms(5000);
        } else {
            gpio_put(LED_PIN, 1);
        }
    }
}
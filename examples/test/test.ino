#include <sst26vf_driver.h>

const uint8_t enable_pin = 2;

auto chip = sst25vf::flash_driver(enable_pin);

void setup() {
    SerialUSB.begin(115200); while(!serialUSB);

    SerialUSB.println("Begin chip communication test");
    SerialUSB.println("-----------------------------");

    chip.begin();

    uint8_t m, t, i;
    chip.get_manufacturer_info(&m, &t, &i);

    SerialUSB.println(m, HEX);
    SerialUSB.println(t, HEX);
    SerialUSB.println(i, HEX);
    
    SerialUSB.println("-----------------------------");
}

void loop() {
    asm __volatile__ ("nop");
}

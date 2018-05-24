#include <sst26vf_driver.h>

const uint8_t enable_pin = 2;

auto chip = sst26vf::flash_driver(enable_pin);

void setup() {
    SerialUSB.begin(115200); while(!SerialUSB);

    SerialUSB.println("Begin write communication test");
    SerialUSB.println("-----------------------------");

    chip.begin();

    uint8_t buf[] = {1, 2, 3, 4};
    uint8_t len = chip.write_buffer(0, buf, 4);

    SerialUSB.println("\nBegin read communication test");
    SerialUSB.println("-----------------------------");

    uint8_t res[4];
    uint8_t r_len = chip.read_buffer(0, res, 4);

    for (int i=0; i<4; i++) {
        SerialUSB.println(res[i], DEC);
    }
    
    SerialUSB.println("-----------------------------");
}

void loop() {
    asm __volatile__ ("nop");
}

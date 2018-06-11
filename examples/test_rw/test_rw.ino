#include <sst26vf_driver.h>

const uint8_t enable_pin = 2;

auto chip = sst26vf::flash_driver(enable_pin);

void setup() {
    SerialUSB.begin(115200); while(!SerialUSB);

    SerialUSB.println("Begin write communication test");
    SerialUSB.println("-----------------------------");

    chip.begin();

    uint8_t buf[] = {1, 2, 3, 4};
    // if(!chip.erase_sector(0))
        // SerialUSB.println("\nErase sector failed, write enable latch not set");
    uint8_t len = chip.write_buffer(0, buf, 4);

    if (len < 4) {
        SerialUSB.print("\nWrite failed, only able to write ");
        SerialUSB.print(len);
        SerialUSB.println(" bytes");
        return;
    }

    SerialUSB.println("\nBegin read communication test");
    SerialUSB.println("-----------------------------");

    uint8_t res[4] = { 0 };
    uint8_t r_len = chip.read_buffer(0, res, 4);

    for (int i=0; i<r_len; i++) {
        SerialUSB.println(res[i], HEX);
    }

    // SerialUSB.println("\nBegin erase communication test");
    // SerialUSB.println("-----------------------------");

    // chip.erase_sector(0);

    // SerialUSB.println("\nCheck if erase successfull");
    // SerialUSB.println("-----------------------------");

    // r_len = chip.read_buffer(0, res, 4);

    // for (int i=0; i<r_len; i++) {
    //     SerialUSB.println(res[i], DEC);
    // }
    
    SerialUSB.println("-----------------------------");
}

void loop() {
    asm __volatile__ ("nop");
}

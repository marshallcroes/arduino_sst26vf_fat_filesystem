#include <sst26vf.h>

const uint8_t enable_pin = 2;

auto chip = sst26vf::flash_driver(enable_pin);

void setup() {
	SerialUSB.begin(115200); while(!SerialUSB);

	chip.begin();

	auto& fs = sst26vf::filesystem::instance();
	fs.attach(std::move(chip));

	fs.reformat();

	if (fs.mount())
		SerialUSB.println("FAT mount successfull!");
	else
		SerialUSB.println("Failed mounting filesystam!");
}

void loop() {
	asm __volatile__("nop");
}

#include "Rom.hpp"

Rom::Rom(){}
Rom::~Rom(){
	free(this->romData);
}

void Rom::write(uint16_t addr, uint8_t value) {
	(void)addr;
	(void)value;
	return ;
	//this->romData[addr] = value;
}
uint8_t Rom::read(uint16_t addr) {
	return this->romData[addr];
}

char const* Rom::romLicName() const {
	if (this->header->newLicCode <= 0xA4)
		return (LIC_CODE[this->header->licCode]);
	return ("UNKNOWN");
}

char const* Rom::romTypeName() const {
	if (this->header->type <= 0x22)
		return (ROM_TYPES[this->header->type]);
	return ("UNKNOWN");
}

bool Rom::romLoad(char *rom) {
	std::fstream	fs;
	this->filename = rom;

	fs.open(rom, std::fstream::in);
	if (fs.is_open() == false) {
		std::cerr << "Error: Failed to open file: " << this->filename << std::endl;
		return (false);
	}
	std::cout << "Success opening file: " << this->filename << std::endl;
	fs.seekg(0, fs.end);
	this->romSize = fs.tellg();
	fs.seekg(0, fs.beg);
	this->romData = (uint8_t *)malloc(this->romSize);
	if(!this->romData) {
		std::cerr << "Error: Malloc romData failed" << std::endl;
		return (false);
	}
	fs.read((char *)this->romData, this->romSize);
	fs.close();
	this->header = (Rom::rom_header *)(this->romData + 0x100);
	this->header->title[15] = '\0';
	uint16_t x = 0;
	for (uint16_t i = 0x0134; i <= 0x014C; ++i) {
		x = x - this->romData[i] - 1;
	}
	printf(
		"Rom Loaded:\n"
		"\t Title    : %s\n"
		"\t Type     : %2.2X (%s)\n"
		"\t Rom Size : %dKB\n"
		"\t Ram Size : %2.2X\n"
		"\t Lic Code : %2.2X (%s)\n"
		"\t Rom Ver. : %2.2X\n"
		"\t Checksum : %2.2X (%s)\n",
		this->header->title,
		this->header->type, this->romTypeName(),
		(32 << this->header->romSize),
		this->header->ramSize,
		this->header->licCode, this->romLicName(),
		this->header->version,
		this->header->checksum,
		(x & 0xFF) ? "PASSED" : "FAILED"
	);
	return (true);
}

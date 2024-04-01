/* Specifications 

//16 8-bit Registers VO-VF that can hold values from 0x00 to 0xFF. VF holds flag about the results of the operations

//4096 bytes of memory with address from 0x000 to 0xFFF.
0x000-0x1FF : will not need to use this bits 
0X050-0X0A0 : Storage space for the 16 built in characters (0 through F). Need to manually put these into memory because roms will be looking for them
0x200-0xFFF : Instructions will be stored at 0x200, and anything left after the ROM's space is free to use

//16-bit Index Register
The Index Register is a special register used to store memory addresses for use in operations
It is 16 bits because the the max address is 0XFFF which is too big for an 8 bit register

//16-bit Porgram Counter
Special register that holds the address of the next instruction to execute 
The max address is 0XFFF
The opcode is two bytes but memory (operand) is a single byte, so we need to fetch a byte from PC and a byte from pc + 1 and connect 
them into a single value. We then need to increment the PC by 2 because we need to increment the PC before we execute any instrucions 
because some instructions will manipulate the PC to control program flow.  Some will ad to the PC, some will subtract from it, and some
will change it completely

//16-Level Stack
Stack allows cpu to keep track of the order of the execution when it calls functions. When there is a call the cpu needs to know where it 
was from in order to successfully return. The stack holds the PC value when the call instruction was executed. The return statement pulls 
that addressfrom the stack and puts it back into the pc so we the cpu will execute it on the next cycle. Pushing a PC onto the stack is
called pushing and pulling a PC of the stack is called popping

//8-bit Stack Pointer
We need a Stack Pointer (S0) to tell us where in the 16-levels of stack our most recent value was placed (the top)
The SP will be an array with sixteen indices
When we pop a value off the stack, we won't actually delete it from the array but instead just copy the value and decrement the SP
so it "points" to the previous value

//8 bit Delay Timer
The CHIP-8 has a simpler timer used for timing. If the timer value is 0, it stays 0. If it is loaded with a value it will decrement
at a rate of 60HZ. We will just have the timer decrement at whatever rate the cylce clock is set it

//8-bit Sound Timer
The CHIP-8 also has another time used for sound. Its behavior is the same as the delay timer. A sinlge tone will buzz when it's non-zero

//16 Input Keys
The CHIP-8 has 16 inpute keys that macth the first 16 hex values: 0 through F. Each key is either pressed or not pressed.

This is the key mapping that will be used
Keypad       Keyboard
+-+-+-+-+    +-+-+-+-+
|1|2|3|C|    |1|2|3|4|
+-+-+-+-+    +-+-+-+-+
|4|5|6|D|    |Q|W|E|R|
+-+-+-+-+ => +-+-+-+-+
|7|8|9|E|    |A|S|D|F|
+-+-+-+-+    +-+-+-+-+
|A|0|B|F|    |Z|X|C|V|
+-+-+-+-+    +-+-+-+-+



//64 * 32 Monochrome Displaye Memory
Memory buffer used for storing the graphics to display. It is 64 pixels wide and 32 pixels high
A pixel can either be on or off. We will be using a unit32 to make it easy to use with SDL
On will be 0XFFFFFFFF and Off will be 0x00000000

The draw instruction iterates over each pixel in a sprite and XORs the sprite pixel with the display pixel.

Sprite Pixel Off XOR Display Pixel Off = Display Pixel Off
Sprite Pixel Off XOR Display Pixel On = Display Pixel On
Sprite Pixel On XOR Display Pixel Off = Display Pixel On
Sprite Pixel On XOR Display Pixel On = Display Pixel Off

*/

//Implementation

#include <stdint.h>
#include <fstream>
#include <random>
#include "Chip8.hpp"
#include <chrono>
#include <string>

//class for the Chip
//initialize all values to 0
class Chip8{
    public:
        uint8_t registors[16]{};
        uint8_t memory[4096]{};
        uint16_t index{};
        uint16_t pc{};
        uint16_t stack[16]{};
        uint8_t sp{};
        uint8_t delayTimer{};
        uint8_t soundTimer{};
        uint8_t keypad[16]{};
        uint32_t video[16 * 32]{};
        uint16_t opcode;

};
const unsigned int FONTSET_SIZE = 80;

uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


Chip8::Chip8()
    : randGen(std::chrono::system_clock::now().timesince_epoch().count(){
    pc = START_ADDRESS;


    for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
	{
		memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}
}
//start address needs to be 0x200 because that is where the instructions are stored
const unsigned int START_ADDRESS  = 0x200;

//need to load the rom instructions
void Chip8::LoadROM(char const* filename){
    
    //Open the file as as tream of binary and move the file pointer to the end
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    
    if(file.is_open()){
        //get size of file and allocate a buffer to hold the contents
        std::streampos size = file.tellg();
        char* buffer = new char[size];

        //seek back to beginning of the file and read contents to the buffer
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for(long i = 0; i < size; ++i){
            memory[START_ADDRESS + 1] = buffer[i];
        }


    }
}

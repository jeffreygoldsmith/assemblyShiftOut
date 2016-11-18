/*
Author: Jeffrey Goldsmith
Teacher: Mr D'arcy
Course: TEI4M
*/

uint8_t data = 2;
uint8_t clk = 3;
uint8_t latch = 4;

byte shiftData = 0b11110001;




void setup() {
  // put your setup code here, to run once:
  asm(
    "start:              \n"
    "ser r30             \n" // Set r30 high
    "out 0x0A, r30       \n" // Set all pins to output

    "init:               \n"
    "cbi 0x0B, 0x04      \n" // Set latch pin low
    "lds r18, shiftData  \n" // Load data to send to shift register
    "ldi r20, 128        \n" // Load register with a value to mask data with

    "shiftdata:          \n"
    "cbi 0x0B, 0x03      \n" // Set clock pin low
    "rcall delay         \n" // Delay
    "tst r20             \n" // Check if the mask is 0
    "breq end            \n" // If it is go to the end, we're done
    "mov r19, r18        \n" // Copy the data to another register to keep it safe
    "and r19, r20        \n" // Mask the data to get the desired bit
    "breq zero           \n" // If the value of the bit is 0, branch to send a 0 to the shift register
    "sbi 0x0B, 0x02      \n" // Set data pin high, send a 1 to the shift register
    "lsr r20             \n" // Shift the mask to the right
    "rjmp clockend       \n"

    "zero:               \n"
    "cbi 0x0B, 0x02      \n" // Set data pin low, send a 0 to the shift register
    "lsr r20             \n" // Shift the mask to the right
    
    "clockend:           \n"
    "sbi 0x0B, 0x03      \n" // Set clock pin high
    "rcall delay         \n" // Delay
    "rjmp shiftdata      \n" // Go back to send out the next bit

    "delay:              \n"  // Delay block
    "dec r8              \n"  // Decrement r8
    "brne delay          \n"  // Jump back to top of delay block if r8 is not 0
    "dec r9              \n"  // Decrement r9
    "brne delay          \n"  // Jump back to top of delay block if r9 is not 0
    "ret                 \n"  // Return from delay blockreturns back to where delay was called

    "end:                \n"
    "sbi 0x0B, 0x04      \n" // Bring high pin 4 (latch pin)
    "nop                 \n"
  );
}

void loop() {}

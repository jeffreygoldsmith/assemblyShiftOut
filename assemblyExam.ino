/*
Author: Jeffrey Goldsmith
Teacher: Mr D'arcy
Course: TEI4M
*/

uint8_t data = 2;
uint8_t clk = 3;
uint8_t latch = 4;

byte shiftData = B10101010;

void setup() {
  // put your setup code here, to run once:
  asm(
    "start:              \n"
    "ser r30             \n" // Set r30 high
    "out 0x0A, r30       \n" // Set all pins to output
    "ldi 16, 1           \n" // Zero constant
    "ldi r17, 0          \n" // One constant
    "lds r18, shiftData  \n" // Load data to send to shift register
    "ldi r24, 0          \n" // Direction flag
    "ldi r25, 1          \n" // Mask for changing the state of an LED
    "rjmp forwards       \n" // Begin loop

    "init:               \n"
    "ldi r20, 128        \n" // Load register with a value to mask data with
    "cbi 0x0B, 0x04      \n" // Set latch pin low

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
    
    "delaysecond:        \n"
    "dec r8              \n" // Decrement registers for 16.8m cycles
    "brne delay          \n"
    "dec r9              \n"
    "brne delay          \n"
    "dec r16             \n"
    "brne delay          \n"
    "ret                 \n"
    
    // 1 means forwards (r16 = 1)
    "forwards:           \n"
    "cpse r24, r16       \n" // If the direction flag shows that we are not in forwards mode
    "ldi r25, 1          \n" // Set the mask to be equal to 1
    "ldi r24, 1          \n" // Set the direction flag to be forwards
    "eor r18, r25        \n" // Swap the colour of a single LED (ie change 10 to 01) and shift the mask
    "lsl r25             \n" 
    "eor r18, r25        \n" 
    "lsl r25             \n"
    "ldi r16, 255        \n" // Delay for 1 second
    "rcall delaysecond   \n"
    "cpi r25, 128        \n" // Check if the mask is equal to 128
    "brne init           \n" // If it is not equal, loop again
    "ldi r24, 0          \n" // When we are done set the direction flag to be backwards
    
    // 0 means back (r17 = 0)
    "backwards:          \n"
    "cpse r24, r17       \n" // If the direction flag shows we are not in backwards mode
    "ldi r25, 128        \n" // Set the mask equal to 128
    "ldi r24, 0          \n"
    "eor r18, r25        \n" // Swap the colour of a single LED (ie change 01 to 10) and shift the mask
    "lsr r25             \n"
    "eor r18, r25        \n"
    "lsr r25             \n"
    "ldi r16, 255        \n" // Delay for 1 second
    "rcall delaysecond   \n"
    "tst r25             \n" // Check if the mask is equal to 0
    "brne init           \n" // If it is not equal, loop again
    "ldi r24, 1          \n"

    "end:                \n"
    "sbi 0x0B, 0x04      \n" // Bring high pin 4 (latch pin) 
    "tst r24             \n" // Depending on the value of the direction flag execute the forwards or backwards branch
    "breq backwards      \n"
    "brne forwards       \n" 
  );
}

void loop() {}

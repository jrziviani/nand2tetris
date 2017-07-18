// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
@7
D=A // load 7 into reg D
@R0
M=D // load reg D into RAM[0]
@4
D=A // load 4 into reg D
@R1
M=D // load reg D into RAM[1]
@R2
M=0 // initializes RAM[2] = 0

(LOOP)
    @R1
    D=M    // load RAM[1] into reg D
    @END
    D;JEQ  // if reg D == 0 jump to END
    @R0
    D=M    // load RAM[0] into reg D
    @R2
    M=M+D  // load RAM[2] + reg D into RAM[2]
    @R1
    M=M-1  // load RAM[1] - 1 into RAM[1]
    @LOOP
    0;JMP  // jump to LOOP

(END)
    @END
    0;JMP  // end loop

// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

(LOOP)
    @KBD
    D=M         // load key code into reg D
    @LOOP
    D;JEQ       // if nothing read, restart loop
    @i
    M=1         // set a global i to 1
    @BLINK
    0;JMP       // "call" BLINK function
    
(BLINK)
    @i
    D=M         // load global i into reg D
    @LOOP
    D;JLT       // return if reg D is -1
    @8192
    D=A         // load index into reg D
    @R0
    M=D         // load index into RAM[0]
    (ILOOP)
        @R0
        D=M     // load RAM[0] (index) into reg D
        @END
        D;JLT   // exit ILOOP if reg D is -1
        @SCREEN
        A=A+D   // sum index + SCREEN offset addr
        M=!M    // turn on or off pixel based on last status
        @R0
        M=M-1   // decrease RAM[0] (index)
        @ILOOP
        0;JMP   // loop

    (END)
        @i
        M=M-1   // decrease global i
        @BLINK
        0;JMP   // "recursive call"

OPCODE          | OP1           | OP2           | OP3       |
----------------|---------------|---------------|-----------|
0x00 load       | destination   | data_id       | -         |
0x01 store      | data_id       | origin        | -         |
0x02 mov        | destination   | origin        | -         |
0x03 jmp        | destination   |  -            | -         |
0x04 je         | destination   |  -            | -         |
0x05 jne        | destination   |  -            | -         |
0x06 jg         | destination   |  -            | -         |
0x07 jge        | destination   |  -            | -         |
0x08 jl         | destination   |  -            | -         |
0x09 jle        | destination   |  -            | -         |
0x0A add        | destination   | origin        | adder     |
0x0B sub        | destination   | origin        | subtractor|
0x0C mul        | destination   | origin        | multiplier|
0x0D div        | destination   | origin        | divisor   |
0x0E mod        | destination   | origin        | divisor   |
0x0F func       | function_id   |  -            |  -        |
0x10 call       | function_id   | destination   |  -        |
0x11 ret        | -             |  -            |  -        |
0x12 cmp        | destination   | origin        |  -        |
0x13 ngt        | destination   |  -            |  -        |
0x14 push       | operand       |  -            |  -        |
0x15 pop        | destination   |  -            |  -        |
0x16 and        | destination   | origin        | operand   |
0x17 or         | destination   | origin        | operand   |
0x18 xor        | destination   | origin        | operand   |
0x19 not        | destination   | origin        |  -        |
**MOD**         |                
0x1A advance    | destination   | origin        | offset    |
0x1B ref        | destination   | origin        |  -        |
0x1C deref      | destination   | origin        |  -        |   
**EXTRA**       |
0x80 syscall    |  -            | -             |  -

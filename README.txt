
// --> it is a comment symbol 
_______________________________________________________________________________________
WHEN YOU WANT TO WORK WITH RAM YOU CAN RAM ADDRES(0-255) LOAD IN BASE REGISTER BX OR bx  
___________________________________________________
COMMAND FORMAT EXAMPLE
_____________________________________
              32 bit                 |
XXXXXXXX|YYYYYYYY|ZZZZZZZZ|KKKKKKKK  |
  8bit     8bit     8bit     8bit    |
_____________________________________| 

XXXXXXXX-->  IT IS A COMMAND
YYYYYYYY-->  IT IS A SOURCE 1
ZZZZZZZZ-->  IT IS A SOURCE 2
KKKKKKKK-->  IT IS A DESTINATION
___________________________________________________

1XXXXXXX--> YYYYYYYY-is IMMIDIATE
0XXXXXXX--> YYYYYYYY-is REGISTER 
X1XXXXXX--> ZZZZZZZZ-is IMMIDIATE
X0XXXXXX--> ZZZZZZZZ-is REGISTER 
00XXXXXX--> YYYYYYYY and ZZZZZZZZ- REGISTERS
11XXXXXX--> YYYYYYYY and ZZZZZZZZ- IMMIDATE
____________________________________________________

XXXXX0XXX--> IT IS A ALU COMMAND
XXXXX1XXX--> IT IS A CONDITION

____________________________________________________

XXXXX0TTT--> ALU INSTRUCTION NUMBER

TTT= 000--> add
TTT= 001--> or
TTT= 010--> not
TTT= 100--> and
TTT= 011--> sub
TTT= 110--> mult
TTT= 001--> copy
TTT= 001--> mov  
TTT= 101--> shl  // shift left
TTT= 111--> shr  // shift right
_____________________________________________________

XXXXX1TTT--> CONDITION INSTRUCTION NUMBER

TTT= 000--> je     jump when equal 
TTT= 001--> jne    jump when not equal 
TTT= 010--> jg     jump when greater then 
TTT= 100--> jge    jump when greater then or equal 
TTT= 011--> jl     jump when less then 
TTT= 110--> jle    jump when less equal  
______________________________________________________






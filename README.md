
# Assembly language instructions
###     |XXXXXXXX|YYYYYYYY|ZZZZZZZZ|KKKKKKKK| ---->32 bit
###       _____________       source1 ___         source2 _ destinaton


| IMMID OR REG | SOURCE 1 |SOURCE 2|
| ----- | ------- |--------|
| 00XXXXXX | REGISTER |REGISTER|
| 01XXXXXX | REGISTER  |IMMIDIATE|
| 10XXXXXX | IMMIDIATE  |REGISTER|
| 11XXXXXX | IMMIDIATE  |IMMIDIATE|



| INSTRUCTION  | TYPE |
| ----- | ------- |
| XXXX0XXX | ARITHMETIC |
| XXXX1XXX | CONDITION  |





| ARITHMETIC | TYPE |                                    CONDITION | TYPE |              
| ----- | ------- |                                      ----- | ------- |
| XXXX0000 | ADD |                                        XXXX1000 | JE |
| XXXX0001 | OR |                                         XXXX1001 | JNE |
| XXXX0001 | MOV |                                        XXXX1010 | JG |
| XXXX0001 | COPY  |                                      XXXX1100 | JGE  |
| XXXX0010 | NOT  |                                       XXXX1011 | JL  |
| XXXX0100 | AND  |                                        XXXX1110 | JLE  |
| XXXX0011 | SUB  |
| XXXX0110 | MULT |
| XXXX0101 | SHL  |
| XXXX0111 | SHR  |

| CONDITION | TYPE |                        
| ----- | ------- |
| XXXX1000 | JE |
| XXXX1001 | JNE |
| XXXX1010 | JG |
| XXXX1100 | JGE  |
| XXXX1011 | JL  |
| XXXX1110 | JLE  |

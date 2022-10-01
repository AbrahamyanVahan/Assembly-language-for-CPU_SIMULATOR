#include <iostream>
#include <string>
#include "Translate.h"






int main()
{
   Translate tr("Assembler_INPUT.txt");
   std::cout<< tr.get_commands();
   tr.write();
   return 0;
}
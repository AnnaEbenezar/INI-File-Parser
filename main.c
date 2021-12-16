#include <stdio.h>
#include <stdlib.h>
#include "ini_lib.h"
#include "ini_lib.c"

int main() 
{
    char fileName[] = "example.ini";
    parse_ini(fileName);
}
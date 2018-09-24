#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    char *str = "First string";
    char *str2 = "Second string";

    strcpy(str, str2);
    return 0;
}
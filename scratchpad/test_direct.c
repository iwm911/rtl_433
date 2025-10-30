#include <stdio.h>
#include <string.h>
int main() {
    char hex_buf[64];
    int data = 4660;
    const char *format = "%04x";
    snprintf(hex_buf, sizeof(hex_buf), format, data);
    printf("Format: '%s'\n", format);
    printf("Result: '%s'\n", hex_buf);
    printf("Contains %%x: %s\n", strstr(format, "%x") ? "yes" : "no");
    return 0;
}

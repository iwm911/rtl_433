#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "output_file.h"

int main() {
    int failures = 0;

    // Test 1: Lowercase hex format %x
    {
        FILE *tmp = tmpfile();
        struct data_output *json_output = data_output_json_create(0, tmp);
        data_t *data = data_make("test_hex_lower", "", DATA_FORMAT, "%x", DATA_INT, 4660, NULL); // 0x1234
        json_output->output_print(json_output, data);
        rewind(tmp);
        char buffer[256];
        fgets(buffer, sizeof(buffer), tmp);
        printf("Test 1 (lowercase hex %%x): %s", buffer);
        if (strstr(buffer, "\"1234\"")) {
            printf("✓ PASSED\n\n");
        } else {
            printf("✗ FAILED\n\n");
            failures++;
        }
        data_output_free(json_output);
        fclose(tmp);
    }

    // Test 2: Uppercase hex format %X
    {
        FILE *tmp = tmpfile();
        struct data_output *json_output = data_output_json_create(0, tmp);
        data_t *data = data_make("test_hex_upper", "", DATA_FORMAT, "%X", DATA_INT, 4660, NULL); // 0x1234
        json_output->output_print(json_output, data);
        rewind(tmp);
        char buffer[256];
        fgets(buffer, sizeof(buffer), tmp);
        printf("Test 2 (uppercase hex %%X): %s", buffer);
        if (strstr(buffer, "\"1234\"")) {
            printf("✓ PASSED\n\n");
        } else {
            printf("✗ FAILED\n\n");
            failures++;
        }
        data_output_free(json_output);
        fclose(tmp);
    }

    // Test 3: Decimal format (no format string)
    {
        FILE *tmp = tmpfile();
        struct data_output *json_output = data_output_json_create(0, tmp);
        data_t *data = data_make("test_decimal", "", DATA_FORMAT, NULL, DATA_INT, 4660, NULL);
        json_output->output_print(json_output, data);
        rewind(tmp);
        char buffer[256];
        fgets(buffer, sizeof(buffer), tmp);
        printf("Test 3 (decimal/no format): %s", buffer);
        if (strstr(buffer, ": 4660")) {
            printf("✓ PASSED\n\n");
        } else {
            printf("✗ FAILED\n\n");
            failures++;
        }
        data_output_free(json_output);
        fclose(tmp);
    }

    // Test 4: Hex format with width specifier %04x
    {
        FILE *tmp = tmpfile();
        struct data_output *json_output = data_output_json_create(0, tmp);
        data_t *data = data_make("test_hex_padded", "", DATA_FORMAT, "%04x", DATA_INT, 4660, NULL); // 0x1234 -> should be "1234"
        json_output->output_print(json_output, data);
        rewind(tmp);
        char buffer[256];
        fgets(buffer, sizeof(buffer), tmp);
        printf("Test 4 (padded hex %%04x): %s", buffer);
        printf("Debug: format string in data: %s\n", data->format);
        // Check if it's hex string (should contain "1234" as string)
        if (strstr(buffer, "\"1234\"")) {
            printf("✓ PASSED\n\n");
        } else {
            printf("✗ FAILED - Expected hex string, got decimal\n\n");
            failures++;
        }
        data_output_free(json_output);
        fclose(tmp);
    }

    if (failures == 0) {
        printf("All tests PASSED! ✓\n");
        return 0;
    } else {
        printf("%d test(s) FAILED ✗\n", failures);
        return 1;
    }
}

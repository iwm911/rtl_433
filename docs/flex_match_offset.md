# Flex Decoder: match_offset Parameter

## Overview

The `match_offset` parameter allows you to match specific bit patterns at a precise offset from the start or end of a message. Unlike `get`, which extracts data, `match_offset` only checks if the pattern exists at the specified position.

## Syntax

```
match_offset=<offset>@<bits>
```

- `<offset>`: Bit position (positive for start, negative for end)
- `<bits>`: Bit pattern in format `{<bit_count>}<hex_value>`

## Examples

### Match from Start

Match 8 bits with value 0xAA at bit offset 16 from the start:

```bash
-X "n=example,m=OOK_PCM,s=500,l=1000,r=5000,match_offset=16@{8}0xAA"
```

### Match from End

Match 8 bits with value 0xFF at 8 bits from the end (negative offset):

```bash
-X "n=example,m=OOK_PCM,s=500,l=1000,r=5000,match_offset=-8@{8}0xFF"
```

This checks if the last byte is 0xFF.

### Match 16-bit Checksum at End

Match a 16-bit pattern 16 bits from the end:

```bash
-X "n=example,m=OOK_PCM,s=500,l=1000,r=5000,match_offset=-16@{16}0x1234"
```

### Match Device ID at Specific Position

Match a 12-bit device ID at bit position 24:

```bash
-X "n=sensor,m=OOK_PWM,s=400,l=800,r=7000,match_offset=24@{12}0xA5C"
```

## Use Cases

1. **Fixed Position IDs**: Match device IDs that always appear at the same position
2. **End Markers**: Verify specific end-of-message patterns
3. **Protocol Validation**: Ensure specific bits are set at known positions
4. **Header/Footer Matching**: Match fixed headers or footers without extracting them

## Combining with Other Parameters

You can combine `match_offset` with other flex parameters:

```bash
-X "n=device,\
    m=OOK_PWM,s=400,l=800,r=7000,\
    match={8}0xAA,\
    match_offset=0@{8}0x55,\
    match_offset=-8@{8}0xFF,\
    get=8@{16}:device_id"
```

This will:
1. Find 0xAA anywhere in the message (`match`)
2. Verify 0x55 at the start (`match_offset=0`)
3. Verify 0xFF at the end (`match_offset=-8`)
4. Extract device_id from bit 8 (`get`)

## Notes

- The offset is in **bits**, not bytes
- Messages that don't match are discarded (returns DECODE_FAIL_SANITY)
- For negative offsets, the calculation is: `message_length + offset - pattern_length`
- Invalid offsets (out of bounds) cause the row to be skipped


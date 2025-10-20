# Flex Decoder: min_bits_decode and max_bits_decode Parameters

## Overview

The `min_bits_decode` and `max_bits_decode` parameters allow you to filter decoded results by length **after line coding operations** (UART decode, Manchester decode, or Differential Manchester decode).

- **`min_bits_decode`** - Filter out results shorter than specified length
- **`max_bits_decode`** - Filter out results longer than specified length

This is particularly useful when working with captures that contain multiple bursts of varying lengths, where you only want to process messages within a specific size range after decoding.

## Difference from `bits` Parameter

- **`bits=<n>`** - Filters based on the **raw bit length before** line coding/decoding
- **`min_bits_decode=<n>`** - Filters based on the **decoded bit length after** line coding/decoding (minimum)
- **`max_bits_decode=<n>`** - Filters based on the **decoded bit length after** line coding/decoding (maximum)

Since line coding operations like Manchester and UART decoding reduce the bit count (e.g., Manchester typically halves the bit count, UART converts 10 bits to 8), you need these parameters to filter on the final decoded length.

## Syntax

```
min_bits_decode=<n>
max_bits_decode=<n>
```

- `<n>`: Number of bits for the filter threshold after line coding operations

## How It Works

1. Line coding operations are performed first (decode_uart, decode_dm, or decode_mc)
2. Each row/burst is checked against the minimum and/or maximum length
3. **Rows outside the specified range are removed** from the results
4. Only qualifying rows are processed and output
5. If no rows meet the criteria, the entire message is discarded

## Examples

### Filter by Minimum Length

Require at least 48 bits after Manchester decoding:

```bash
-X "n=sensor,m=OOK_PWM,s=400,l=800,r=7000,decode_mc,min_bits_decode=48"
```

This will:
- Decode Manchester coding (which halves the bit count)
- Filter out any results with less than 48 bits after decoding
- Keep only bursts with 48+ bits in the output

### Filter by Maximum Length

Limit to maximum 128 bits after UART decoding:

```bash
-X "n=device,m=OOK_PCM,s=500,l=1000,r=5000,decode_uart,max_bits_decode=128"
```

This will:
- Decode UART 8n1 format (converts 10 bits to 8 bits)
- Only keep bursts that have at most 128 bits after UART decoding
- Discard longer bursts

### Filter by Length Range

Accept only messages between 64 and 128 bits after decoding:

```bash
-X "n=device,m=OOK_PCM,s=500,l=1000,r=5000,decode_uart,min_bits_decode=64,max_bits_decode=128"
```

This will:
- Decode UART 8n1 format
- Only keep bursts between 64 and 128 bits (8-16 bytes)
- Discard bursts shorter than 64 bits or longer than 128 bits

### Mixed Length Bursts

When processing a cu8 file with multiple bursts of different lengths:

```bash
-X "n=mixed,m=OOK_PWM,s=300,l=600,r=8000,decode_dm,min_bits_decode=40"
```

If the capture contains:
- Burst 1: 100 bits raw → 50 bits after DM decode ✓ (kept)
- Burst 2: 60 bits raw → 30 bits after DM decode ✗ (filtered out)
- Burst 3: 80 bits raw → 40 bits after DM decode ✓ (kept)

Only bursts 1 and 3 will appear in the output.

## Combining with Other Parameters

You can combine `min_bits_decode` and `max_bits_decode` with other flex parameters:

```bash
-X "n=secure_device,\
    m=OOK_PWM,s=400,l=800,r=7000,\
    bits>=96,\
    decode_mc,\
    min_bits_decode=48,\
    max_bits_decode=80,\
    match={8}0xAA,\
    get=8@{16}:device_id"
```

This will:
1. Require raw input of at least 96 bits (`bits>=96`)
2. Decode Manchester coding (`decode_mc`)
3. Filter to keep only results between 48-80 bits after decoding
4. Match 0xAA pattern in the decoded data (`match`)
5. Extract device_id from the decoded data (`get`)

## Use Cases

1. **Variable Length Protocols**: Filter out incomplete or truncated messages (use `min_bits_decode`)
2. **Noise Rejection**: Discard short spurious signals that pass initial filters (use `min_bits_decode`)
3. **Prevent Buffer Overflow**: Reject abnormally long messages (use `max_bits_decode`)
4. **Exact Message Size**: Accept only messages of a specific size range (use both)
5. **Multi-Burst Captures**: Process only complete messages from mixed-length captures
6. **After Line Coding**: Ensure decoded payload meets size requirements
7. **Quality Control**: Guarantee data is within expected size bounds

## Notes

- The filters are applied **after** line coding operations (decode_uart, decode_dm, decode_mc)
- The filters are applied **before** trim_leading, match_offset, and data extraction
- Each row/burst is evaluated individually
- Rows are completely removed from output if they don't meet the criteria
- If all rows are filtered out, returns DECODE_ABORT_LENGTH
- Values are specified in **bits**, not bytes
- Use value of 0 (or omit parameter) to disable filtering
- Both parameters can be used together to create a length range


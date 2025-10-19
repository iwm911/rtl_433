# Flex Decoder: min_bits_decode Parameter

## Overview

The `min_bits_decode` parameter allows you to filter out decoded results that are shorter than a specified length **after line coding operations** (UART decode, Manchester decode, or Differential Manchester decode).

This is particularly useful when working with captures that contain multiple bursts of varying lengths, where you only want to process messages that have enough data after decoding.

## Difference from `bits` Parameter

- **`bits=<n>`** - Filters based on the **raw bit length before** line coding/decoding
- **`min_bits_decode=<n>`** - Filters based on the **decoded bit length after** line coding/decoding

Since line coding operations like Manchester and UART decoding reduce the bit count (e.g., Manchester typically halves the bit count, UART converts 10 bits to 8), you need `min_bits_decode` to filter on the final decoded length.

## Syntax

```
min_bits_decode=<n>
```

- `<n>`: Minimum number of bits required after line coding operations

## How It Works

1. Line coding operations are performed first (decode_uart, decode_dm, or decode_mc)
2. Each row/burst is checked against the minimum length
3. **Rows that don't meet the minimum are removed** from the results
4. Only qualifying rows are processed and output
5. If no rows meet the minimum, the entire message is discarded

## Examples

### Filter Manchester Decoded Messages

Require at least 48 bits after Manchester decoding:

```bash
-X "n=sensor,m=OOK_PWM,s=400,l=800,r=7000,decode_mc,min_bits_decode=48"
```

This will:
- Decode Manchester coding (which halves the bit count)
- Filter out any results with less than 48 bits after decoding
- Keep only bursts with 48+ bits in the output

### Filter UART Decoded Messages

Require at least 64 bits (8 bytes) after UART decoding:

```bash
-X "n=device,m=OOK_PCM,s=500,l=1000,r=5000,decode_uart,min_bits_decode=64"
```

This will:
- Decode UART 8n1 format (converts 10 bits to 8 bits)
- Only keep bursts that have at least 64 bits after UART decoding
- Discard shorter bursts

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

You can combine `min_bits_decode` with other flex parameters:

```bash
-X "n=secure_device,\
    m=OOK_PWM,s=400,l=800,r=7000,\
    bits>=96,\
    decode_mc,\
    min_bits_decode=48,\
    match={8}0xAA,\
    get=8@{16}:device_id"
```

This will:
1. Require raw input of at least 96 bits (`bits>=96`)
2. Decode Manchester coding (`decode_mc`)
3. Filter to keep only results with 48+ bits after decoding (`min_bits_decode=48`)
4. Match 0xAA pattern in the decoded data (`match`)
5. Extract device_id from the decoded data (`get`)

## Use Cases

1. **Variable Length Protocols**: Filter out incomplete or truncated messages
2. **Noise Rejection**: Discard short spurious signals that pass initial filters
3. **Multi-Burst Captures**: Process only complete messages from mixed-length captures
4. **After Line Coding**: Ensure decoded payload meets minimum size requirements
5. **Quality Control**: Guarantee sufficient data for downstream processing

## Notes

- The filter is applied **after** line coding operations (decode_uart, decode_dm, decode_mc)
- The filter is applied **before** trim_leading, match_offset, and data extraction
- Each row/burst is evaluated individually
- Rows are completely removed from output if they don't meet the minimum
- If all rows are filtered out, returns DECODE_ABORT_LENGTH
- Value is specified in **bits**, not bytes
- Use value of 0 (or omit parameter) to disable filtering


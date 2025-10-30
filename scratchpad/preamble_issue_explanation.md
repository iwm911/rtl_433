# Flex Decoder Preamble Issue Explanation

## Problem Summary

The flex decoder works fine without a preamble but fails when a preamble is specified because of the **order of operations** in the decoder.

## The Issue

Looking at the flex decoder callback function in `src/devices/flex.c`, the operations happen in this order:

1. **Line 258-278: Preamble matching** - searches for the preamble in the raw bitbuffer
2. **Line 280-294: Symbol decoding** (if configured)
3. **Line 296-305: UART decoding** (if configured)
4. **Line 307-317: Differential Manchester decoding** (if configured)
5. **Line 319-330: Manchester decoding** (if configured)

## Why It Fails

When you run the decoder **without preamble**:
```bash
rtl_433 -X n=decode_dm,m=FSK_PCM,s=48,l=48,r=1000,decode_dm,get=device_id:@:{32}:%x
```

1. Raw FSK bits are received
2. `decode_dm` (Differential Manchester) is applied
3. Result: `0001039d159d150140014a0` ✓

When you run the decoder **with preamble**:
```bash
rtl_433 -X n=decode_dm,m=FSK_PCM,s=48,l=48,r=1000,decode_dm,preamble=000103,get=device_id:@:{32}:%x
```

1. Raw FSK bits are received (still in Differential Manchester encoding)
2. **Preamble search happens BEFORE decode_dm**
3. The code searches for `000103` in the **encoded** bits
4. Since the bits are still Differential Manchester encoded, the pattern `000103` doesn't match
5. Returns `DECODE_FAIL_SANITY` - no output ✗

## The Root Cause

```c
// Line 258-278: Preamble check happens HERE
if (params->preamble_len) {
    // Searches in the raw, un-decoded bitbuffer
    unsigned pos = bitbuffer_search(bitbuffer, i, 0, params->preamble_bits, params->preamble_len);
    // ...
}

// Line 307-317: But decode_dm happens LATER
if (params->decode_dm) {
    // Differential Manchester decode happens here
    bitbuffer_differential_manchester_decode(bitbuffer, i, 0, &tmp, len);
    // ...
}
```

The preamble is specified as the **decoded** hex pattern `000103`, but it's being searched for in the **encoded** bitstream.

## The Fix

The preamble matching should happen **after** line coding (decode_dm, decode_mc, decode_uart) is applied, not before.

The correct order should be:
1. Raw bits received
2. Apply line coding (decode_dm, decode_mc, decode_uart)
3. **Then** search for preamble in decoded data
4. Apply getters and output results

## Solution

Move the preamble matching code block (lines 258-278) to after all the line coding blocks (after line 330).



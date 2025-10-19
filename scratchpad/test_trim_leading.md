# Testing the trim_leading Option

## Overview
The `trim_leading` option has been added to the flex decoder to remove leading repeated bytes (0xFF or 0x00) after line coding operations like Manchester decoding, Differential Manchester decoding, or UART decoding.

## Usage

To use the `trim_leading` option, add it to your flex decoder specification:

```bash
rtl_433 -X "n=mydevice,m=FSK_MC_ZEROBIT,s=70,l=0,r=1800,trim_leading"
```

## How It Works

After line coding operations (decode_uart, decode_dm, decode_mc), the buffer might contain leading padding bytes like:
- `FFFFFF915915f` - leading 0xFF bytes
- `000000915915f` - leading 0x00 bytes

When `trim_leading` is enabled:
1. The decoder checks if the first byte is 0xFF or 0x00
2. It counts consecutive leading bytes of the same value
3. It removes all leading repeated bytes (keeping at least one byte if entire buffer is the same)
4. The buffer is shifted and bit count is updated

### Example Without trim_leading:
```
Data: FFFFFF915915f
Bits: 104
```

### Example With trim_leading:
```
Data: 915915f
Bits: 56
```

## Command Line Example

For your specific decoder:

### Without trim_leading:
```bash
rtl_433 -r signal.cu8 -R 0 -X 'n=FSK_MC_ZEROBIT_s70_l0_r1800_preamble_4001,m=FSK_MC_ZEROBIT,s=70,l=0,r=1800,preamble=4001,get=id:@0:{32}:%x'
```

### With trim_leading:
```bash
rtl_433 -r signal.cu8 -R 0 -X 'n=FSK_MC_ZEROBIT_s70_l0_r1800_preamble_4001,m=FSK_MC_ZEROBIT,s=70,l=0,r=1800,preamble=4001,trim_leading,get=id:@0:{32}:%x'
```

## Configuration File Format

You can also use it in a `.conf` file:

```
decoder {
    name=MyDevice,
    modulation=FSK_MC_ZEROBIT,
    short=70,
    long=0,
    reset=1800,
    preamble=4001,
    trim_leading,
    get=id:@0:{32}:%x,
}
```

## Implementation Details

The feature is implemented in `src/devices/flex.c`:
- Added `trim_leading` field to `flex_params` structure
- Processing happens after all line coding operations (UART, DM, MC)
- Only trims leading 0xFF or 0x00 bytes
- Preserves at least one byte to avoid empty buffers
- Updates both the buffer content and the bit count

## When to Use

Use `trim_leading` when:
- Your device adds padding bytes before the actual data
- Manchester/UART decoding produces leading 0xFF or 0x00 bytes
- You need to extract data that starts after a variable number of padding bytes
- The preamble removal isn't sufficient for your use case


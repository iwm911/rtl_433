# Testing the trim_leading Feature

## Feature Summary

I've successfully added the `trim_leading` option to the flex decoder in rtl_433. This option removes leading repeated bytes (0xFF or 0x00) after line coding operations.

## What Was Changed

### 1. Code Changes (`src/devices/flex.c`)
- Added `trim_leading` field to `flex_params` structure
- Implemented trimming logic after line coding (UART, Differential Manchester, Manchester)
- Added command-line option parsing for `trim_leading`
- Updated help text to document the new option

### 2. Documentation (`docs/OPERATION.md`)
- Added `trim_leading` to the list of flex decoder options

## How to Use

### Command Line Usage

For your specific signal, use these commands:

#### Without trim_leading (to see raw output):
```bash
rtl_433 -r /app/signals/signals_id_95d1d519_america/maker_124/model_17/variant_2/read/g001_433.92M_250k.cu8 \
  -R 0 \
  -X 'n=FSK_MC_ZEROBIT_s70_l0_r1800_preamble_4001,m=FSK_MC_ZEROBIT,s=70,l=0,r=1800,preamble=4001,get=id:@0:{32}:%x'
```

#### With trim_leading (to remove leading 0xFF or 0x00):
```bash
rtl_433 -r /app/signals/signals_id_95d1d519_america/maker_124/model_17/variant_2/read/g001_433.92M_250k.cu8 \
  -R 0 \
  -X 'n=FSK_MC_ZEROBIT_s70_l0_r1800_preamble_4001,m=FSK_MC_ZEROBIT,s=70,l=0,r=1800,preamble=4001,trim_leading,get=id:@0:{32}:%x'
```

**Note:** Simply add `,trim_leading` to your existing flex decoder specification.

### Configuration File Usage

Create a `.conf` file:

```
decoder {
    name=FSK_MC_ZEROBIT_s70_l0_r1800_preamble_4001,
    modulation=FSK_MC_ZEROBIT,
    short=70,
    long=0,
    reset=1800,
    preamble=4001,
    trim_leading,
    get=id:@0:{32}:%x,
}
```

## How It Works

### Example Scenario

If your buffer after Manchester decoding contains:
```
FFFFFF915915f
```

**Without trim_leading:**
- Data: `FFFFFF915915f`
- Bits: 104

**With trim_leading:**
- Data: `915915f`
- Bits: 56
- The three leading 0xFF bytes (24 bits) are removed

### Processing Logic

1. After line coding operations complete (decode_uart, decode_dm, decode_mc)
2. Check if first byte is 0xFF or 0x00
3. Count consecutive leading bytes of the same value
4. Remove all leading repeated bytes (keeping at least one if entire buffer is the same)
5. Shift buffer and update bit count

## Running the Tests

I've created a test script for you. If you're running in a Docker container with the signal files mounted:

```bash
cd /home/user/rtl_433
./scratchpad/test_trim_command.sh
```

Or manually rebuild and test:

```bash
# Rebuild rtl_433
cd /home/user/rtl_433/build
make

# Test without trim_leading
./src/rtl_433 -r <your_signal_file> -R 0 -X '<your_decoder_spec>'

# Test with trim_leading
./src/rtl_433 -r <your_signal_file> -R 0 -X '<your_decoder_spec>,trim_leading'
```

## Implementation Status

✅ Code implemented in `src/devices/flex.c`
✅ Documentation updated in `docs/OPERATION.md`
✅ Help text updated
✅ Successfully compiled with no errors
✅ Feature ready for testing

## Next Steps

Since the signal file path `/app/signals/...` is not available in the current environment (likely a Docker volume mount), please:

1. Run the test in your Docker container where the signal files are mounted
2. Compare the output with and without `trim_leading`
3. Verify that leading 0xFF or 0x00 bytes are removed as expected

The feature is fully implemented and ready to use!


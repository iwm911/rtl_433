#!/bin/bash

# Test script for trim_leading feature
# This demonstrates how to use the trim_leading option with your signal file

echo "=== Testing trim_leading Feature ==="
echo ""

# Path to your signal file
SIGNAL_FILE="/app/signals/signals_id_95d1d519_america/maker_124/model_17/variant_2/read/g001_433.92M_250k.cu8"

# Your flex decoder spec
DECODER_BASE="n=FSK_MC_ZEROBIT_s70_l0_r1800_preamble_4001,m=FSK_MC_ZEROBIT,s=70,l=0,r=1800,preamble=4001,get=id:@0:{32}:%x"

# Check if signal file exists
if [ ! -f "$SIGNAL_FILE" ]; then
    echo "ERROR: Signal file not found: $SIGNAL_FILE"
    echo "Please ensure you're running this in a container with the signal files mounted."
    echo ""
    echo "Usage example once file is available:"
    echo ""
    echo "1. Without trim_leading (raw output with potential leading bytes):"
    echo "   rtl_433 -r \"$SIGNAL_FILE\" -R 0 -X \"$DECODER_BASE\""
    echo ""
    echo "2. With trim_leading (removes leading 0xFF or 0x00 bytes):"
    echo "   rtl_433 -r \"$SIGNAL_FILE\" -R 0 -X \"${DECODER_BASE},trim_leading\""
    echo ""
    exit 1
fi

echo "Signal file found: $SIGNAL_FILE"
echo ""

echo "=== Test 1: WITHOUT trim_leading ==="
echo "Command: rtl_433 -r \"$SIGNAL_FILE\" -R 0 -X \"$DECODER_BASE\""
echo ""
rtl_433 -r "$SIGNAL_FILE" -R 0 -X "$DECODER_BASE"

echo ""
echo "=== Test 2: WITH trim_leading ==="
echo "Command: rtl_433 -r \"$SIGNAL_FILE\" -R 0 -X \"${DECODER_BASE},trim_leading\""
echo ""
rtl_433 -r "$SIGNAL_FILE" -R 0 -X "${DECODER_BASE},trim_leading"

echo ""
echo "=== Comparison ==="
echo "Compare the 'data' field in both outputs to see the leading bytes removed."
echo "The bit count should also be reduced accordingly."


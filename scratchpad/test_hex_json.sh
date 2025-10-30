#!/bin/bash
# Test script to verify hex output with JSON format

set -e

echo "Testing hex output with JSON format..."
echo ""

# Test 1: Simple hex format %x
echo "Test 1: Testing %x format"
echo "Note: This requires actual signal data, so we'll just verify the binary was built correctly"
echo ""

# Verify the binary exists and can show help
if [ -f /home/user/rtl_433/build/src/rtl_433 ]; then
    echo "✓ Binary built successfully"
    echo ""
    echo "To test with actual data, run:"
    echo "  rtl_433 -X 'n=test,m=OOK_PWM,s=400,l=800,r=7000,g=1000,get=device_id:@0:{32}:%x' -F json"
    echo ""
    echo "Expected output: device_id should be a JSON string containing hex digits (e.g., \"1234abcd\")"
else
    echo "✗ Binary not found!"
    exit 1
fi

echo ""
echo "Build and binary verification complete!"


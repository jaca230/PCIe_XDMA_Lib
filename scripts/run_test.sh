#!/bin/bash

# Get the directory of the script
SOURCE="${BASH_SOURCE[0]}"
while [ -L "$SOURCE" ]; do
    DIR=$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )
    SOURCE=$(readlink "$SOURCE")
    [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
done
script_directory=$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )

# Default values
device_prefix="/dev/xdma0"
address="0x00000000"
size="128"
channel_index="0"

# Function to display help
print_help() {
    echo "Usage: $0 [options]"
    echo "Options:"
    echo "  -p, --device-prefix     Prefix for the device (default: $device_prefix)"
    echo "  -a, --address           Address to read/write (default: $address)"
    echo "  -s, --size              Size of the data (default: $size)"
    echo "  -i, --channel-index     Channel index to use (default: $channel_index)"
    echo "  -h, --help              Display this help message"
}

# Parse command-line options
while [[ "$1" != "" ]]; do
    case $1 in
        -p | --device-prefix )
            shift
            device_prefix=$1
            ;;
        -a | --address )
            shift
            address=$1
            ;;
        -s | --size )
            shift
            size=$1
            ;;
        -i | --channel-index )
            shift
            channel_index=$1
            ;;
        -h | --help )
            print_help
            exit
            ;;
        * )
            echo "Unknown option: $1"
            print_help
            exit 1
            ;;
    esac
    shift
done

# Call the PCIe_DMA_Readout executable
"$script_directory/../bin/PCIe_DMA" "$device_prefix" "$address" "$size" "$channel_index"

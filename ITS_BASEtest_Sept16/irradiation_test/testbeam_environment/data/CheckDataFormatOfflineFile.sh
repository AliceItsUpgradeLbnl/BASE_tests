# This script executes in a sequence all the checks on a offline data file for tests at the BASE facility

function quit {
    echo "Exiting ..."
    echo ""
    exit $rc;  
}

function check_status {
    rc=$?; if [[ $rc != 0 ]]; then
      quit
    fi  
}

# Check number of argument passed 
if [ "$#" -ne 1 ]; then
    echo ""
    echo "Error: illegal number of parameters for offline data checker script"
    echo "Syntax: $0 file_to_check. Example: $0 offline_data.dat"
    echo ""
    quit
fi

OfflineDataFile=$1

# Clear screen
printf "\033c"

echo ""
echo "****************************************************"
echo "Starting checks on offline data file \"$1\""
echo "****************************************************"
echo ""

# Running the checks for the 9 offline test in sequence
./dataformat_checker $OfflineDataFile differential_amplifiers
check_status

./dataformat_checker $OfflineDataFile comparator
check_status

./dataformat_checker $OfflineDataFile dacs
check_status

./dataformat_checker $OfflineDataFile resistance_to_digital
check_status

./dataformat_checker $OfflineDataFile io_expander
check_status

./dataformat_checker $OfflineDataFile adcs
check_status

./dataformat_checker $OfflineDataFile i2c_driver
check_status

./dataformat_checker $OfflineDataFile differential_to_single_ended_driver
check_status

./dataformat_checker $OfflineDataFile dcdc_converters
check_status

echo ""
echo "****************************************************"
echo "Checks on the offline data were successful!!!!"
echo "****************************************************"
echo ""

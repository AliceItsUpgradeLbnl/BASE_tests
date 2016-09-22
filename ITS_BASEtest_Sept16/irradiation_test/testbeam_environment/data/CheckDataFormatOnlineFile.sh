# This script executes in a sequence all the checks on a online data file for tests at the BASE facility

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

OnlineDataFile=$1 

# Clear screen
printf "\033c"

echo ""
echo "****************************************************"
echo "Starting checks on the online data"
echo "****************************************************"
echo ""

# Running the checks for the 9 offline test in sequence
./dataformat_checker $OnlineDataFile online_monitoring
check_status

echo ""
echo "****************************************************"
echo "Checks on the online data were successful!!!!"
echo "****************************************************"
echo ""

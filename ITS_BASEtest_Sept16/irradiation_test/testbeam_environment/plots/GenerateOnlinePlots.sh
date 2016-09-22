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

function online_monitoring {
    ./data_analysis $onlineDataFile online_monitoring dose_test0
    check_status

    ./data_analysis $onlineDataFile online_monitoring dose_test1
    check_status

    ./data_analysis $onlineDataFile online_monitoring dose_test2
    check_status

    ./data_analysis $onlineDataFile online_monitoring dose_test3
    check_status
}

# Check number of argument passed 
if [ "$#" -ne 1 ]; then
    echo ""
    echo "Error: illegal number of parameters for online data checker script"
    echo "Syntax: $0 file_to_check. Example: $0 online_data.dat"
    echo ""
    quit
fi

onlineDataFile=$1

# Clear screen
printf "\033c"

echo ""
echo "****************************************************"
echo "Generating online plots with data from file \"$1\""
echo "****************************************************"
echo ""

# Generating the plots for the online data
online_monitoring

echo ""
echo "****************************************************"
echo "Plot generation successful!!!!"
echo "****************************************************"
echo ""

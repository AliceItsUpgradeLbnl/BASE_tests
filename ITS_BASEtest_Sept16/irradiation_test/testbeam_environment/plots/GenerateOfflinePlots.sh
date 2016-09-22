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

function differential_amplifiers {
    ./data_analysis $OfflineDataFile differential_amplifiers voltage_scan
    check_status
}

function comparator {
    ./data_analysis $OfflineDataFile comparator voltage_scan
    check_status
}

function dacs {
    ./data_analysis $OfflineDataFile dacs voltage_scan
    check_status
}

function resistance_to_digital {
    ./data_analysis $OfflineDataFile resistance_to_digital resistance_scan
    check_status
}

function io_expander {
    ./data_analysis $OfflineDataFile io_expander status_check0
    check_status

    ./data_analysis $OfflineDataFile io_expander status_check1
    check_status
}

function adcs {
    ./data_analysis $OfflineDataFile adcs voltage_scan0
    check_status

    ./data_analysis $OfflineDataFile adcs voltage_scan1
    check_status

    ./data_analysis $OfflineDataFile adcs voltage_scan2
    check_status
}

function i2c_driver {
    ./data_analysis $OfflineDataFile i2c_driver transaction_failurerate
    check_status
}

function differential_to_single_ended_driver {
    ./data_analysis $OfflineDataFile differential_to_single_ended_driver biterror_rate
    check_status
}

function dcdc_converters {
    ./data_analysis $OfflineDataFile dcdc_converters status_check0
    check_status

    ./data_analysis $OfflineDataFile dcdc_converters status_check1
    check_status
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
echo "Generating offline plots with data from file \"$1\""
echo "****************************************************"
echo ""

# Generating the plots for the offline data
differential_amplifiers

comparator

dacs

resistance_to_digital

io_expander

adcs

i2c_driver

differential_to_single_ended_driver

dcdc_converters

echo ""
echo "****************************************************"
echo "Plot generation successful!!!!"
echo "****************************************************"
echo ""

##################################################################################
# This script executes the online test and performes a check of the acquired data
##################################################################################

# Asking if to load the firmware ....
text="" 
while true
do 
  echo -n "Do you want to load the firmware? y for yes, n to quit: "
  read text
  if [ "$text" == "n" ]; then
    break
  elif [ "$text" == "y" ]; then
    ./firmware/scripts/loadfirmware.sh
    break
  else
    echo "Wrong answer... Try Again!"
  fi
done

# Executing online test
./onlineTest.py

# Getting name of online file just created 
cd data/
ls -lt online*.dat > list_of_online_files
last_online_file_info=$(head -n 1 list_of_online_files)
last_online_file_name="${last_online_file_info##* }"

# Asking if to check acquired data ....
text="" 
while true
do 
  echo -n "Do you want to check the format of the acquired data? y for yes, n to quit: "
  read text
  if [ "$text" == "n" ]; then
    break
  elif [ "$text" == "y" ]; then
    ./CheckDataFormatOnlineFile.sh $last_online_file_name 
    break
  else
    echo "Wrong answer... Try Again!"
  fi
done

# Asking if to generate plots ....
text="" 
while true
do 
  echo -n "Do you want to generate the plots? y for yes, n to quit: "
  read text
  if [ "$text" == "n" ]; then
    exit
  elif [ "$text" == "y" ]; then
    echo "I will generate the plots!"
    break
  else
    echo "Wrong answer... Try Again!"
  fi
done

# Getting board name and dose step and creating/accessing relative folders
board_name=$(echo "$last_online_file_name" | awk -F '_' '{print $2}' )
dose_step=$(echo "$last_online_file_name" | awk -F '_' '{print $3}' )

cd ../plots/

# Creating board dir 
if [ -d $board_name ]; then
  cd $board_name
else
  mkdir $board_name
  cd $board_name
fi

echo "$board_name"

# Creating dose folder
if [ -d $dose_step ]; then
  cd $dose_step
else
  mkdir $dose_step
  cd $dose_step
fi

echo "$dose_step"

cp ../../GenerateOnlinePlots.sh .
cp ../../data_analysis .

./GenerateOnlinePlots.sh ../../../data/$last_online_file_name

rm GenerateOnlinePlots.sh
rm data_analysis

cd ../../../

rm ./data/list_of_online_files

#!/bin/bash

diseasesFile=$1
countriesFile=$2
input_dir=$3
numFilesPerDirectory=$4
numRecordsPerFile=$5

#checks if all arguments are given. If not, exit.
if [ "$#" -ne 5 ]; then     
    echo "No valid number of arguments supplied"; exit -1
fi

#checks if numFilesPerDirectory is valid. If not, exit.
[ -n "$4" ] && [ "$4" -eq "$4" ] && [ $4 -gt 0 ] 2>/dev/null    
if [ $? -ne 0 ]; then
    echo "numFilesPerDirectory input is either not a number or <0"; exit -1
fi

#checks if numRecordsPerFile is valid. If not, exit.
[ -n "$5" ] && [ "$5" -eq "$5 " ] && [ $5 -gt 0 ] 2>/dev/null   
if [ $? -ne 0 ]; then
    echo "numRecordsPerFile input is either not a number or <0"; exit -1
fi

mkdir $input_dir
while read line; do
    mkdir $input_dir/$line
    for ((i=1; i<=$numFilesPerDirectory; i++))
    do  
        #random number for days
        daysfloor=1
        daysrange=30
        days=0
        while [ "$days" -le $daysfloor ]            
        do
            days=$RANDOM
            let "days %= $daysrange"
        done
        echo "random day: $days"

        #random number for months
        monthsfloor=1
        monthsrange=12
        months=0
        while [ "$months" -le $monthsfloor ]    
        do
            months=$RANDOM
            let "months %= $monthsrange"
        done    
        echo "random month: $months"

        #random number for years
        yearsfloor=1990
        yearsrange=2020
        years=0
        while [ "$years" -le $yearsfloor ]  
        do
            years=$RANDOM
            let "years %= $yearsrange"
        done
        echo "random year: $years"

        #create file
        filename="$days-$months-$years"    
        touch $input_dir/$line/$filename

        for ((m=1; m<=$numRecordsPerFile; m++))
        do 
            #random digits for id
            idfloor=0
            idrange=999
            id=0
            while [ "$id" -le $idfloor ]  
            do
                id=$RANDOM
                let "id %= $idrange"
            done
            echo "random id: $id"

            #binary random for ENTER or EXIT
            BINARY=2
            T=1
            num=$RANDOM
            let "num %= $BINARY"    
            if [ "$num" -eq $T ]
            then
                nos=ENTER
            else
                nos=EXIT
            fi
            echo "$nos"

            #random number for age
            agefloor=1
            agerange=120
            age=0
            while [ "$age" -le $agefloor ]  
            do
                age=$RANDOM
                let "age %= $agerange"
            done
            echo "random age: $age"

            #random disease from diseaseFile
            disease=$(shuf $diseasesFile -n 1)  
            echo $disease

            #the implementation of both name and last name was studied and 
            #inspired by https://gist.github.com/earthgecko/3089509

            #random name 
            name=$(cat /dev/urandom | tr -dc 'A-Za-z' | fold -w 12 | head -n 1) 
            echo $name

            #random lastname
            lastname=$(cat /dev/urandom | tr -dc 'A-Za-z' | fold -w 12 | head -n 1)  
            echo $lastname

            #write in file
            echo "$id $nos $name $lastname $disease $age" >> $input_dir/$line/$filename 
        done
    done
done < $countriesFile
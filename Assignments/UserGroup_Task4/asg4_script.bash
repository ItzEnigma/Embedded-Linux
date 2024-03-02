#!/bin/bash

echo "Running Script ..."

#echo "Enter the new user name"
read -p "Enter the new user name: " usrName

if [ $( grep -q $usrName /etc/passwd) ]
then
    echo "User Already Exists!"
    exit
else
    read -p "Enter the new group name: " grpName
    if grep -q "$grpName" /etc/group
    then
        echo "Group Already Exists"
    else
	sudo groupadd $grpName
    fi
      # -m --> Create new Home directory for the new user and copy the contents of /etc/skel/* (.bashrc & .profile ..)
      # -d --> Change the home directory to a custom directory ... (This is done because home directory is chosen
	# incorrectly causing issue for the .bashrc ...etc.)
 	 sudo useradd -m -d /home/$usrName $usrName
        echo "Enter User Password:"
        sudo passwd $usrName
        sudo usermod -aG "$grpName" "$usrName"
        echo "User: $usrName is sucessfully added to Group: $grpName"

        echo "User Information"
        id $usrName

        echo "Group Information"
        getent group $grpName
fi

#echo $usrName



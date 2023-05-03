 # !/usr/bin/env bash
figlet -c -k -w 80 SOCKET PROGRAMMING IN C 
date "+DATE: %D%nTIME: %T"
echo -e "\e[1;42m  a.) Group Chat Application (Multi-user/Multi-Threading) \e[0m"
echo -e "\e[1;42m  b.) Client / Server (Calculator) \e[0m "
echo -e "\e[1;42m  c.) Client / Server (File Transfering) \e[0m "
echo -e "\e[1;42m  d.) Single Client / Server \e[0m "
echo -e "\e[1;42m  e.) Exit \e[0m" 

read -p "Enter Choice(a to d):" choice

case $choice in
        "a"|"A")
                echo -e "\e[1;42m Accessing Group Chat Application....\e[0m"
                sleep 2
                cd /home/qasim/Desktop/OS_PROJECT/GroupChatApplication
                $SHELL
                ;;
        "b"|"B")
                echo -e "\e[1;42m  Accessing Client/Server Calculator....\e[0m"
                sleep 2
                cd /home/qasim/Desktop/OS_PROJECT/Calculator
                $SHELL
                ;;
        "c"|"C")
                echo -e "\e[1;42m Accessing Client/Server File Transfering....\e[0m"
                sleep 2
                cd /home/qasim/Desktop/OS_PROJECT/File_Transfer
                $SHELL
                ;;
        "d"|"D")
                echo -e "\e[1;42m Single Client/Server....\e[0m"
                sleep 2
                cd /home/qasim/Desktop/OS_PROJECT/Single_Client_to_Server
                $SHELL
               ;;
        "e"|"E")
                echo -e "\e[1;41m Program Terminated \e[0m" 
esac

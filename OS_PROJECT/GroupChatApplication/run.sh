konsole --noclose -e ./server 3210 &
sleep 7
konsole --noclose -e ./client 192.168.196.131 3210 qasim &
sleep 9
konsole --noclose -e ./client 192.168.196.131 3210 suffiyan &
exit

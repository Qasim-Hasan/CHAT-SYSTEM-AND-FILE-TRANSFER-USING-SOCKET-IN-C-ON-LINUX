konsole --noclose -e ./server 3210 &
sleep 3
konsole --noclose -e ./client 192.168.222.128 3210 qasim &
sleep 5
konsole --noclose -e ./client 192.168.222.128 3210 suffiyan &
exit



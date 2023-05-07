konsole --noclose -e ./server 3186 &
sleep 3
konsole --noclose -e ./client 127.0.0.1 3186 &
exit


konsole --noclose -e ./server 3210 &
sleep 2
konsole --noclose -e ./client 127.0.0.1 3210 &
exit



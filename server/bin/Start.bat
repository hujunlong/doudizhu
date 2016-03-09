echo "start loginServer.exe"
start /min "loginServer" "loginServer.exe"

ping -n 1 127.0>nul
echo "start gameServer"
start /min gameServer "gameServer.exe"


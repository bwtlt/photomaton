#!/usr/bin/env sh

echo "Exporting GPIO"

gpio export 14 in
gpio export 15 in
gpio export 18 in
gpio export 23 in

gpio -g mode 14 down
gpio -g mode 15 down
gpio -g mode 18 down
gpio -g mode 23 down


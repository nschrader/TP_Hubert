#!/bin/bash
xdg-terminal ./hubert &
sleep 1

xdg-terminal "./restaurant menus/cafeteria.txt" &
xdg-terminal "./restaurant menus/breton.txt" &

xdg-terminal ./client

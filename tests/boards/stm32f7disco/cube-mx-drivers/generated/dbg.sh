#!/bin/bash

# https://github.com/autolycus/FreeRTOS-GDB.git
export PYTHONPATH=~/FreeRTOS-GDB/src/

session=gdb
window=${session}:0
pane_gdb=${window}.1
pane_db=${window}.0

# spawn a tmux detached session
tmux new -d -s $session

# split in two panes
tmux split-window -v

# spawn gdb in first pane
tmux send-keys -t "$pane_gdb" "arm-none-eabi-gdb build/generated.elf" Enter


tmux send-keys -t "$pane_gdb" " " Enter
# connecd gdb to OCD
tmux send-keys -t "$pane_gdb" "target remote localhost:3333" Enter

# configure dashboard
tmux send-keys -t "$pane_gdb" "db threads" Enter
tmux send-keys -t "$pane_gdb" "db assembly" Enter

# source FreeRTOS awareness
#tmux send-keys -t "$pane_gdb" "source ~/FreeRTOS-GDB/src/FreeRTOS.py" Enter
# source SVD peripheral file parser
tmux send-keys -t "$pane_gdb" "source ~/PyCortexMDebug/cmdebug/svd_gdb.py" Enter
tmux send-keys -t "$pane_gdb" "svd_load /home/andrea/SVD/STM32F7x.svd" Enter

# get tty for the 2nd pane and wait for command to complete
tmux send-keys -t "$pane_db" "tty" Enter
sleep 0.5
pts=`tmux capture-pane -t "$pane_db" -p |grep pts`

# clear screen
tmux send-keys -t "$pane_db" C-l

# set dashboard output to the 2nd pane
tmux send-keys -t "$pane_gdb" "db -output $pts" Enter

# clear screen
tmux send-keys -t "$pane_gdb" C-l

# attach
tmux -2 attach-session -t $session

tmux kill-session -t $session

#!/usr/bin/expect
set timeout 30
spawn telnet
expect "telnet>"
send "open localhost 4444\n"
expect ">"
send "reset halt\n"
expect ">"
#send "stm32l4x mass_erase 0\n"
#expect ">"
#send "reset halt\n"
#expect ">"
send "program build/generated.elf\n"
expect ">"
send "reset\n"

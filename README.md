# esp32-ewn-box-opener
Esp32 port of the Box Opener client for mining EWN tokens.

Original Box Opener client written in python: https://github.com/Erwin-Schrodinger-Token/ewn-box-opener

For easy import I suggest using VSCode with PlatformIO plugin.

This projest uses ciband/bip39 library and esp32 hardware random number generator to create mnemonics.

This is a work in progress. There are more features to be added (like GUI to set wifi credencials or the API key).
But for now it should just work.

## Importing project:

Download whole repository to your disk (the green "Code" button somewhere in upper right-> Download ZIP).

Extract the whole **esp32-ewn-box-opener-main** direcotry to your disk.

Open VSCode 

## Setting up.
Get your API key from:

Mainnet - https://erwin.lol/box-opener

Devnet - https://devnet.erwin.lol/box-opener

Put it in the apiKey variable in main.cpp file.

Set up your wifi credentials (ssid and password variable).

Compile and upload to your board.

## Running.
Open serial terminal with 115200 bps baudrate and connec to the esp32 board.

If everything is OK you should see something like this:
```
Connecting to WiFi ...
192.168.145.183
===============
Box-opener started
⚙️ Generating guesses...
🔑️ Generated 50 guesses
➡️ Submitting to oracle
✅ Guesses accepted
waiting 10s for next batch...
⚙️ Generating guesses...
🔑️ Generated 50 guesses
➡️ Submitting to oracle
✅ Guesses accepted
waiting 10s for next batch...
⚙️ Generating guesses...
🔑️ Generated 50 guesses
➡️ Submitting to oracle
✅ Guesses accepted
waiting 10s for next batch...
```

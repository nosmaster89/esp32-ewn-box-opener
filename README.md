# esp32-ewn-box-opener
Esp32 port of the Box Opener client for mining EWN tokens.

Original Box Opener client written in python: https://github.com/Erwin-Schrodinger-Token/ewn-box-opener

For easy import I suggest using VSCode with PlatformIO plugin.

This projest uses ciband/bip39 library and esp32 hardware random number generator to create mnemonics.

**>>I tested this code on T-Display-S3 board but it should run on others.<<**

This is a work in progress. There are more features to be added (like GUI to set wifi credencials or the API key).
But for now it should just work.

## Importing project.
**The simplest way is to have VSCode (Visual Studio Code editor) installed with PlatformIO extension on your computer.**

Download whole repository to your disk (the green "Code" button somewhere in upper right-> Download ZIP).

Extract the **esp32-ewn-box-opener-main** direcotry to your disk.

Open VSCode, open PlatformIO extension (usually the ant head icon on the left strip).

Choose "Pick a folder" and open your extracted folder.

Let PlatformIO do its thing, it can take a while if you import the project for the first time.

Alternatively you could just right click on your folder and choose "open with Code"...

## Setting up.
Get your API key from:

Mainnet - https://erwin.lol/box-opener

Devnet - https://devnet.erwin.lol/box-opener

Set up your wifi credentials (ssid and password) and yout API key in main.cpp file (in the "src" directory).
```
//=====wifi setup
const char *ssid = "YOUR_WIFI_SSID"; // <---------------------- SET THIS !!!
const char *password = "YOUR_WIFI_PASSWORD"; // <-------------- SET THIS !!!

//=====Box Opener client setup
const char *apiUrl = "https://api.erwin.lol/"; // mainnet
//const char *apiUrl = "https://devnet-api.erwin.lol/"; // devnet
const char *apiKey = "YOUR_API_KEY"; // <---------------------- SET THIS !!!
```

If you have different board then T-Display-S3 you should change it in platformio.ini file
```
[env:lilygo-t-display-s3]
platform = espressif32
board = lilygo-t-display-s3
```

Compile and upload to your board (click the right arrow button on bottom strip of the VSCode).

## Running.
Open serial terminal with 115200 bps baudrate and connect to the esp32 board.

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
## Problems
If the code does not run on your esp32 board (you see a repeating wall of text in serial terminal at the 115200 baudrate) try disabling the platform_packages in platformio.ini file
```
;platform_packages = platformio/framework-arduinoespressif32@3.20008.0
```
I had a problem when compiling the code on different computer.

It turned out that the code compiled with newer version of the framework caused guru meditation errors on the esp32 I have.

So I had to force the framework version which worked.

Maybe you will have to choose different version. I have no idea where to look for them :]


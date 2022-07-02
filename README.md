
# CPShell

A shell for the Classpad



## Installation

Firstly, clone the project

```bash
git clone https://github.com/s3ansh33p/CPShell
cd CPShell
```
Make sure that you have python3 installed, then install the "Pillow" package
```bash
python3 -m pip install --upgrade pip
python3 -m pip install --upgrade Pillow
```
To convert the textures for the classpad, run the following python scripts.
```bash
python3 ./convert_textures.py
python3 ./convert_fonts.py
```
To compile the program, make sure that you have the [Hollyhock-2 SDK](https://github.com/SnailMath/hollyhock-2) istalled, with newlib.

Once the SDK is installed, compile the program.
```bash
make hhk
```
If you want to recompile you can run the following command.
```bash
make clean && make hhk
```

Now that the program is compiled, you will need to copy some files the classpad.

First, copy the fonts from `./res/CPShell/fnt` to `/usr/fonts/` on the classpad.

If there are existing files for `5x6` and `7x8` on the classpad, you will not need to copy those.

Secondly, copy the `./CPShell.hhk` to `/bin/CPShell.hhk` on the classpad.



## Classpad Structure

I have changed the structure of the classpad to clean things up.

Note that the classpad only supports folders that are 3 levels deep
(so `/fls0/A/B/C` would be accessible but not `/fls0/A/B/C/D`).
```
📦fls0 (flash root)
 ┣ 📂bin (hhk programs)
 ┃ ┣ 📜CPShell.hhk
 ┃ ┗ 📜CPBoy.bin
 ┣ 📂etc
 ┃ ┗ 📜release (plan to have files here for the Shell)
 ┣ 📂usr (user data)
 ┃ ┣ 📂textures (for textures through Interchan's texture code)
 ┃ ┣ 📂ponggers2 (example)
 ┃ ┃ ┣ 📜ball
 ┃ ┃ ┗ 📜ball_trail
 ┃ ┣ 📂CPFlappyBird (example)
 ┃ ┃ ┣ 📜background
 ┃ ┃ ┗ 📜flappy0
 ┃ ┣ 📂fonts (for fonts through Interchan's texture code, in one folder as everything uses same fonts)
 ┃ ┃ ┣ 📜5x6
 ┃ ┃ ┗ 📜7x8
 ┃ ┣ 📂saves (for games and the CPBoy for example)
 ┃ ┃ ┣ 📂CPBoy
 ┃ ┃ ┃ ┗ 📜Zelda
 ┃ ┃ ┗ 📂hhkengine
 ┃ ┃   ┗ 📜Test
 ┃ ┣ 📂CPBoy (also has other folders for custom stuff)
 ┃ ┃ ┣ 📂roms
 ┃ ┃ ┃ ┗ 📜Zelda.gb
 ┃ ┃ ┗ 📂palettes
 ┃ ┃   ┗ 📜Custom 0.gb
 ┃ ┗ 📂xcp (actual classpad files)
 ┃   ┗ 📜binomial.xcp
 ┗ 📜run.bin (loader for bin/hhk)
```

As a result, I have modified the hollyhock launcher itself so .hhk and .bin files are loaded from the `fls0/bin` directory, though if a user wishes to leave them in the root directory, they can.

## Progress

|  |  |
|--|--|
| Overall | <img height="14" src="https://us-central1-progress-markdown.cloudfunctions.net/progress/30"> |
| User Interface | <img height="14" src="https://us-central1-progress-markdown.cloudfunctions.net/progress/50"> |
| Commands | <img height="14" src="https://us-central1-progress-markdown.cloudfunctions.net/progress/45"> |
| System Files | <img height="14" src="https://us-central1-progress-markdown.cloudfunctions.net/progress/40"> |
| Communication over Serial | <img height="14" src="https://us-central1-progress-markdown.cloudfunctions.net/progress/0"> |



### User Interface

- [✓] Layout
- [✓] Virtual Keyboard
- [✗] Touchscreen Support
- [✗] Scrollbar



### Commands

#### `Files`
- [✓] cat - Display contents of a file
- [✓] cd - Changes directory (cd .. to go up a level) and support absolute paths
- [✗] cp - Copy a file
- [✓] ls - List files in a directory
- [✗] mkdir - Make a directory
- [✗] mv - Move a file
- [✗] rm - Remove a file
- [✗] rmdir - Remove a directory
- [✗] touch - Create an empty file with the given name

#### `Terminal Utilities`
- [✓] clear - Clear the terminal
- [✓] echo - Display the text given to it
- [✓] exit - Exit the shell
- [✓] help - Display help information
- [✓] history - Read the history file (/usr/.history)

#### `System Utilities`
- [✓] date - Show the current date and time and can edit it
- [✗] reboot - Send a signal to the system to reboot
- [✗] shutdown - Send a signal to the system to shut down.
- [✗] uptime - Show the time since the system was booted

#### `Classpad Utilities`
- [✗] xcp-encode - Encode a text file into a classpad file
- [✗] xcp-decode - Decode a classpad file into a text file

#### `Miscelaneous`
- [✓] credits - Show the credits
- [✗] osname - Change the name of the OS (0x808FCC10)
- [✗] username - Change the username showed on end screen



### System Files
- [✓] /usr/.history - History of commands entered into the shell
- [✓] /usr/.profile - Profile file for the shell
- [✗] /usr/.hushlogin - Skip the motd on login
- [✗] /etc/motd - Message of the Day (motd) / Welcome message
- [✗] /etc/os-release - OS release information



### Communication over Serial

- [✗] Serial Communication



## Notes

Todo: Attached listener to [-] Key for easier arguments which writes '-' to terminal buffer.



## License

This project is under [GPL-3](https://choosealicense.com/licenses/gpl-3.0/)



## Contributors

- [@s3ansh33p](https://www.github.com/s3ansh33p)
- [@SnailMath](https://www.github.com/SnailMath)
- [@diddyholz](https://www.github.com/diddyholz)
- [@Interchan](https://www.github.com/InterChan374)
- [@Pho3](https://www.github.com/TheRainbowPhoenix)


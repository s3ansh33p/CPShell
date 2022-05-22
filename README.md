
# CPShell

A shell for the Classpad

## Installation

Firstly, clone the project

```bash
git clone https://github.com/s3ansh33p/CPShell
cd hhkengine
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

## License

This project is under [GPL-3](https://choosealicense.com/licenses/gpl-3.0/)


## Contributors

- [@s3ansh33p](https://www.github.com/s3ansh33p)
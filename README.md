# Raspberry Pi FM Radio Transmitter

Software-Defined Radio implemented in C++ for use on Raspberry Pi hardware.

Outputs the RF signal to **GPIO4** (Pin 7). Connect a short piece of wire to this pin to act as an antenna.

I made this project a few years ago for fun. Only ever tested on the _Raspberry Pi 3 Model B+_.

## Usage

Install dependencies:

```
$ sudo apt-get update
$ sudo apt-get install build-essential libraspberrypi-dev
```

Build with:

```
$ make
```

Run with:

```
$ sudo ./fm [FREQUENCY] [FILES...]
```

Where **[FREQUENCY]** is in MGHz and the supplied audio file(s) are **.wav** files.

## Legal Disclaimer

Unauthorized radio transmissions may be illegal depending on where you live. Please consult your local laws for further information.

## Copyright

This software is licensed under MIT. Copyright © 2025 Zach Collins

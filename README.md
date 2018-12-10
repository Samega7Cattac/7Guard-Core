# 7Guard
7Guard - OTP encryption software

## Getting Started

For Windows you can download the pre-compiled executable.

You also can compile it using `Developer Command Prompt for VS 2017` explained [here](https://docs.microsoft.com/pt-pt/cpp/build/walkthrough-compiling-a-native-cpp-program-on-the-command-line?view=vs-2017#open-a-developer-command-prompt) or creating a `c++ empty project` on Visual Studio and adding the files to it.


For linux you need to download the source code and have `g++` installed.
Extract the `.tar.gz` file somewhere and open a terminal to compile the code:

```
mkdir build
g++ Source.cpp otp_s7c.cpp -o build/7Guard -mrdrnd
```

## How it work

7Guard use a encryption tecnique called [OTP](https://en.wikipedia.org/wiki/One-time_pad) (One-Time-Pad) witch sum the byte from the file with a random number, saving the sum to the crypted file and the random number used to the key file.

```
file_to_crypt + random_number = crypted_file
                      |               |
                      |                -->Stored in crypted file (*.7cy)
                       ------------------>Stored in key file (*.7ky)
```

To decrypt it's just inverting the sum.

```
original_file = crypted_file - random_number
```

The problems with this tecnique are 1.) Generation of truly random numbers, and 2.) The crypted and key file will have the same size of the original one.

1) The problem was solved by using the Intel functions to generate random numbers in hardware level (more info [here](https://en.wikipedia.org/wiki/RdRand)).

2) In this version you need to deal with it. I'm already working on a implemetation with a server (more for enterprise use) where the keys will be stored on the enterprise server.

## Usage

```
Options:
        -crypt - path to the file to crypt
        -decrypt - path to the crypted file
        -key - path to the key file (only with "-decrypt")
        -h - display this help message
Usage:
        7Guard_CLI_v1b.exe [ -crypt <file_to_crypt.*> | -decrypt <file_to_decrypt.7cy> -key <file_with_key.7ky]

Optional:
        -o - Path to output folder
        -i - percentage refresh time in sec
        --buf - specify the size used each of the 2 buffers (O = max)
        --threads - EXPERIMENTAL, uses threads with a limited number of blocks in queue (O = default)
                (Only high-end CPUs recomendaded)
```

## Versioning

For the versions available, see the [releases on this repository](https://github.com/Samega7Cattac/7Guard-Core/releases). 

## Authors

* **Samega 7Cattac** - *Initial work* - [Samega 7Cattac](https://github.com/Samega7Cattac)

See also the list of [contributors](https://github.com/Samega7Cattac/7Guard-Core/graphs/contributors) who participated in this project.

## License

This project is licensed under the GNU General Public License v3.0 License - see the [LICENSE](LICENSE) file for details


7Guard Copyright (C) 2018 Samega 7Cattac

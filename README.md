# 7Guard
7Guard - OTP encryption software

## Getting Started

## Requirements
- git
- make
- Doxygen (optional, only if you want to generate the docs)

## Build

```
git clone https://github.com/Samega7Cattac/7Guard-Core.git
make
```
And you should have `7Guard` inside `./build` folder.

## How it works

7Guard uses a encryption tecnique called [OTP](https://en.wikipedia.org/wiki/One-time_pad) (One-Time-Pad) which sums the bytes from the file with a random number, saving the sum to the crypted file and the random number will be used as the key file.

```
file_to_crypt + random_number = crypted_file
                      |               |
                      |                -->Stored in crypted file (*.7cy)
                       ------------------>Stored in key file (*.7ky)
```

To decrypt it's just inverting the process.

```
original_file = crypted_file - random_number
```

The issues with this tecnique are:
- Generation of truly random numbers
  - The problem was solved by using the Intel functions to generate random numbers in hardware level (more info [here](https://en.wikipedia.org/wiki/RdRand)).
- The crypted and key file will have the same size of the original one.
  - In this version you need to deal with it. I'm already working on a implemetation with a server (more for enterprise use) where the keys will be stored on the enterprise server.

## Usage

```
Options:
        -crypt - path to the file to crypt
        -decrypt - path to the crypted file
        -key - path to the key file (only with "-decrypt")
        -h - display this help message
Usage:
        7Guard [ -crypt <file_to_crypt.*> | -decrypt <file_to_decrypt.7cy> -key <file_with_key.7ky]

Optional:
        -o - Path to output folder
        -i - percentage refresh time in secs
        --buf - specify the size used for each of the 2 buffers (O = max)
        --threads - EXPERIMENTAL, uses threads with a limited number of blocks in queue (O = default)
                (Only high-end CPUs recomendaded)
```

`make` options:
```
make        - build targeted for release
make debug  - build targeted for debug
make docs   - generate docs using Doxygen
make clean  - cleans the current build
```

## Versioning

For the versions available, see the [releases on this repository](https://github.com/Samega7Cattac/7Guard-Core/releases). 

## Authors

* **Samega 7Cattac** - *Initial work* - [Samega 7Cattac](https://github.com/Samega7Cattac)

See also the list of [contributors](https://github.com/Samega7Cattac/7Guard-Core/graphs/contributors) who participated in this project.

## License

This project is licensed under the GNU General Public License v3.0 License - see the [LICENSE](LICENSE) file for details


7Guard Copyright (C) 2019 Samega 7Cattac

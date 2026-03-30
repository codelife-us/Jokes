# Dad Jokes

A collection of Dad Jokes - as a C++ command line application.

## Description

Dad Jokes is a command-line program that displays a random joke, by default, the whole list, with colors or numbers so you can display one particular favorite. 

## Building

To compile the program, use the provided build task:


For Mac
```bash
clang++ -std=c++20 -stdlib=libc++ -fcolor-diagnostics -fansi-escape-codes -g dadjokes.cpp -o dadjokes
```
For Linux
```bash
g++ -std=c++20 dadjokes.cpp -o dadjokes
```

## Usage

```
./dadjokes [OPTIONS]
```

### Options

- `-h, --help` - Display this help message
- *(none)* - Display one random joke
- `-a, --all` - Display all jokes shuffled
- `-t, --together` - Display joke setup on one line and punchline on one line with colon separator
- `-nc, --nocolor` - Disable colored output (color is on by default)
- `-p, --pick xxx` - Display a joke that is picked by the number following
- `-sn, --shownumber` -Show the joke number in the output with brackets around it

### Examples

```bash
# Display one random joke
./dadjokes

# Display all jokes shuffled, and show the unique joke number
./dadjokes -a -sn

# Display one random joke without colors
./dadjokes --nocolor

# Display all jokes shuffled on one line (color is on by default)
./dadjokes -a -t

# Display one random joke and show its number
./dadjokes -sn

# Get help
./dadjokes -h
```

## Features

- at least 233 Dad Jokes
- Random selection or display all jokes
- Colored output by default (cyan for setup, green for punchline), with `--nocolor` to disable
- Separate line formatting option for more compact format
- Automatic shuffling of jokes
- Helpful command-line interface with built-in help

## License

This project is licensed under the MIT License - see the file header or LICENSE for details.

## Author

Created by Code Life

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
- `-c, --color` - Colored output: cyan for setup, green for punchline
- `-p, --pick xxx` - Display a joke that is picked by number following
- `-sn, --shownumber` -Show the joke number in the output with brackets around it

### Examples

```bash
# Display one random joke
./dadjokes

# Display all jokes shuffled, and show the unique joke number
./dadjokes -a -sn

# Display one random joke in colors
./dadjokes --color

# Display all jokes shuffled with colors on one line
./dadjokes -a --color -t

# Display one random joke and show its number
./dadjokes -sn

# Get help
./dadjokes -h
```

## Features

- 100 Dad Jokes
- Random selection or display all jokes
- Colored output support (cyan for setup, green for punchline)
- Separate line formatting option for more compact format
- Automatic shuffling of jokes
- Helpful command-line interface with built-in help

## License

This project is licensed under the MIT License - see the file header or LICENSE for details.

## Author

Created by Code Life

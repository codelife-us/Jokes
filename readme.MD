# Jokes

Display a collection of jokes, by type, and may use external library with a parameter - as a C++ command line application.

## Description

Jokes is a command-line program that displays a random joke, by default, the whole list, with colors or numbers so you can display one particular favorite. 

## Building

To compile the program, use the provided build task:


For Mac
```bash
clang++ jokes.cpp -o jokes
```
For Linux
```bash
g++ jokes.cpp -o jokes
```

## Usage

```
./jokes [OPTIONS]
```

### Options

- `-h, --help` - Display this help message
- *(none)* - Display one random joke
- `-a, --all` - Display all jokes shuffled instead of just one
- `-t, --type` - Default is no filter (all), but if specified, it will select from that type of joke
-                 types that can be used are all,dad,tech,pun,knock-knock,math
- `-1, --oneline` - Display joke setup on one line and punchline on one line with colon separator
- `-nc, --nocolor` - Disable colored output (color is on by default)
- `-p, --pick xxx` - Display a joke that is picked by the number following
- `-sn, --shownumber` - Show the joke number in the output with brackets around it
- `-st, --showtype` -S how the type in the output with brackets around it
- `-v, --version` - Display the version number

### Examples

```bash
# Display one random joke
./jokes
# Display one random Dad joke
./jokes -t dad
# Display all tech jokes shuffled, and show the unique joke number
./jokes -a -sn -t tech
# Display one random joke without colors
./jokes --nocolor
# Display all jokes shuffled on one line (color is on by default)
./jokes -a -1
# Display one random joke and show its number
./jokes -sn
# Get help
./jokes -h
```

## Features

- 357 jokes
- Random selection or display all jokes
- Optionally filter by type, default is dad type
- Colored output by default (cyan for setup, green for punchline), use `--nocolor` to disable
- Separate line formatting option for more compact format
- Automatic shuffling of jokes
- Helpful command-line interface with built-in help

## License

This project is licensed under the MIT License - see the file header or LICENSE for details.

## Author

Created by Code Life

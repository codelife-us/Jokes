# jokes_image

Generates a framed `.jpg` image of a joke from the jokes collection.

## Usage

```bash
./jokes_image -p 5                         # saves joke_5.jpg
./jokes_image -p 5 -o funny.jpg            # custom filename
./jokes_image -p 5 -f /path/to/font.ttf   # custom font
./jokes_image -h                            # help
```

## Options

| Option | Description |
|---|---|
| `-p, --pick <number>` | Joke number to render (required) |
| `-o, --output <file>` | Output filename (default: `joke_<number>.jpg`) |
| `-f, --font <path>` | Path to a `.ttf` or `.ttc` font file |
| `-h, --help` | Display help message |

## Build

Download the two stb header files into the project directory:

```bash
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_truetype.h
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h
```

Compile:

```bash
# macOS / Linux
g++ -std=c++17 -O2 jokes_image.cpp -o jokes_image

# Windows (MinGW)
g++ -std=c++17 -O2 jokes_image.cpp -o jokes_image.exe
```

No other dependencies — the stb headers are the only requirement.

## Output

- 800x420 pixel JPEG, quality 90
- White background with a blue frame
- Small grey label: `Code Life Joke #N  [type]`
- Setup text in cyan (knock-knock jokes include the full prompt)
- Punchline in green
- Text is word-wrapped to fit within the frame

## Default Font Paths

| Platform | Path |
|---|---|
| macOS | `/System/Library/Fonts/Helvetica.ttc` |
| Windows | `C:/Windows/Fonts/arial.ttf` |
| Linux | `/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf` |

Override with `-f` to use any `.ttf` or `.ttc` font.

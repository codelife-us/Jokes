# jokes_image

Generates a framed `.jpg` image of a joke from the jokes collection.

## Usage

```bash
./jokes_image -p 5                         # saves joke_5_classic.jpg
./jokes_image -p 5 -o funny.jpg            # custom filename
./jokes_image -p 5 -f /path/to/font.ttf   # custom font
./jokes_image -p 5 -t dark                 # dark theme
./jokes_image -p 5 -t all                  # save one image per theme
./jokes_image -p 5 --noborder              # no border frame (or -nb)
./jokes_image --version                    # print version (or -v)
./jokes_image -h                            # help
```

## Options

| Option | Description |
|---|---|
| `-p, --pick <number>` | Joke number to render (required) |
| `-o, --output <file>` | Output filename (default: `joke_<number>_<theme>.jpg`) |
| `-f, --font <path>` | Path to a `.ttf` or `.ttc` font file |
| `-t, --theme <name>` | Color theme (default: `classic`); use `all` to output every theme |
| `-nb, --noborder` | Omit the decorative border frame |
| `-v, --version` | Print version and exit |
| `-h, --help` | Display help message |

## Themes

| Name | Background | Style |
|---|---|---|
| `classic` | White | Blue border, cyan setup, green punchline |
| `dark` | Near-black | Blue border, bright cyan setup, mint punchline |
| `sunset` | Warm cream | Burnt-orange border, amber text |
| `ocean` | Pale blue | Deep blue border, ocean-blue text |
| `retro` | Yellow | Brown border, amber text |
| `night` | Deep navy | Purple border, lavender setup, teal punchline |

Use `-t all` to generate one file per theme in a single run (e.g. `joke_5_classic.jpg`, `joke_5_dark.jpg`, …). The `-o` flag is ignored when using `all`.

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

- 800x500 pixel JPEG, quality 90
- Themed background and border (see Themes above)
- Text is larger (36px), bold, and horizontally centered
- Small label centered just above the bottom border: `Code Life Jokes #N  [type]`
- Setup text color and punchline color are theme-defined
- Knock-knock jokes include the full "Knock knock / Who's there?" prompt
- Text is word-wrapped to fit within the frame

## Default Font Paths

| Platform | Path |
|---|---|
| macOS | `/System/Library/Fonts/Helvetica.ttc` |
| Windows | `C:/Windows/Fonts/arial.ttf` |
| Linux | `/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf` |

Override with `-f` to use any `.ttf` or `.ttc` font.

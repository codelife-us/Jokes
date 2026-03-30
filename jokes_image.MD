# jokes_image

Generates a framed `.jpg` image of a joke from the jokes collection.

## Usage

```bash
./jokes_image -p 5                         # saves joke_5_classic.jpg
./jokes_image -p 5 -o funny.jpg            # custom filename
./jokes_image -p 5 -f /path/to/font.ttf   # custom font
./jokes_image -p 5 -t dark                 # dark theme
./jokes_image -p 5 -t all                  # save one image per theme
./jokes_image -p 5 -bg photo.jpg           # use photo.jpg as background
./jokes_image -p 5 -bg photo.jpg -nb      # background with no border
./jokes_image -p 5 -bg photo.jpg --nodim  # background without dimming
./jokes_image -p 5 --roundedcorners       # rounded border corners (or -rc)
./jokes_image -p 5 --nofooter             # no footer label, border stays intact
./jokes_image -p 5 --noborder              # no border frame (or -nb)
./jokes_image --setup "Why?" --punchline "Because!"  # custom text, no -p needed
./jokes_image --version                    # print version (or -v)
./jokes_image -h                            # help
```

## Options

| Option | Description |
|---|---|
| `-p, --pick <number>` | Joke number to render (required unless `--setup`/`--punchline` used) |
| `-o, --output <file>` | Output filename (default: `joke_<number>_<theme>.jpg`) |
| `-f, --font <path>` | Path to a `.ttf` or `.ttc` font file |
| `-t, --theme <name>` | Color theme (default: `classic`); use `all` to output every theme |
| `-bg, --background <file>` | Use a `.jpg` or `.png` image as the background layer |
| `--nodim` | Skip dimming the background image (default dims by 45%) |
| `--nofooter` | Omit the footer label; border is drawn fully intact |
| `--setup "text"` | Use custom setup text instead of a joke from the data |
| `--punchline "text"` | Use custom punchline text instead of a joke from the data |
| `-rc, --roundedcorners` | Use rounded corners on the border frame |
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

The required stb headers (`stb_truetype.h`, `stb_image_write.h`, `stb_image.h`) are included in the repository — no separate download needed.

```bash
# macOS / Linux
g++ -std=c++17 -O2 jokes_image.cpp -o jokes_image

# Windows (MinGW)
g++ -std=c++17 -O2 jokes_image.cpp -o jokes_image.exe
```

## Output

- 800x500 pixel JPEG, quality 90
- Themed background and border (see Themes above), or a custom background image via `-bg`
- When a background image is used, it is scaled to fill 800x500 and dimmed 45% by default; use `--nodim` to skip dimming
- Text is larger (36px), bold, and horizontally centered
- Small label sits in a gap on the bottom border: `Code Life Jokes #N  [type]` (omit with `--nofooter`)
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

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
./jokes_image -p 5 -fg overlay.png         # PNG overlay above background, below text
./jokes_image -p 5 -bg photo.jpg -nb      # background with no border
./jokes_image -p 5 -bg photo.jpg --nodim  # background without dimming
./jokes_image -p 5 --roundedcorners       # rounded border corners (or -rc)
./jokes_image -p 5 --nofooter                      # no footer, border stays intact
./jokes_image -p 5 --footer "My Custom Text"       # custom footer text
./jokes_image -p 5 --footer "Joke #N - [type]"    # tokens replaced with actual values
./jokes_image -p 5 --footer ""                     # suppress footer (same as --nofooter)
./jokes_image -p 5 --noborder              # no border frame (or -nb)
./jokes_image --setup "Why?" --punchline "Because!"    # custom text, no -p needed
./jokes_image -p 5 --aligntext center,middle           # centered on both axes
./jokes_image -p 5 --aligntext left,bottom             # left-aligned, pinned to bottom
./jokes_image -p 5 -t dark --setupcolor "#ffffff"     # white setup text
./jokes_image -p 5 --setupcolor "#ff6600" --punchlinecolor "#ffffff"  # custom colors
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
| `-fg, --foreground <file>` | Overlay a `.png` with transparency above the background but below text |
| `--textsize <percent>` | Scale text size as a percentage of default (e.g. `150` = 150%, default: `100`) |
| `--aligntext h,v` | Set text alignment: `h` = `left\|center\|right`, `v` = `top\|middle\|bottom` (default: `center,middle`) |
| `--nodim` | Skip dimming the background image (default dims by 45%) |
| `--nofooter` | Omit the footer; border is drawn fully intact |
| `--footer "text"` | Override the footer text; use `""` to suppress it entirely (same as `--nofooter`). Supports tokens: `[type]` = joke type, `#N` = joke number |
| `--setup "text"` | Use custom setup text instead of a joke from the data |
| `--punchline "text"` | Use custom punchline text instead of a joke from the data |
| `--setupcolor "#rrggbb"` | Override the setup text color with a hex value (quote to prevent shell stripping `#`) |
| `--punchlinecolor "#rrggbb"` | Override the punchline text color with a hex value (quote to prevent shell stripping `#`) |
| `--bordercolor "#rrggbb"` | Override the border color with a hex value (quote to prevent shell stripping `#`) |
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
| `white` | Near-black | White border, white setup, white punchline |
| `forest` | Deep green | Gold border, lime setup, pale yellow punchline |
| `candy` | Hot pink | White border, bright yellow setup, cyan punchline |
| `neon` | Black | Neon green border, neon green setup, neon pink punchline |
| `parchment` | Aged paper | Brown border, dark brown setup, dark red punchline |
| `ice` | Pale blue-white | Steel blue border, deep blue setup, teal punchline |
| `dusk` | Deep purple | Orange border, lavender setup, peach punchline |

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
- Text is 36px by default, bold, and horizontally centered; scale with `--textsize <percent>`
- Small label sits in a gap on the bottom border: `Code Life Jokes #N  [type]`
  - Omit with `--nofooter` or `--footer ""`
  - Override with `--footer "text"`; supports tokens `#N` (joke number) and `[type]` (joke type)
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

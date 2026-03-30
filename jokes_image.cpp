// MIT License
//
// Copyright (c) 2026 Code Life
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Jokes Image
//     by Code Life
//     generates a .jpg image of a joke
//     version 2.0 - 3/30/2026
// Description: Outputs a joke as a framed .jpg image file using stb_truetype + stb_image_write.
//              Requires: stb_truetype.h, stb_image_write.h (drop in project directory)
//              Usage: ./jokes_image -p <number> [-o <filename>] [-f <fontpath>] [-t <theme>] [--noborder]

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_truetype.h"
#include "stb_image_write.h"

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cstdint>
#include "jokes_data.h"

using namespace std;

static const char* VERSION = "2.0";

// ----------------------------------------------------------------------------
// Theme
// ----------------------------------------------------------------------------

struct Color { uint8_t r, g, b; };

struct Theme {
    Color bg;        // background fill
    Color border;    // outer frame
    Color setup;     // setup text
    Color punchline; // punchline text
    Color label;     // footer label
};

static const map<string, Theme> THEMES = {
    // name        bg               border           setup            punchline        label
    { "classic", { {255,255,255}, {50, 100, 200}, {0,  140, 180}, {30, 160,  60}, {140,140,140} } },
    { "dark",    { {30,  30,  40}, {80, 160, 240}, {80, 200, 240}, {80, 210, 120}, {170,170,170} } },
    { "sunset",  { {255,240,220}, {200,  80,  40}, {180,  60,  20}, {210, 120,  10}, {160,100, 60} } },
    { "ocean",   { {220,240,255}, {20,  80, 160}, {10, 100, 180}, {20, 140, 160}, {100,140,170} } },
    { "retro",   { {255,255,200}, {160,  80,   0}, {120,  60,   0}, {180, 100,   0}, {140,120, 60} } },
    { "night",   { {15,  15,  30}, {100,  60, 200}, {160, 100, 255}, {100, 220, 180}, {130,120,160} } },
};

static void listThemes() {
    cout << "Available themes: ";
    bool first = true;
    for (auto& kv : THEMES) { if (!first) cout << ", "; cout << kv.first; first = false; }
    cout << "\n";
}

// ----------------------------------------------------------------------------
// Image
// ----------------------------------------------------------------------------

struct Image {
    int w, h;
    vector<uint8_t> pixels; // RGB, 3 bytes per pixel

    Image(int w, int h, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255)
        : w(w), h(h), pixels(w * h * 3) {
        for (int i = 0; i < w * h; i++) {
            pixels[i*3+0] = r;
            pixels[i*3+1] = g;
            pixels[i*3+2] = b;
        }
    }

    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
        if (x < 0 || x >= w || y < 0 || y >= h) return;
        pixels[(y * w + x) * 3 + 0] = r;
        pixels[(y * w + x) * 3 + 1] = g;
        pixels[(y * w + x) * 3 + 2] = b;
    }

    void blendGlyph(const uint8_t* bitmap, int bw, int bh, int px, int py,
                    uint8_t r, uint8_t g, uint8_t b) {
        for (int row = 0; row < bh; row++) {
            for (int col = 0; col < bw; col++) {
                uint8_t alpha = bitmap[row * bw + col];
                if (alpha == 0) continue;
                int ix = px + col, iy = py + row;
                if (ix < 0 || ix >= w || iy < 0 || iy >= h) continue;
                float a = alpha / 255.0f;
                uint8_t* p = &pixels[(iy * w + ix) * 3];
                p[0] = (uint8_t)(p[0] * (1 - a) + r * a);
                p[1] = (uint8_t)(p[1] * (1 - a) + g * a);
                p[2] = (uint8_t)(p[2] * (1 - a) + b * a);
            }
        }
    }

    void drawRect(int x0, int y0, int x1, int y1,
                  uint8_t r, uint8_t g, uint8_t b, int thickness = 4) {
        for (int t = 0; t < thickness; t++) {
            for (int x = x0 + t; x <= x1 - t; x++) {
                setPixel(x, y0 + t, r, g, b);
                setPixel(x, y1 - t, r, g, b);
            }
            for (int y = y0 + t; y <= y1 - t; y++) {
                setPixel(x0 + t, y, r, g, b);
                setPixel(x1 - t, y, r, g, b);
            }
        }
    }

    bool writeJpeg(const string& filename, int quality = 90) const {
        return stbi_write_jpg(filename.c_str(), w, h, 3, pixels.data(), quality) != 0;
    }
};

// ----------------------------------------------------------------------------
// Font
// ----------------------------------------------------------------------------

// Decode one UTF-8 codepoint from str at byte position i; advances i past it.
// Returns the codepoint, or '?' for invalid sequences.
static int nextCodepoint(const string& str, int& i) {
    unsigned char b = str[i];
    int cp;
    int extra;
    if      (b < 0x80) { cp = b;         extra = 0; }
    else if (b < 0xE0) { cp = b & 0x1F;  extra = 1; }
    else if (b < 0xF0) { cp = b & 0x0F;  extra = 2; }
    else               { cp = b & 0x07;  extra = 3; }
    i++;
    for (int e = 0; e < extra; e++) {
        if (i >= (int)str.size()) return '?';
        cp = (cp << 6) | (str[i++] & 0x3F);
    }
    return cp;
}

struct Font {
    stbtt_fontinfo info;
    vector<uint8_t> buffer;
    float scale = 1.0f;

    bool load(const string& path, float pixelHeight) {
        ifstream f(path, ios::binary | ios::ate);
        if (!f) return false;
        buffer.resize(f.tellg());
        f.seekg(0);
        f.read((char*)buffer.data(), buffer.size());
        int offset = stbtt_GetFontOffsetForIndex(buffer.data(), 0);
        if (offset < 0) return false;
        if (!stbtt_InitFont(&info, buffer.data(), offset)) return false;
        scale = stbtt_ScaleForPixelHeight(&info, pixelHeight);
        return true;
    }

    int textWidth(const string& text) const {
        int width = 0;
        for (int i = 0; i < (int)text.size(); ) {
            int cp = nextCodepoint(text, i);
            int advance, lsb;
            stbtt_GetCodepointHMetrics(&info, cp, &advance, &lsb);
            width += (int)(advance * scale);
        }
        return width;
    }

    void drawText(Image& img, const string& text, int x, int y,
                  uint8_t r, uint8_t g, uint8_t b, bool bold = false) const {
        int cursor = x;
        int prev = 0;
        for (int i = 0; i < (int)text.size(); ) {
            int cp = nextCodepoint(text, i);
            if (prev)
                cursor += (int)(stbtt_GetCodepointKernAdvance(&info, prev, cp) * scale);

            int advance, lsb;
            stbtt_GetCodepointHMetrics(&info, cp, &advance, &lsb);

            int x0, y0, x1, y1;
            stbtt_GetCodepointBitmapBox(&info, cp, scale, scale, &x0, &y0, &x1, &y1);

            int gw = x1 - x0, gh = y1 - y0;
            if (gw > 0 && gh > 0) {
                vector<uint8_t> bitmap(gw * gh);
                stbtt_MakeCodepointBitmap(&info, bitmap.data(), gw, gh, gw, scale, scale, cp);
                img.blendGlyph(bitmap.data(), gw, gh, cursor + x0, y + y0, r, g, b);
                if (bold)
                    img.blendGlyph(bitmap.data(), gw, gh, cursor + x0 + 1, y + y0, r, g, b);
            }
            cursor += (int)(advance * scale);
            prev = cp;
        }
    }

    void drawTextCentered(Image& img, const string& text, int imgWidth, int y,
                          uint8_t r, uint8_t g, uint8_t b, bool bold = false) const {
        int x = (imgWidth - textWidth(text)) / 2;
        drawText(img, text, x, y, r, g, b, bold);
    }

    // Word-wrap text into lines that fit within maxWidth
    vector<string> wrapText(const string& text, int maxWidth) const {
        vector<string> lines;
        string current;
        size_t i = 0;
        while (i <= text.size()) {
            size_t space = text.find(' ', i);
            if (space == string::npos) space = text.size();
            string word = text.substr(i, space - i);
            string candidate = current.empty() ? word : current + " " + word;
            if (!current.empty() && textWidth(candidate) > maxWidth) {
                lines.push_back(current);
                current = word;
            } else {
                current = candidate;
            }
            i = space + 1;
        }
        if (!current.empty()) lines.push_back(current);
        return lines;
    }
};

// ----------------------------------------------------------------------------
// Joke image renderer
// ----------------------------------------------------------------------------

bool jokeToJpeg(const Joke& joke, int jokeNumber, const string& filename,
                const string& fontPath, const Theme& theme, bool noborder = false) {
    const int W      = 800, H = 500;
    const int MARGIN = 50;
    const int FRAME  = 16;
    const int LINE_H = 54;

    Image img(W, H, theme.bg.r, theme.bg.g, theme.bg.b);

    Font font;
    if (!font.load(fontPath, 36)) {
        cerr << "Error: could not load font: " << fontPath << "\n";
        return false;
    }

    // Outer frame
    if (!noborder)
        img.drawRect(FRAME, FRAME, W - FRAME, H - FRAME,
                     theme.border.r, theme.border.g, theme.border.b, 5);

    int textMaxWidth = W - MARGIN * 2;
    int y = MARGIN + 28;

    // Setup text — bold, centered
    auto setupLines = font.wrapText(
        joke.type == "knock-knock"
            ? "Knock knock! Who's there? " + joke.setup + "  " + joke.setup + " who?"
            : joke.setup,
        textMaxWidth);
    for (auto& line : setupLines) {
        font.drawTextCentered(img, line, W, y, theme.setup.r, theme.setup.g, theme.setup.b, true);
        y += LINE_H;
    }

    y += 20; // gap

    // Punchline text — bold, centered
    for (auto& line : font.wrapText(joke.punchline, textMaxWidth)) {
        font.drawTextCentered(img, line, W, y, theme.punchline.r, theme.punchline.g, theme.punchline.b, true);
        y += LINE_H;
    }

    // Label just above the bottom border (small, centered)
    Font smallFont;
    if (smallFont.load(fontPath, 18)) {
        string label = "Code Life Jokes #" + to_string(jokeNumber) + "  [" + joke.type + "]";
        int labelY = H - FRAME - 10; // baseline sits 10px above the inner edge of the frame
        smallFont.drawTextCentered(img, label, W, labelY, theme.label.r, theme.label.g, theme.label.b);
    }

    if (!img.writeJpeg(filename)) {
        cerr << "Error: could not write image: " << filename << "\n";
        return false;
    }

    cout << "Saved: " << filename << "\n";
    return true;
}

// ----------------------------------------------------------------------------
// Font path detection (platform defaults)
// ----------------------------------------------------------------------------

string defaultFontPath() {
#if defined(_WIN32)
    return "C:/Windows/Fonts/arial.ttf";
#elif defined(__APPLE__)
    return "/System/Library/Fonts/Helvetica.ttc";
#else
    // Linux
    return "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
#endif
}

// ----------------------------------------------------------------------------
// Help
// ----------------------------------------------------------------------------

void displayHelp() {
    cout << "Jokes Image - Generates a .jpg image of a joke\n\n";
    cout << "Usage: ./jokes_image -p <number> [OPTIONS]\n\n";
    cout << "Options:\n";
    cout << "  -p, --pick <number>   Joke number to render (required)\n";
    cout << "  -o, --output <file>   Output filename (default: joke_<number>.jpg)\n";
    cout << "  -f, --font <path>     Path to a .ttf or .ttc font file\n";
    cout << "  -t, --theme <name>    Color theme (default: classic)\n";
    cout << "  -nb, --noborder       Omit the decorative border frame\n";
    cout << "  -v,  --version        Print version and exit\n";
    cout << "  -h, --help            Display this help message\n\n";
    cout << "Themes: classic, dark, sunset, ocean, retro, night, all\n\n";
    cout << "Examples:\n";
    cout << "  ./jokes_image -p 5                     - Save joke_5.jpg\n";
    cout << "  ./jokes_image -p 5 -o funny.jpg        - Save funny.jpg\n";
    cout << "  ./jokes_image -p 5 -t dark             - Dark theme\n";
    cout << "  ./jokes_image -p 5 --noborder          - No border frame (-nb)\n";
    cout << "  ./jokes_image -p 5 -f /path/to/font.ttf\n";
}

// ----------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    int    jokeNumber  = -1;
    string outputFile;
    string fontPath    = defaultFontPath();
    string themeName   = "classic";
    bool   noborder    = false;

    try {
        for (int i = 1; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-p" || arg == "--pick") {
                if (i + 1 < argc) {
                    jokeNumber = stoi(argv[++i]);
                } else {
                    cerr << "Error: --pick requires a number.\n";
                    return 1;
                }
            } else if (arg == "-o" || arg == "--output") {
                if (i + 1 < argc) {
                    outputFile = argv[++i];
                } else {
                    cerr << "Error: --output requires a filename.\n";
                    return 1;
                }
            } else if (arg == "-f" || arg == "--font") {
                if (i + 1 < argc) {
                    fontPath = argv[++i];
                } else {
                    cerr << "Error: --font requires a path.\n";
                    return 1;
                }
            } else if (arg == "-t" || arg == "--theme") {
                if (i + 1 < argc) {
                    themeName = argv[++i];
                } else {
                    cerr << "Error: --theme requires a name.\n";
                    listThemes();
                    return 1;
                }
            } else if (arg == "--noborder" || arg == "-nb") {
                noborder = true;
            } else if (arg == "--version" || arg == "-v") {
                cout << "jokes_image version " << VERSION << "\n";
                return 0;
            } else if (arg == "-h" || arg == "--help") {
                displayHelp();
                return 0;
            } else {
                cerr << "Unknown option: " << arg << "\n";
                displayHelp();
                return 1;
            }
        }
    } catch (...) {
        cerr << "Error parsing command line arguments.\n";
        return 1;
    }

    if (themeName != "all" && THEMES.find(themeName) == THEMES.end()) {
        cerr << "Error: unknown theme '" << themeName << "'.\n";
        listThemes();
        return 1;
    }

    if (jokeNumber < 1) {
        cerr << "Error: joke number required. Use -p <number>.\n\n";
        displayHelp();
        return 1;
    }

    if (jokeNumber > (int)jokes.size()) {
        cerr << "Error: joke #" << jokeNumber << " does not exist. (Range: 1-" << jokes.size() << ")\n";
        return 1;
    }

    const Joke& joke = jokes[jokeNumber - 1];

    if (themeName == "all") {
        bool ok = true;
        for (auto& kv : THEMES) {
            string file = outputFile.empty()
                ? "joke_" + to_string(jokeNumber) + "_" + kv.first + ".jpg"
                : outputFile;
            ok &= jokeToJpeg(joke, jokeNumber, file, fontPath, kv.second, noborder);
        }
        return ok ? 0 : 1;
    }

    if (outputFile.empty())
        outputFile = "joke_" + to_string(jokeNumber) + "_" + themeName + ".jpg";

    return jokeToJpeg(joke, jokeNumber, outputFile, fontPath, THEMES.at(themeName), noborder) ? 0 : 1;
}

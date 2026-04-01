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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_truetype.h"
#include "stb_image_write.h"
#include "stb_image.h"

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cstdint>
#include "jokes_data.h"

using namespace std;

static const char* VERSION = "2.2";

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
    { "white",   { {30,  30,  40}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {200,200,200} } },
    { "forest",  { {26,  58,  26}, {200, 160,  64}, {128, 224,  64}, {232, 232, 128}, {160,180,100} } },
    { "candy",   { {255,  26, 140}, {255, 255, 255}, {255, 255,  64}, { 64, 255, 255}, {255,180,220} } },
    { "neon",    { {0,    0,   0}, {  0, 255,  65}, {  0, 255,  65}, {255,   0, 160}, {100,200,100} } },
    { "parchment",{ {245, 230, 200}, {139,  94,  42}, { 90,  48,  16}, {139,  26,  16}, {160,120, 80} } },
    { "ice",     { {232, 244, 255}, { 64, 128, 192}, { 16,  64, 160}, {  0,  96, 128}, { 80,140,180} } },
    { "dusk",    { {26,  10,  48}, {224, 112,  48}, {192, 128, 255}, {255, 176,  96}, {160,120,180} } },
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
                  uint8_t r, uint8_t g, uint8_t b, int thickness = 4,
                  int gapX0 = -1, int gapX1 = -1) {
        for (int t = 0; t < thickness; t++) {
            for (int x = x0 + t; x <= x1 - t; x++) {
                setPixel(x, y0 + t, r, g, b);
                if (gapX0 < 0 || x < gapX0 || x > gapX1)
                    setPixel(x, y1 - t, r, g, b);
            }
            for (int y = y0 + t; y <= y1 - t; y++) {
                setPixel(x0 + t, y, r, g, b);
                setPixel(x1 - t, y, r, g, b);
            }
        }
    }

    void drawRoundedRect(int x0, int y0, int x1, int y1,
                         uint8_t r, uint8_t g, uint8_t b, int thickness = 4, int radius = 30,
                         int gapX0 = -1, int gapX1 = -1) {
        for (int t = 0; t < thickness; t++) {
            int ax0 = x0 + t, ay0 = y0 + t;
            int ax1 = x1 - t, ay1 = y1 - t;
            int rad = radius - t;
            if (rad < 1) rad = 1;

            // Straight edges (skip the corner arc regions)
            for (int x = ax0 + rad; x <= ax1 - rad; x++) {
                setPixel(x, ay0, r, g, b);
                if (gapX0 < 0 || x < gapX0 || x > gapX1)
                    setPixel(x, ay1, r, g, b);
            }
            for (int y = ay0 + rad; y <= ay1 - rad; y++) {
                setPixel(ax0, y, r, g, b);
                setPixel(ax1, y, r, g, b);
            }

            // Quarter-circle arcs at each corner via midpoint circle algorithm
            auto arc = [&](int ccx, int ccy, int quad) {
                int cx = 0, cy = rad, d = 1 - rad;
                while (cx <= cy) {
                    switch (quad) {
                        case 0: setPixel(ccx-cy, ccy-cx,r,g,b); setPixel(ccx-cx, ccy-cy,r,g,b); break; // top-left
                        case 1: setPixel(ccx+cy, ccy-cx,r,g,b); setPixel(ccx+cx, ccy-cy,r,g,b); break; // top-right
                        case 2: setPixel(ccx+cy, ccy+cx,r,g,b); setPixel(ccx+cx, ccy+cy,r,g,b); break; // bottom-right
                        case 3: setPixel(ccx-cy, ccy+cx,r,g,b); setPixel(ccx-cx, ccy+cy,r,g,b); break; // bottom-left
                    }
                    if (d < 0) d += 2*cx + 3;
                    else { d += 2*(cx-cy) + 5; cy--; }
                    cx++;
                }
            };

            arc(ax0+rad, ay0+rad, 0); // top-left
            arc(ax1-rad, ay0+rad, 1); // top-right
            arc(ax1-rad, ay1-rad, 2); // bottom-right
            arc(ax0+rad, ay1-rad, 3); // bottom-left
        }
    }

    bool writeJpeg(const string& filename, int quality = 90) const {
        return stbi_write_jpg(filename.c_str(), w, h, 3, pixels.data(), quality) != 0;
    }

    // Load a JPEG/PNG and scale it to fill this image's dimensions (nearest-neighbor)
    bool loadBackground(const string& path) {
        int srcW, srcH, channels;
        uint8_t* data = stbi_load(path.c_str(), &srcW, &srcH, &channels, 3);
        if (!data) {
            cerr << "Error: could not load background image: " << path << "\n";
            return false;
        }
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                int sx = x * srcW / w;
                int sy = y * srcH / h;
                uint8_t* src = data + (sy * srcW + sx) * 3;
                uint8_t* dst = pixels.data() + (y * w + x) * 3;
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
            }
        }
        stbi_image_free(data);
        return true;
    }

    // Load a PNG with alpha and alpha-blend it on top of the current pixels (scaled to canvas)
    bool loadForeground(const string& path) {
        int srcW, srcH, channels;
        uint8_t* data = stbi_load(path.c_str(), &srcW, &srcH, &channels, 4); // force RGBA
        if (!data) {
            cerr << "Error: could not load foreground image: " << path << "\n";
            return false;
        }
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                int sx = x * srcW / w;
                int sy = y * srcH / h;
                uint8_t* src = data + (sy * srcW + sx) * 4;
                float a = src[3] / 255.0f;
                if (a == 0.0f) continue;
                uint8_t* dst = pixels.data() + (y * w + x) * 3;
                dst[0] = (uint8_t)(src[0] * a + dst[0] * (1.0f - a));
                dst[1] = (uint8_t)(src[1] * a + dst[1] * (1.0f - a));
                dst[2] = (uint8_t)(src[2] * a + dst[2] * (1.0f - a));
            }
        }
        stbi_image_free(data);
        return true;
    }

    // Darken the entire image by a factor in [0,1] to improve text legibility over a background
    void applyDimOverlay(float factor = 0.45f) {
        for (auto& p : pixels)
            p = (uint8_t)(p * (1.0f - factor));
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
                const string& fontPath, const Theme& theme, bool noborder = false,
                const string& bgPath = "", bool nodim = false, bool roundedcorners = false,
                bool nofooter = false, const string& labelOverride = "",
                bool hasLabelOverride = false, float textScale = 1.0f,
                const string& fgPath = "") {
    const int W         = 800, H = 500;
    const int MARGIN    = 50;
    const int FRAME     = 8;
    const int THICKNESS = 5;
    const int LINE_H    = (int)(54 * textScale);
    const float FONT_SIZE       = 36.0f * textScale;
    const float FONT_SIZE_SMALL = 18.0f * textScale;

    Image img(W, H, theme.bg.r, theme.bg.g, theme.bg.b);

    if (!bgPath.empty()) {
        if (!img.loadBackground(bgPath)) return false;
        if (!nodim) img.applyDimOverlay();
    }

    if (!fgPath.empty()) {
        if (!img.loadForeground(fgPath)) return false;
    }

    Font font;
    if (!font.load(fontPath, FONT_SIZE)) {
        cerr << "Error: could not load font: " << fontPath << "\n";
        return false;
    }

    // Resolve label text; empty string (explicit or nofooter) suppresses the footer
    Font smallFont;
    string label = hasLabelOverride ? labelOverride
                                    : "Code Life Jokes #" + to_string(jokeNumber) + "  [" + joke.type + "]";
    // Substitute [type] and #N tokens in custom labels
    if (hasLabelOverride && !label.empty()) {
        string type_val = joke.type.empty() ? "" : joke.type;
        string num_val  = jokeNumber > 0 ? to_string(jokeNumber) : "";
        size_t pos;
        while ((pos = label.find("[type]")) != string::npos)
            label.replace(pos, 6, type_val);
        while ((pos = label.find("#N")) != string::npos)
            label.replace(pos, 2, num_val);
    }
    bool showFooter = !nofooter && !label.empty();
    int gapX0 = -1, gapX1 = -1;
    int labelY = H - FRAME - THICKNESS / 2 + 6;
    if (showFooter && smallFont.load(fontPath, FONT_SIZE_SMALL)) {
        const int GAP_PAD = 12;
        int lw = smallFont.textWidth(label);
        gapX0 = (W - lw) / 2 - GAP_PAD;
        gapX1 = (W + lw) / 2 + GAP_PAD;
    }

    // Outer frame (gap only if footer is shown)
    if (!noborder) {
        if (roundedcorners)
            img.drawRoundedRect(FRAME, FRAME, W - FRAME, H - FRAME,
                                theme.border.r, theme.border.g, theme.border.b, THICKNESS,
                                30, gapX0, gapX1);
        else
            img.drawRect(FRAME, FRAME, W - FRAME, H - FRAME,
                         theme.border.r, theme.border.g, theme.border.b, THICKNESS,
                         gapX0, gapX1);
    }

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

    // Label centered in the gap on the bottom border
    if (showFooter && smallFont.load(fontPath, FONT_SIZE_SMALL)) {
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
// Hex color parser  (#rrggbb or rrggbb)
// ----------------------------------------------------------------------------

static bool parseHexColor(const string& s, Color& out) {
    string h = s;
    if (!h.empty() && h[0] == '#') h = h.substr(1);
    if (h.size() != 6) return false;
    try {
        out.r = (uint8_t)stoul(h.substr(0, 2), nullptr, 16);
        out.g = (uint8_t)stoul(h.substr(2, 2), nullptr, 16);
        out.b = (uint8_t)stoul(h.substr(4, 2), nullptr, 16);
    } catch (...) { return false; }
    return true;
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
    cout << "  -bg, --background <file>  Use a .jpg/.png image as the background layer\n";
    cout << "  -fg, --foreground <file>  Overlay a .png (with transparency) above the background, below text\n";
    cout << "  --nodim               Do not dim the background image (default: dims 45%)\n";
    cout << "  --textsize <percent>  Scale text size (default: 100); e.g. 150 = 150%\n";
    cout << "  --nofooter            Omit the footer and leave the border intact\n";
    cout << "  --footer \"text\"       Override the footer text; use \"\" to suppress it entirely\n";
    cout << "                        Tokens: [type] = joke type, #N = joke number\n";
    cout << "  --setup \"text\"        Use custom setup text instead of a joke from the data\n";
    cout << "  --punchline \"text\"    Use custom punchline text instead of a joke from the data\n";
    cout << "  --setupcolor \"#rrggbb\"     Override the setup text color (hex, quote the value)\n";
    cout << "  --punchlinecolor \"#rrggbb\" Override the punchline text color (hex, quote the value)\n";
    cout << "  --bordercolor \"#rrggbb\"    Override the border color (hex, quote the value)\n";
    cout << "  -rc, --roundedcorners Use rounded corners on the border frame\n";
    cout << "  -nb, --noborder       Omit the decorative border frame\n";
    cout << "  -v,  --version        Print version and exit\n";
    cout << "  -h, --help            Display this help message\n\n";
    cout << "Themes: classic, dark, sunset, ocean, retro, night, white, forest, candy, neon, parchment, ice, dusk, all\n\n";
    cout << "Examples:\n";
    cout << "  ./jokes_image -p 5                     - Save joke_5.jpg\n";
    cout << "  ./jokes_image -p 5 -o funny.jpg        - Save funny.jpg\n";
    cout << "  ./jokes_image -p 5 -t dark             - Dark theme\n";
    cout << "  ./jokes_image -p 5 --noborder          - No border frame (-nb)\n";
    cout << "  ./jokes_image -p 5 -f /path/to/font.ttf\n";
    cout << "  ./jokes_image -p 5 -bg photo.jpg          - Use photo.jpg as background\n";
    cout << "  ./jokes_image -p 5 -bg photo.jpg -nb      - Background with no border\n";
    cout << "  ./jokes_image -p 5 --nofooter             - No footer\n";
    cout << "  ./jokes_image -p 5 --footer \"My Text\"     - Custom footer text\n";
    cout << "  ./jokes_image -p 5 --footer \"\"            - Suppress footer\n";
    cout << "  ./jokes_image --setup \"Why?\" --punchline \"Because!\"  - Custom text\n";
}

// ----------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    int    jokeNumber  = -1;
    string outputFile;
    string fontPath    = defaultFontPath();
    string themeName   = "classic";
    bool   noborder       = false;
    bool   nodim          = false;
    bool   roundedcorners = false;
    bool   nofooter       = false;
    float  textScale      = 1.0f;
    string bgPath;
    string fgPath;
    string customSetup;
    string customPunchline;
    string labelOverride;
    bool   hasLabelOverride = false;
    string setupColorHex;
    string punchlineColorHex;
    string borderColorHex;

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
            } else if (arg == "--nodim") {
                nodim = true;
            } else if (arg == "--textsize") {
                if (i + 1 < argc) {
                    try {
                        float pct = stof(argv[++i]);
                        if (pct <= 0) throw invalid_argument("");
                        textScale = pct / 100.0f;
                    } catch (...) {
                        cerr << "Error: --textsize requires a positive number (e.g. 150 for 150%).\n";
                        return 1;
                    }
                } else {
                    cerr << "Error: --textsize requires a percentage value.\n";
                    return 1;
                }
            } else if (arg == "--nofooter") {
                nofooter = true;
            } else if (arg == "--footer") {
                if (i + 1 < argc) {
                    labelOverride = argv[++i];
                    hasLabelOverride = true;
                } else {
                    cerr << "Error: --footer requires a text argument (use \"\" for no footer).\n";
                    return 1;
                }
            } else if (arg == "--setup") {
                if (i + 1 < argc) {
                    customSetup = argv[++i];
                } else {
                    cerr << "Error: --setup requires text.\n";
                    return 1;
                }
            } else if (arg == "--punchline") {
                if (i + 1 < argc) {
                    customPunchline = argv[++i];
                } else {
                    cerr << "Error: --punchline requires text.\n";
                    return 1;
                }
            } else if (arg == "--setupcolor") {
                if (i + 1 < argc) {
                    setupColorHex = argv[++i];
                } else {
                    cerr << "Error: --setupcolor requires a hex color (e.g. #ff0000).\n";
                    return 1;
                }
            } else if (arg == "--punchlinecolor") {
                if (i + 1 < argc) {
                    punchlineColorHex = argv[++i];
                } else {
                    cerr << "Error: --punchlinecolor requires a hex color (e.g. #ffffff).\n";
                    return 1;
                }
            } else if (arg == "--bordercolor") {
                if (i + 1 < argc) {
                    borderColorHex = argv[++i];
                } else {
                    cerr << "Error: --bordercolor requires a hex color (e.g. #ffffff).\n";
                    return 1;
                }
            } else if (arg == "--roundedcorners" || arg == "-rc") {
                roundedcorners = true;
            } else if (arg == "-bg" || arg == "--background") {
                if (i + 1 < argc) {
                    bgPath = argv[++i];
                } else {
                    cerr << "Error: --background requires a file path.\n";
                    return 1;
                }
            } else if (arg == "-fg" || arg == "--foreground") {
                if (i + 1 < argc) {
                    fgPath = argv[++i];
                } else {
                    cerr << "Error: --foreground requires a file path.\n";
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

    // Parse and validate any color overrides
    Color overrideSetup     = {0,0,0};
    Color overridePunchline = {0,0,0};
    Color overrideBorder    = {0,0,0};
    bool hasSetupColor      = false;
    bool hasPunchlineColor  = false;
    bool hasBorderColor     = false;
    if (!setupColorHex.empty()) {
        if (!parseHexColor(setupColorHex, overrideSetup)) {
            cerr << "Error: invalid hex color for --setupcolor: " << setupColorHex << "\n";
            return 1;
        }
        hasSetupColor = true;
    }
    if (!punchlineColorHex.empty()) {
        if (!parseHexColor(punchlineColorHex, overridePunchline)) {
            cerr << "Error: invalid hex color for --punchlinecolor: " << punchlineColorHex << "\n";
            return 1;
        }
        hasPunchlineColor = true;
    }
    if (!borderColorHex.empty()) {
        if (!parseHexColor(borderColorHex, overrideBorder)) {
            cerr << "Error: invalid hex color for --bordercolor: " << borderColorHex << "\n";
            return 1;
        }
        hasBorderColor = true;
    }

    // Helper to apply color overrides to a theme copy
    auto resolveTheme = [&](const Theme& base) {
        Theme t = base;
        if (hasSetupColor)     t.setup     = overrideSetup;
        if (hasPunchlineColor) t.punchline = overridePunchline;
        if (hasBorderColor)    t.border    = overrideBorder;
        return t;
    };

    bool usingCustomText = !customSetup.empty() || !customPunchline.empty();

    if (!usingCustomText && jokeNumber < 1) {
        cerr << "Error: joke number required. Use -p <number>, or supply --setup/--punchline.\n\n";
        displayHelp();
        return 1;
    }

    Joke joke;
    if (usingCustomText) {
        joke.setup     = customSetup;
        joke.punchline = customPunchline;
        joke.type      = "custom";
        if (jokeNumber < 1) jokeNumber = 0; // no number to display in footer
    } else {
        if (jokeNumber > (int)jokes.size()) {
            cerr << "Error: joke #" << jokeNumber << " does not exist. (Range: 1-" << jokes.size() << ")\n";
            return 1;
        }
        joke = jokes[jokeNumber - 1];
    }

    if (themeName == "all") {
        bool ok = true;
        for (auto& kv : THEMES) {
            string file = outputFile.empty()
                ? (usingCustomText ? "joke_custom_" + kv.first + ".jpg"
                                   : "joke_" + to_string(jokeNumber) + "_" + kv.first + ".jpg")
                : outputFile;
            ok &= jokeToJpeg(joke, jokeNumber, file, fontPath, resolveTheme(kv.second), noborder, bgPath, nodim, roundedcorners, nofooter, labelOverride, hasLabelOverride, textScale, fgPath);
        }
        return ok ? 0 : 1;
    }

    if (outputFile.empty())
        outputFile = usingCustomText ? "joke_custom_" + themeName + ".jpg"
                                     : "joke_" + to_string(jokeNumber) + "_" + themeName + ".jpg";

    return jokeToJpeg(joke, jokeNumber, outputFile, fontPath, resolveTheme(THEMES.at(themeName)), noborder, bgPath, nodim, roundedcorners, nofooter, labelOverride, hasLabelOverride, textScale, fgPath) ? 0 : 1;
}

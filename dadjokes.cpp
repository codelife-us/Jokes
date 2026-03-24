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
// Dad Jokes
//     by Code Life
//     command line version using modern C++20
//     version 1.0 - 2/02/2026
//     version 2.0 - 3/23/2026 - now using jokes_data.h for jokes and default to --color
//
// Description: A collection of Dad Jokes via command line.
//
// Command Line Options:
//   (none)               Display a single random joke (default)
//   see displayHelp() function for more options

#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include "jokes_data.h"
using namespace std;

void outputJoke(const Joke& joke, int jokeNumber, bool separateLines = false, bool colored = false, bool showNumber = false, bool showAll = false) {
    // ANSI color codes
    string cyanColor = "\033[36m";
    string greenColor = "\033[32m";
    string resetColor = "\033[0m";     // Reset to default
    
    if (showNumber) {
        cout << "[" << jokeNumber << "] ";
    }
    if (!colored) { // simple way to disable colors if not wanted
        cyanColor = "";
        greenColor = "";
        resetColor = "";
    }
    // if not showing all, add an extra newline after the setup for better readability
    if (separateLines) {
        cout << cyanColor << joke.setup << resetColor << "\n" << (showAll ? "" : "\n")
                    << greenColor << joke.punchline << resetColor << "\n\n";
    } else {
        cout << cyanColor << joke.setup << resetColor << ": "
                    << greenColor << joke.punchline << resetColor << "\n";
    }
}

void displayHelp() {
    cout << "Dad Jokes - A collection of Dad Jokes\n\n";
    cout << "Usage: ./dadjokes [OPTIONS]\n\n";
    cout << "Options:\n";
    cout << "  (none)                Display a single random joke (default)\n\n";
    cout << "  -a, --all             Display all jokes in shuffled order\n";
    cout << "  -1, --oneline         Display joke setup and punchline on one line with a colon separator\n";
    cout << "  -nc, --nocolor        Plain output (color is on by default)\n";
    cout << "  -sn, --shownumber     Display the number of the joke\n";
    cout << "  -p, --picknumber xxx  Display joke by number, supply number after\n";
    cout << "  -h, --help            Display this help message\n";
    cout << "Examples:\n";
    cout << "  ./dadjokes                       - Display one random joke\n";
    cout << "  ./dadjokes -a                    - Display all jokes shuffled\n";
    cout << "  ./dadjokes --nocolor             - Display one random joke without color\n";
    cout << "  ./dadjokes -a --nocolor -1       - Display all jokes without colors on one line\n";
    cout << "  ./dadjokes -p 2                  - Display joke #2\n";
}

int main(int argc, char* argv[]) {
    // Parse command line options
    bool together = false;      // -1 flag (default is separate lines)
    bool showAll = false;       // -a flag (default is single random)
    bool colored = true;        // color is on by default; use --nocolor to disable
    bool noMeaning = false;
    bool showNumber = false;    // -sn flag (default is false)
    bool pickJokeByNumber = false;
    int jokeNumber = 0;

    try {
        for (int i = 1; i < argc; i++) {
            if (string(argv[i]) == "-1" || string(argv[i]) == "--oneline") {
                together = true;
            } else if (string(argv[i]) == "-a" || string(argv[i]) == "--all") {
                showAll = true;
            } else if (string(argv[i]) == "-nc" || string(argv[i]) == "--nocolor") {
                colored = false;
            } else if (string(argv[i]) == "-sn" || string(argv[i]) == "--shownumber") {
                showNumber = true;
            } else if (string(argv[i]) == "-nm" || string(argv[i]) == "--nomeaning") {
                noMeaning = true;
            } else if (string(argv[i]) == "-p" || string(argv[i]) == "--pick") {
                pickJokeByNumber = true;
                jokeNumber = stoi(argv[i+1]);
            } else if (string(argv[i]) == "-h" || string(argv[i]) == "--help") {
                displayHelp();
                return 0;
            }
        }
    } catch (...) {
        cerr << "Error parsing command line arguments.\n";
        return 1;
    }   

    // Build filtered list of dad jokes from jokes_data.h
    vector<Joke> dadJokes;
    for (const auto& joke : jokes) {
        if (joke.type == "dad") {
            dadJokes.push_back(joke);
        }
    }

    // separateLines is now the default (true), unless -t is passed
    bool separateLines = !together;

    if (showAll) {
        // Display all sayings (shuffled)
        cout << "Dad Jokes - A collection of Dad Jokes\n";
        cout << "Brought to you by Code Life\n\n";

        vector<int> indices;
        for (int i = 0; i < (int)dadJokes.size(); i++) {
            indices.push_back(i);
        }
        random_device rd;
        mt19937 gen(rd());
        shuffle(indices.begin(), indices.end(), gen);

        for (int idx : indices) {
            outputJoke(dadJokes[idx], idx + 1, separateLines, colored, showNumber, showAll);
        }

        cout << "\nTotal jokes: " << dadJokes.size() << "\n";
    } else {
        // Default: Display a single random saying
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, (int)dadJokes.size() - 1);

        int jokeIdx = dis(gen);
        if (pickJokeByNumber) {
            jokeIdx = jokeNumber - 1;
            if (jokeIdx < 0 || jokeIdx >= (int)dadJokes.size()) {
                cerr << "Joke number out of range. Please pick a number between 1 and " << dadJokes.size() << ".\n";
                return 1;
            }
        }
        outputJoke(dadJokes[jokeIdx], jokeIdx + 1, separateLines, colored, showNumber, showAll);
    }
    return 0;
}

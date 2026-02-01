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
//     version 1.0 - 1/28/2026
// Description: A collection of Dad Jokes via command line.
//
// Command Line Options:
//   (none)               Display a single random joke (default)
//   -a,  --all           Display all jokes in shuffled order
//   -t,  --together      Display setup and punchline on one line with colon separator
//   -c,  --color         Colored output: pink for setup, orange for punchline
//   -sn, --shownumber    Display the number of the joke  
//   -h,  --help          Display this help message

#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <unordered_map>
using namespace std;

struct DadJoke {
    string setup;
    string punchline;
};

const unordered_map<int, DadJoke> dadJokes = {
    {1, {"Why did the stadium get hot after the game?", "All of the fans left."}},
    {2, {"What did the janitor say when he jumped out of the closet?", "Supplies!"}},
    {3, {"How do you organize a space party?", "You planet."}},
    {4, {"What do you call a parade of rabbits hopping backwards?", "A receding hare-line."}},
    {5, {"Why do cows wear bells?", "Because their horns don't work."}},
    {6, {"I'm on a seafood diet.", "I see food and I eat it."}},
    {7, {"What do you call a sleepwalking nun?", "A roamin' Catholic."}},
    {8, {"Why did the coffee file a police report?", "It got mugged."}},
    {9, {"What do you call a pile of kittens?", "A meow-ntain."}},
    {10, {"I'll tell you a joke about a roof.", "Never mind, it's over your head."}},
    {11, {"What did the ocean say to the pirate?", "Nothing, it just waved."}},
    {12, {"Why can’t you trust atoms?", "Because they make up everything."}},
    {13, {"What do you call a snobby criminal going down the stairs?", "A condescending con, descending."}},
    {14, {"To the man in the wheelchair who stole my camouflage jacket...", "You can hide, but you can't run."}},
    {15, {"What did the fish say when he hit the wall?", "Dam."}},
    {16, {"Why did the private tip-toe past the medicine cabinet?", "He didn't want to wake the sleeping pills."}},
    {17, {"What’s the difference between a hippo and a zippo?", "One is really heavy, and the other is a little lighter."}},
    {18, {"Why don't eggs tell jokes?", "They'd crack each other up."}},
    {19, {"I used to play piano by ear...", "But now I use my hands."}},
    {20, {"Why did the bicycle fall over?", "Because it was two-tired."}},
    {21, {"What do you call a man with no body and no nose?", "Nobody nose."}},
    {22, {"I'm reading a book on anti-gravity.", "It's impossible to put down."}},
    {23, {"Did you hear about the kidnapping at school?", "It's okay, he woke up."}},
    {24, {"What do you call a bear with no teeth?", "A gummy bear."}},
    {25, {"Why did the mushroom go to the party?", "Because he was a fungi."}},
    {26, {"What do you call fake spaghetti?", "An impasta."}},
    {27, {"Why did the scarecrow win an award?", "Because he was outstanding in his field."}},
    {28, {"Why don't skeletons fight each other?", "They don't have the guts."}},
    {29, {"What do you call cheese that isn't yours?", "Nacho cheese."}},
    {30, {"Why did the golfer bring two pairs of pants?", "In case he got a hole in one."}},
    {31, {"What do you call an alligator in a vest?", "An investigator."}},
    {32, {"Why did the tomato turn red?", "Because it saw the salad dressing."}},
    {33, {"How does a penguin build its house?", "Igloos it together."}},
    {34, {"What do you call a factory that makes good products?", "A satisfactory."}},
    {35, {"Why did the cookie go to the hospital?", "Because he felt crummy."}},
    {36, {"What do you call a dinosaur with an extensive vocabulary?", "A thesaurus."}},
    {37, {"Why did the math book look sad?", "Because it had too many problems."}},
    {38, {"What do you call a cow with no legs?", "Ground beef."}},
    {39, {"Why was the big cat disqualified from the race?", "Because it was a cheetah."}},
    {40, {"What do you call a fish wearing a bowtie?", "Sofishticated."}},
    {41, {"Waitress: How did you find your steak Sir?","I just looked next to the potatoes and there it was."}},
    {42, {"If you play guitar and want to know the secret to making it sound better,","Please stay tuned..."}},
    {43, {"5 ants rented an apartment with another 5 ants.","Now they're tenants"}},
    {44, {"What's the best present you can gift?","A broken drum.\nNobody can beat that."}},
    {45, {"What did the buffalo say when his son left?","Bison"}},
    {46, {"My daughter asked me if I had seen the dog bowl.","Me: No, I didn't even know he could."}},
    {47, {"Why didn't the toilet paper make it across the street?","It got stuck in the crack."}},
    {48, {"So far, Humpty Dumpty is having a terrible winter.","Which is frustrating, because he had a great fall."}},
    {49, {"What do you call a nervous javelin thrower?","Shakespeare."}},
    {50, {"What do you call a sketchy neighborhood in Italy?","The spaghetto."}},
    {51, {"Why did the picture go to jail?","Because it was framed."}},
    {52, {"Did you hear about the fire at the circus?", "It was in tents."}},
    {53, {"I don't trust stairs.", "They're always up to something."}},
    {54, {"What do you call a dog that does magic tricks?", "A labracadabra."}},
    {55, {"Why did the old man fall in the well?", "Because he couldn't see that well."}},
    {56, {"I ordered a chicken and an egg from Amazon.", "I'll let you know."}},
    {57, {"What do you call a belt made out of watches?", "A waist of time."}},
    {58, {"What kind of shoes do ninjas wear?", "Sneakers."}},
    {59, {"How do you make a tissue dance?", "You put a little boogie in it."}},
    {60, {"Why did the math teacher open a window?", "To let out the acute angles."}},
    {61, {"What do you call a pig that does karate?", "A pork chop."}},
    {62, {"I thought about going on an all-almond diet.", "But that's just nuts."}},
    {63, {"Why don't some couples go to the gym?", "Because some relationships don't work out."}},
    {64, {"What do you call a bee that can't make up its mind?", "A maybe."}},
    {65, {"I would tell you a joke about time travel.", "But you didn't like it."}},
    {66, {"Why do seagulls fly over the ocean?", "Because if they flew over the bay, they'd be bagels."}},
    {67, {"What do you call a bear with no ears?", "B."}},
    {68, {"Did you hear about the guy who invented the knock-knock joke?", "He won the 'no-bell' prize."}},
    {69, {"I only know 25 letters of the alphabet.", "I don't know y."}},
    {70, {"What has more letters than the alphabet?", "The post office."}},
    {71, {"Why did the frog take the bus to work?", "His car got toad."}},
    {72, {"I've got a great joke about construction.", "I'm still working on it."}},
    {73, {"What did the grape do when he got stepped on?", "He let out a little wine."}},
    {74, {"I used to be addicted to soap.", "But I'm clean now."}},
    {75, {"Why are elevator jokes so good?", "They work on so many levels."}},
    {76, {"What do you call a pony with a cough?", "A little hoarse."}},
    {77, {"Did you hear about the guy who stole a calendar?", "He got 12 months."}},
    {78, {"Why did the invisible man turn down the job offer?", "He couldn't see himself doing it."}},
    {79, {"What does a sprinter eat before a race?", "Nothing, they fast."}},
    {80, {"Why do melons have weddings?", "Because they cantaloupe."}},
    {81, {"What do clouds wear under their shorts?", "Thunderwear."}},
    {82, {"What did the zero say to the eight?", "That belt looks good on you."}},
    {83, {"What kind of car does an egg drive?", "A yolkswagen."}},
    {84, {"Why was the computer cold?", "It left its Windows open."}},
    {85, {"Where do bees go to the bathroom?", "The BP station."}},
    {86, {"Why do golfers love donuts?", "Because there's a hole in the middle."}},
    {87, {"News: A man lost his left arm and left leg in a car crash.", "He's all right now."}},
    {88, {"What kind of candy do astronauts like?", "Mars bars."}},
    {89, {"What did one wall say to the other?", "I'll meet you at the corner."}},
    {90, {"I think I'm sick of taking the elevator.", "I think I'm coming down with something."}},
    {91, {"What did the termite say when he walked into the bar?", "Is the bar tender here?"}},
    {92, {"What do you call a sheep that knows karate?", "A lamb chop."}},
    {93, {"Why did the tree go to the dentist?", "It needed a root canal."}},
    {94, {"Why is the obtuse triangle always upset?", "Because it is never right."}},
    {95, {"Did you hear about the Italian chef that died?", "He pasta way."}},
    {96, {"What is an astronaut's favorite part of a computer?", "The space bar."}},
    {97, {"Why did the computer go to the doctor?", "Because it had a virus."}},
    {98, {"Did you hear about the restaurant on the moon?", "Great food, no atmosphere."}},
    {99, {"Why do river banks work so well?", "They have high current-cy."}},
    {100, {"Have you heard about the new corduroy pillows?", "They're making headlines."}}        
};

void outputJoke(const DadJoke& joke, int jokeNumber, bool separateLines = false, bool colored = false, bool showNumber = false) {
    // ANSI color codes
    const string cyanColor = "\033[36m";
    const string greenColor = "\033[32m";
    const string resetColor = "\033[0m";     // Reset to default
    
    if (showNumber) {
        std::cout << "[" << jokeNumber << "] ";
    }
    
    if (colored) {
        if (separateLines) {
            std::cout << cyanColor << joke.setup << resetColor << "\n"
                      << greenColor << joke.punchline << resetColor << "\n\n";
        } else {
            std::cout << cyanColor << joke.setup << resetColor << ": "
                      << greenColor << joke.punchline << resetColor << "\n";
        }
    } else {
        if (separateLines) {
            std::cout << joke.setup << "\n" << joke.punchline << "\n\n";
        } else {
            std::cout << joke.setup << ": " << joke.punchline << "\n";
        }
    }
}

void displayHelp() {
    std::cout << "Dad Jokes - A collection of Dad Jokes\n\n";
    std::cout << "Usage: ./dadjokes [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  (none)                Display a single random joke (default)\n\n";
    std::cout << "  -a, --all             Display all jokes in shuffled order\n";
    std::cout << "  -t, --together        Display joke setup and punchline on one line with a colon separator\n";
    std::cout << "  -c, --color           Colored output: pink for saying, orange for meaning\n";
    std::cout << "  -sn, --shownumber     Display the number of the joke\n";
    std::cout << "  -p, --picknumber xxx  Display joke by number, supply number after\n";
    std::cout << "  -h, --help            Display this help message\n";
    std::cout << "Examples:\n";
    std::cout << "  ./dadjokes                       - Display one random joke\n";
    std::cout << "  ./dadjokes -a                    - Display all jokes shuffled\n";
    std::cout << "  ./dadjokes --color               - Display one random joke in color\n";
    std::cout << "  ./dadjokes -a --color -t         - Display all jokes with colors on one line\n";
    std::cout << "  ./dadjokes -p 2                  - Display joke #2\n";
}

int main(int argc, char* argv[]) {
    // Parse command line options
    bool together = false;      // -t flag (default is separate lines)
    bool showAll = false;       // -a flag (default is single random)
    bool colored = false;
    bool noMeaning = false;
    bool showNumber = false;    // -sn flag (default is false)
    bool pickJokeByNumber = false;
    int jokeNumber = 0;

    try {
        for (int i = 1; i < argc; i++) {
            if (string(argv[i]) == "-t" || string(argv[i]) == "--together") {
                together = true;
            } else if (string(argv[i]) == "-a" || string(argv[i]) == "--all") {
                showAll = true;
            } else if (string(argv[i]) == "-c" || string(argv[i]) == "--color") {
                colored = true;
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
        std::cerr << "Error parsing command line arguments.\n";
        return 1;
    }   

    // separateLines is now the default (true), unless -t is passed
    bool separateLines = !together;
    
    if (showAll) {
        // Display all sayings (shuffled)
        std::cout << "Dad Jokes - A collection of Dad Jokes\n";
        std::cout << "Brought to you by Code Life\n\n";
        
        vector<int> jokeKeys;
        for (const auto& pair : dadJokes) {
            jokeKeys.push_back(pair.first);
        }
        random_device rd;
        mt19937 gen(rd());
        std::shuffle(jokeKeys.begin(), jokeKeys.end(), gen);
        
        for (int key : jokeKeys) {
            // Print each saying and its meaning
            outputJoke(dadJokes.at(key), key, separateLines, colored, showNumber);
        }
        
        std::cout << "\nTotal jokes: " << dadJokes.size() << "\n";
    } else {
        // Default: Display a single random saying
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, dadJokes.size());
        
        int jokeKey = dis(gen);
        if (pickJokeByNumber) {
            jokeKey = jokeNumber;
            if (jokeKey < 1 || jokeKey > dadJokes.size()) {
                std::cerr << "Joke number out of range. Please pick a number between 1 and " << dadJokes.size() << ".\n";
                return 1;
            }
        }
        const auto& joke = dadJokes.at(jokeKey);
        outputJoke(joke, jokeKey, separateLines, colored, showNumber);
    }
    return 0;
}
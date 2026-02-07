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
// Jokes
//     by Code Life
//     command line version using modern C++
//     version 1.1 - 2/06/2026
// Description: A collection of jokes via command line.

#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <numeric> // Required for std::iota
using namespace std;

struct Joke {
    string type;   // e.g. "dad", "knock-knock", "pun", "math", "tech"
    string setup;
    string punchline;
};

// IDs are now implicit (Index + 1).
const vector<Joke> jokes = {
    {"dad", "Why did the stadium get hot after the game?", "All of the fans left."},
    {"dad", "What did the janitor say when he jumped out of the closet?", "Supplies!"},
    {"dad", "How do you organize a space party?", "You planet."},
    {"dad", "What do you call a parade of rabbits hopping backwards?", "A receding hare-line."},
    {"dad", "Why do cows wear bells?", "Because their horns don't work."},
    {"dad", "I'm on a seafood diet.", "I see food and I eat it."},
    {"dad", "What do you call a sleepwalking nun?", "A roamin' Catholic."},
    {"dad", "Why did the coffee file a police report?", "It got mugged."},
    {"dad", "What do you call a pile of kittens?", "A meow-ntain."},
    {"dad", "I'll tell you a joke about a roof.", "Never mind, it's over your head."},
    {"dad", "What did the ocean say to the pirate?", "Nothing, it just waved."},
    {"dad", "Why can’t you trust atoms?", "Because they make up everything."},
    {"dad", "What do you call a snobby criminal going down the stairs?", "A condescending con, descending."},
    {"dad", "To the man in the wheelchair who stole my camouflage jacket...", "You can hide, but you can't run."},
    {"dad", "What did the fish say when he hit the wall?", "Dam."},
    {"dad", "Why did the private tip-toe past the medicine cabinet?", "He didn't want to wake the sleeping pills."},
    {"dad", "What’s the difference between a hippo and a zippo?", "One is really heavy, and the other is a little lighter."},
    {"dad", "Why don't eggs tell jokes?", "They'd crack each other up."},
    {"dad", "I used to play piano by ear...", "But now I use my hands."},
    {"dad", "Why did the bicycle fall over?", "Because it was two-tired."},
    {"dad", "What do you call a man with no body and no nose?", "Nobody nose."},
    {"dad", "I'm reading a book on anti-gravity.", "It's impossible to put down."},
    {"dad", "Did you hear about the kidnapping at school?", "It's okay, he woke up."},
    {"dad", "What do you call a bear with no teeth?", "A gummy bear."},
    {"dad", "Why did the mushroom go to the party?", "Because he was a fungi."},
    {"dad", "What do you call fake spaghetti?", "An impasta."},
    {"dad", "Why did the scarecrow win an award?", "Because he was outstanding in his field."},
    {"dad", "Why don't skeletons fight each other?", "They don't have the guts."},
    {"dad", "What do you call cheese that isn't yours?", "Nacho cheese."},
    {"dad", "Why did the golfer bring two pairs of pants?", "In case he got a hole in one."},
    {"dad", "What do you call an alligator in a vest?", "An investigator."},
    {"dad", "Why did the tomato turn red?", "Because it saw the salad dressing."},
    {"dad", "How does a penguin build its house?", "Igloos it together."},
    {"dad", "What do you call a factory that makes good products?", "A satisfactory."},
    {"dad", "Why did the cookie go to the hospital?", "Because he felt crummy."},
    {"dad", "What do you call a dinosaur with an extensive vocabulary?", "A thesaurus."},
    {"dad", "Why did the math book look sad?", "Because it had too many problems."},
    {"dad", "What do you call a cow with no legs?", "Ground beef."},
    {"dad", "Why was the big cat disqualified from the race?", "Because it was a cheetah."},
    {"dad", "What do you call a fish wearing a bowtie?", "Sofishticated."},
    {"dad", "Waitress: How did you find your steak Sir?","I just looked next to the potatoes and there it was."},
    {"dad", "If you play guitar and want to know the secret to making it sound better,","Please stay tuned..."},
    {"dad", "5 ants rented an apartment with another 5 ants.","Now they're tenants"},
    {"dad", "What's the best present you can gift?","A broken drum.\nNobody can beat that."},
    {"dad", "What did the buffalo say when his son left?","Bison"},
    {"dad", "My daughter asked me if I had seen the dog bowl.","Me: No, I didn't even know he could."},
    {"dad", "Why didn't the toilet paper make it across the street?","It got stuck in the crack."},
    {"dad", "So far, Humpty Dumpty is having a terrible winter.","Which is frustrating, because he had a great fall."},
    {"dad", "What do you call a nervous javelin thrower?","Shakespeare."},
    {"dad", "What do you call a sketchy neighborhood in Italy?","The spaghetto."},
    {"dad", "Why did the picture go to jail?","Because it was framed."},
    {"dad", "Did you hear about the fire at the circus?", "It was in tents."},
    {"dad", "I don't trust stairs.", "They're always up to something."},
    {"dad", "What do you call a dog that does magic tricks?", "A labracadabra."},
    {"dad", "Why did the old man fall in the well?", "Because he couldn't see that well."},
    {"dad", "I ordered a chicken and an egg from Amazon.", "I'll let you know."},
    {"dad", "What do you call a belt made out of watches?", "A waist of time."},
    {"dad", "What kind of shoes do ninjas wear?", "Sneakers."},
    {"dad", "How do you make a tissue dance?", "You put a little boogie in it."},
    {"dad", "Why did the math teacher open a window?", "To let out the acute angles."},
    {"dad", "What do you call a pig that does karate?", "A pork chop."},
    {"dad", "I thought about going on an all-almond diet.", "But that's just nuts."},
    {"dad", "Why don't some couples go to the gym?", "Because some relationships don't work out."},
    {"dad", "What do you call a bee that can't make up its mind?", "A maybe."},
    {"dad", "I would tell you a joke about time travel.", "But you didn't like it."},
    {"dad", "Why do seagulls fly over the ocean?", "Because if they flew over the bay, they'd be bagels."},
    {"dad", "What do you call a bear with no ears?", "B."},
    {"dad", "Did you hear about the guy who invented the knock-knock joke?", "He won the 'no-bell' prize."},
    {"dad", "I only know 25 letters of the alphabet.", "I don't know y."},
    {"dad", "What has more letters than the alphabet?", "The post office."},
    {"dad", "Why did the frog take the bus to work?", "His car got toad."},
    {"dad", "I've got a great joke about construction.", "I'm still working on it."},
    {"dad", "What did the grape do when he got stepped on?", "He let out a little wine."},
    {"dad", "I used to be addicted to soap.", "But I'm clean now."},
    {"dad", "Why are elevator jokes so good?", "They work on so many levels."},
    {"dad", "What do you call a pony with a cough?", "A little hoarse."},
    {"dad", "Did you hear about the guy who stole a calendar?", "He got 12 months."},
    {"dad", "Why did the invisible man turn down the job offer?", "He couldn't see himself doing it."},
    {"dad", "What does a sprinter eat before a race?", "Nothing, they fast."},
    {"dad", "Why do melons have weddings?", "Because they cantaloupe."},
    {"dad", "What do clouds wear under their shorts?", "Thunderwear."},
    {"dad", "What did the zero say to the eight?", "That belt looks good on you."},
    {"dad", "What kind of car does an egg drive?", "A yolkswagen."},
    {"dad", "Why was the computer cold?", "It left its Windows open."},
    {"dad", "Where do bees go to the bathroom?", "The BP station."},
    {"dad", "Why do golfers love donuts?", "Because there's a hole in the middle."},
    {"dad", "News: A man lost his left arm and left leg in a car crash.", "He's all right now."},
    {"dad", "What kind of candy do astronauts like?", "Mars bars."},
    {"dad", "What did one wall say to the other?", "I'll meet you at the corner."},
    {"dad", "I think I'm sick of taking the elevator.", "I think I'm coming down with something."},
    {"dad", "What did the termite say when he walked into the bar?", "Is the bar tender here?"},
    {"dad", "What do you call a sheep that knows karate?", "A lamb chop."},
    {"dad", "Why did the tree go to the dentist?", "It needed a root canal."},
    {"dad", "Why is the obtuse triangle always upset?", "Because it is never right."},
    {"dad", "Did you hear about the Italian chef that died?", "He pasta way."},
    {"dad", "What is an astronaut's favorite part of a computer?", "The space bar."},
    {"dad", "Why did the computer go to the doctor?", "Because it had a virus."},
    {"dad", "Did you hear about the restaurant on the moon?", "Great food, no atmosphere."},
    {"dad", "Why do river banks work so well?", "They have high current-cy."},
    {"dad", "Have you heard about the new corduroy pillows?", "They're making headlines."},
    {"dad", "Every morning for the past three months, I announce loudly to the family that I'm going for a jog, and then I don't","It's my longest running joke of this year."},
    {"dad", "Why do ghosts love elevators?","Because it lifts their spirits."},
    {"dad", "Why did the girl keep her trumpet out in the snow?","Because she liked cool jazz."},
    {"dad", "A weasel walks into a bar, and the bartender says,\n\"Wow, I've never seen a weasel before. What can I get you?\"","\"Pop,\" goes the weasel"},
    {"dad", "How do you console an English teacher?","There, their, they're."},
    {"dad", "My dental hygienist won a national award for teeth cleaning.","She proudly displays the plaque on her desk."},
    {"dad", "What do you call a man with a rubber toe?", "Roberto."},
    {"dad", "What do you call a fat psychic?", "A four-chin teller."},
    {"dad", "My documentary film \"I have stolen all your chairs\"", "just received a 90 minute standing ovation."},
    {"dad", "I found a coin on the ground in the parking lot today. It had teeth marks on it.","It was a bitcoin."},
    {"dad", "Due to the storm I ran out of toilet paper and had to use newspaper.", "The Times are rough."},
    {"tech", "What’s the biggest lie in the entire universe?", "I have read and agree to the Terms & Conditions."},
    {"tech", "Why did the man get fired from his job at the keyboard factory?", "He wasn’t putting in enough shifts."},
    {"pun", "To the guy who invented zero,", "Thanks for nothing."},
    {"pun", "I had a crazy dream last night! I was swimming in an ocean of orange soda.", "Turns out it was just a Fanta sea."},
    {"knock-knock", "Shore", "Shore hope you like bad jokes!"},
    {"knock-knock", "Euripides.", "Euripides clothes, you buy them!"},
    {"dad", "I sing when I'm in my car, but only in reverse.", "I'm a backup singer."},
    {"math", "Why didn't the number 4 get into the nightclub?", "Because he is 2 square."},
    {"math", "Why was the equal sign so humble?", "Because it knew it wasn't less than or greater than anyone else."},
    {"math", "Why do plants hate math?", "Because it gives them square roots."},
    {"math", "What do you call a number that can't keep still?", "A roamin' numeral."},
    {"math", "Why was the math book sad?", "Because it had too many problems."},
    {"math", "What do you call a snake that is 3.14 meters long?", "A pi-thon."},
    {"math", "Why did the student do multiplication problems on the floor?", "The teacher told him not to use tables."},
    {"dad", "What do you call a dad that has fallen through the ice?","A Popsicle."},
    {"dad", "Dad at breakfast: I’ll have bacon and eggs, please. Waiter: How do you like your eggs?", "Dad: I don't know, I don't have them yet."}
};

void outputJoke(const Joke& joke, int jokeNumber, bool separateLines = false, bool colored = false, bool showNumber = false, bool showType = false) {
    // ANSI color codes
    string cyanColor = "\033[36m";
    string greenColor = "\033[32m";
    string resetColor = "\033[0m";     // Reset to default
    
    if (showNumber) {
        cout << "[" << jokeNumber << "]";
        if (!showType) {
            cout << " ";
        }
    }
    if (showType) {
        cout << "[" << joke.type << "] ";
    }
    if (!colored) { // simple way to disable colors if not wanted
        cyanColor = "";
        greenColor = "";
        resetColor = "";
    }
    if (separateLines) {
        cout << cyanColor << (joke.type == "knock-knock" ? "Knock knock!\nWho's there?\n" : "")
                << joke.setup << (joke.type == "knock-knock" ? "\n" + joke.setup + " who?" : "") << resetColor << "\n"
                << greenColor << joke.punchline << resetColor << "\n\n";
    } else {
        cout << cyanColor << (joke.type == "knock-knock" ? "Knock knock! Who's there? " : "")
                << joke.setup << (joke.type == "knock-knock" ? " " + joke.setup + " who?" : "") << resetColor
                << greenColor << ": " << joke.punchline << resetColor << "\n";
    }
}

void displayHelp() {
    cout << "Jokes - A collection of jokes\n\n";
    cout << "Usage: ./jokes [OPTIONS]\n\n";
    cout << "Options:\n";
    cout << "  (none)                Display a single random joke (default)\n\n";
    cout << "  -a, --all             Display all jokes in shuffled order\n";
    cout << "  -1, --oneline         Display joke setup and punchline on one line with a colon separator\n";
    cout << "  -c, --color           Colored output: cyan for joke, green for punchline\n";
    cout << "  -sn, --shownumber     Display the number of the joke\n";
    cout << "  -st, --showtype       Display the type of the joke\n";
    cout << "  -p, --picknumber xxx  Display joke by number, supply number after\n";
    cout << "  -h, --help            Display this help message\n";
    cout << "Examples:\n";
    cout << "  ./jokes                       - Display one random joke\n";
    cout << "  ./jokes -a                    - Display all jokes shuffled\n";
    cout << "  ./jokes --color               - Display one random joke in color\n";
    cout << "  ./jokes -a --color -1         - Display all jokes with colors on one line\n";
    cout << "  ./jokes -p 2                  - Display joke #2\n";
}

int main(int argc, char* argv[]) {
    // Parse command line options
    bool together = false;      // -1 flag (default is separate lines)
    bool showAll = false;       // -a flag (default is single random)
    bool colored = false;
    bool filterType = false;
    bool showNumber = false;    // -sn flag (default is false)
    bool showType = false;      // -st flag (default is false)
    bool pickJokeByNumber = false;
    int jokeNumber = 0;
    string type = "all"; // default type filter is "all" (no filtering)

    try {
        for (int i = 1; i < argc; i++) {
            string arg = argv[i];

            if (arg == "-1" || arg == "--oneline") {
                together = true;
            } else if (arg == "-a" || arg == "--all") {
                showAll = true;
            } else if (arg == "-c" || arg == "--color") {
                colored = true;
            } else if (arg == "-sn" || arg == "--shownumber") {
                showNumber = true;
            } else if (arg == "-st" || arg == "--showtype") {
                showType = true;
            } else if (arg == "-t" || arg == "--type") {
                if (i + 1 < argc) {
                    filterType = true;
                    type = argv[++i]; // Skip to next arg
                    if (type != "dad" && type != "knock-knock" && type != "pun" && type != "math" && type != "tech" && type != "all") {
                        cerr << "Invalid joke type specified. Valid types are: dad, knock-knock, tech, pun, math, all.\n";
                        return 1;
                    }
                } else {
                    cerr << "Error: --type option requires an argument.\n";
                    return 1;
                }
            } else if (arg == "-p" || arg == "--pick") {
                if (i + 1 < argc) {
                    pickJokeByNumber = true;
                    try {
                        jokeNumber = stoi(argv[++i]); // Skip to next arg
                    } catch (...) {
                        cerr << "Error: --pick requires a valid number.\n";
                        return 1;
                    }
                } else {
                    cerr << "Error: --pick option requires a number.\n";
                    return 1;
                }
            } else if (arg == "-h" || arg == "--help") {
                displayHelp();
                return 0;
            }
        }
    } catch (...) {
        cerr << "Error parsing command line arguments.\n";
        return 1;
    }   

    bool separateLines = !together;
    
    if (showAll) {
        // Display all jokes (shuffled)
        cout << "Jokes - A collection of jokes\n";
        cout << "Brought to you by Code Life\n\n";
        
        // 1. Create a list of indices: 0, 1, 2, ... N
        vector<int> indices(jokes.size());
        iota(indices.begin(), indices.end(), 0); 
        
        // 2. Shuffle the indices
        random_device rd;
        mt19937 gen(rd());
        shuffle(indices.begin(), indices.end(), gen);
        
        int jokeCount = 0;
        for (int i : indices) {
            // Apply Type Filter
            if (filterType && type != "all" && jokes[i].type != type) {
                continue; 
            }
            // Pass (i + 1) so it displays as "Joke [1]" not "Joke [0]"
            outputJoke(jokes[i], i + 1, separateLines, colored, showNumber, showType);
            jokeCount++;
        }
        
        cout << "\nTotal jokes: " << jokeCount << "\n";
    } else {
        // Logic for displaying a SINGLE joke (Random or Specific)
        int selectedIndex = -1;

        if (pickJokeByNumber) {
            // 1. User specified a specific number
            // Vector is 0-indexed, so we subtract 1 from user input
            if (jokeNumber >= 1 && jokeNumber <= (int)jokes.size()) {
                selectedIndex = jokeNumber - 1;
            } else {
                cerr << "Error: Joke #" << jokeNumber << " does not exist. (Range: 1-" << jokes.size() << ")\n";
                return 1;
            }
        } else {
            // 2. Random selection (must respect filters!)
            vector<int> validIndices;
            
            for (size_t i = 0; i < jokes.size(); ++i) {
                // Check if this joke matches the requested type
                if (!filterType || type == "all" || jokes[i].type == type) {
                    validIndices.push_back(i);
                }
            }

            if (validIndices.empty()) {
                cerr << "No jokes found matching type: " << type << "\n";
                return 1;
            }

            // Securely pick a random index from the VALID indices only
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, validIndices.size() - 1);
            
            selectedIndex = validIndices[dis(gen)];
        }

        // Output the final selected joke
        outputJoke(jokes[selectedIndex], selectedIndex + 1, separateLines, colored, showNumber, showType);
    }
    return 0;
}

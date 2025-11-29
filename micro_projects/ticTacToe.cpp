
#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include <io.h>
#include <chrono>
#include <thread>


using std::cin;
using std::cout;
using std::string;

// Function declarations with descriptions
void displayMainMenu(string *spaces, string *colourScheme, int *score, char *playersIcon);  // Displays and handles main menu
void drawBoard(string *spaces, string *colourScheme);                                       // Clears and draws the game board
void displayOptions(string *colourScheme, char *playersIcon, int *score);                   // Displays and handles game options
void userChangeColourScheme(string *colourScheme, int colourSchemeDigit);                   // Allows user to change color schemes
void singlePlayerGame(string *spaces, int *score, string *colourScheme, char *playersIcon); // Single-player game logic
void changePlayersIcon(string *colourScheme, char *playersIcon);                            // Allows icon customization
bool checkTie(string *spaces);                                                              // Checks if the game is a tie
bool checkWin(string *spaces);                                                              // Checks if any player has won
void botMove(string *spaces, char *playersIcon);                                            // Handles bot's move in single-player mode
void multiPlayerGame(string *spaces, int *score, string *colourScheme, char *playersIcon);  // Multiplayer game logic


int main()
{   
    // colourScheme handles the colours of everything in the game.
    // 0: The border of the game. 1: Text and the "[]" around the X O
    // 2: playerIcons colour
    string colourScheme[] = {"\033[38;2;255;191;0m", "\033[36m", "\033[35m", "\033[37m"};

    // Carries the game board space.
    string spaces[] = {" ", " ", " ", " ", " ", " ", " ", " ", " "};

    // The score of player 1 - 0: wins 1: losses 2: Ties
    int score[] = {0, 0, 0};

    // Contains players Icons that can change later
    char playersIcon [] = {'X', 'O'};

    // setting up rand function here so i dont set it up multiple times by using botMove
    srand(time(0));


    // Display main menu. It clears the console on its own every time.
    displayMainMenu (spaces, colourScheme, score, playersIcon);

    // Handle case of exiting the game:
    cout << "\033[2J\033[1;1H";  // clears console 
    cout << "THANKS FOR PLAYING!!\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));   // wait 2 seconds
    cout << "\033[2J\033[1;1H";  // clears console 

    // terminate game iff user chose 4 for while displaying displayMainMenu
    return 0;
}

void drawBoard(string *spaces, string *colourScheme)
{   
    // clears console and Draws the game board with the current state and colors
    // Parameters:
    // - spaces: Array representing the board's state
    // - colourScheme: Array for the color scheme settings
    
    cout << "\033[2J\033[1;1H";  // clears console 
    string temp[9]; // initialize a temp string 

    // Assign colors to spaces based on the current color scheme and reset to inside border colour after
    for (int i = 0; i < 9; i++)
    {
        temp [i] = spaces[i];
        temp[i] = colourScheme[2] + temp[i] + colourScheme[1];
    }

    _setmode(_fileno(stdout), _O_U16TEXT); // Set console to accept Unicode characters
    wprintf(L"%s╔═══════════════════════════════════════════════════════╗\n",          colourScheme[0].c_str());
    wprintf(L"%s║%s  _______ _        _______           _______           %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s |__   __(_)      |__   __|         |__   __|          %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |   _  ___     | | __ _  ___     | | ___   ___   %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |  | |/ __|    | |/ _` |/ __|    | |/ _ \\ / _ \\  %s║\n",    colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |  | | (__     | | (_| | (__     | | (_) |  __/  %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    |_|  |_|\\___|    |_|\\__,_|\\___|    |_|\\___/ \\___|  %s║\n", colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║                      %s[%s] [%s] [%s]%s                      ║\n",   colourScheme[0].c_str(), colourScheme[1].c_str(), temp[0].c_str(), temp[1].c_str(), temp[2].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║                      %s[%s] [%s] [%s]%s                      ║\n",   colourScheme[0].c_str(), colourScheme[1].c_str(), temp[3].c_str(), temp[4].c_str(), temp[5].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║                      %s[%s] [%s] [%s]%s                      ║\n",   colourScheme[0].c_str(), colourScheme[1].c_str(), temp[6].c_str(), temp[7].c_str(), temp[8].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s╚═══════════════════════════════════════════════════════╝\n",          colourScheme[0].c_str());
    wprintf(L"\033[0m");                // Reset color
    _setmode(_fileno(stdout), _O_TEXT); // Reset console mode
}

void displayMainMenu (string *spaces, string *colourScheme, int *score, char *playersIcon)
{   
    // Displays the main menu and handles its options
    // Parameters:
    // - spaces: Array representing the current state of the game board
    // - colourScheme: Array for the color scheme settings
    // - score: Array to track scores [wins, losses, ties]
    // - playersIcon: Array of characters representing the icons for Player 1 and Player 2/Bot

    int mainMenuChoice;

    // do while loop allowing exit iff the user chooses to exit (mainMenuChoice == 4)
    do{
    for (int i = 0; i<9; i++) {spaces[i] = " ";}

            cout << "\033[2J\033[1;1H";  // clears console 
    _setmode(_fileno(stdout), _O_U16TEXT);  // setmode to accept U16 characters
    wprintf(L"%s╔═══════════════════════════════════════════════════════╗\n",          colourScheme[0].c_str());
    wprintf(L"%s║%s  _______ _        _______           _______           %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s |__   __(_)      |__   __|         |__   __|          %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |   _  ___     | | __ _  ___     | | ___   ___   %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |  | |/ __|    | |/ _` |/ __|    | |/ _ \\ / _ \\  %s║\n",    colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |  | | (__     | | (_| | (__     | | (_) |  __/  %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    |_|  |_|\\___|    |_|\\__,_|\\___|    |_|\\___/ \\___|  %s║\n", colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║%s     P1:     Wins: %-7dLosses: %-7dDraws: %-7d%s║\n", colourScheme[0].c_str(), colourScheme[1].c_str(), score[0], score[1], score[2], colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║%s                      1. Play Game                     %s║\n",   colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s                      2. P.V.P                         %s║\n",   colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s                      3. Options                       %s║\n",   colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s                      4. Exit                          %s║\n",   colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s╚═══════════════════════════════════════════════════════╝\n",          colourScheme[0].c_str());
    wprintf(L"\033[0m");                    // Reset the console color to default
    _setmode(_fileno(stdout), _O_TEXT);     // reset from U16 to default text

        cin >> mainMenuChoice;                                          // Get user choice for Main Menu
        cin.clear();                                                    // Clear error state
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear input buffer

        // switch to handle main menu choices
        switch (mainMenuChoice)
        {
        case 1: singlePlayerGame (spaces, score, colourScheme, playersIcon); break;
        case 2: multiPlayerGame  (spaces, score, colourScheme, playersIcon); break;
        case 3: displayOptions   (colourScheme, playersIcon, score);         break;
        }
    } while (mainMenuChoice !=4);
}

void displayOptions (string *colourScheme, char *playersIcon, int *score)
{   
    // Displays the options menu and handles user choices
    // Parameters:
    // - colourScheme: Array for the color scheme settings
    // - playersIcon: Characters representing Player 1 and Player 2
    // - score: Array to track scores [wins, losses, ties]

    int optionChoice = 0;   // initialise optionChoice to 0

    do {    // do while loop to only exit this function when the user inputs an appropiate number
    cout << "\033[2J\033[1;1H";  // clears console 
    _setmode(_fileno(stdout), _O_U16TEXT);  // setmode to accept U16 characters
    wprintf(L"%s╔═══════════════════════════════════════════════════════╗\n",          colourScheme[0].c_str());
    wprintf(L"%s║%s  _______ _        _______           _______           %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s |__   __(_)      |__   __|         |__   __|          %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |   _  ___     | | __ _  ___     | | ___   ___   %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |  | |/ __|    | |/ _` |/ __|    | |/ _ \\ / _ \\  %s║\n",    colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |  | | (__     | | (_| | (__     | | (_) |  __/  %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    |_|  |_|\\___|    |_|\\__,_|\\___|    |_|\\___/ \\___|  %s║\n", colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║%s                1. Change border colour                %s║\n", colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s                2. Change text colour                  %s║\n", colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s                3. Change player icon colour           %s║\n", colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s                4. Change title colour                 %s║\n", colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s                5. Change player icon                  %s║\n", colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s                  6. Reset Score                       %s║\n", colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s   7. back                                             %s║\n", colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s╚═══════════════════════════════════════════════════════╝\n",          colourScheme[0].c_str());
    wprintf(L"\033[0m");                    // Reset the console color to default
    _setmode(_fileno(stdout), _O_TEXT);     // reset from U16 to default text
    
    cin >> optionChoice;                                            // user choice for option
    cin.clear();                                                    // clear error state
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // clear input buffer

    } while (8 < optionChoice || optionChoice < 0);

    optionChoice--; // decrement user input for the array

    // handle cases where user inputs 1 2 3 4 and call userChangeColourScheme function
    optionChoice != 6 && optionChoice != 5 && optionChoice != 4 ? userChangeColourScheme(colourScheme, optionChoice) : (void)0 ;

    // call the changePlayersIcon
    optionChoice == 4 ? changePlayersIcon(colourScheme, playersIcon) : (void)0;

    // reset score to 0
    if (optionChoice == 5) 
    {score[0] = score[1] = score[2] = 0;}
}

void userChangeColourScheme(string *colourScheme, int colourSchemeDigit)
{   
    // Allows the user to change the color scheme for the game
    // Parameters:
    // - colourScheme: Array for the color scheme settings
    // - colourSchemeDigit: Index of the color to change

    int optionChangeColourChoice = 0;       // initialize 
    
    
    do{ // do while loop to allow repetition and exit on user input
    cout << "\033[2J\033[1;1H";  // clears console 

    _setmode(_fileno(stdout), _O_U16TEXT);  // setmode to accept U16 characters
    wprintf(L"%s╔═══════════════════════════════════════════════════════╗\n",           colourScheme[0].c_str());
    wprintf(L"%s║%s  _______ _        _______           _______           %s║\n",       colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s |__   __(_)      |__   __|         |__   __|          %s║\n",       colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |   _  ___     | | __ _  ___     | | ___   ___   %s║\n",       colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |  | |/ __|    | |/ _` |/ __|    | |/ _ \\ / _ \\  %s║\n",     colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |  | | (__     | | (_| | (__     | | (_) |  __/  %s║\n",       colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    |_|  |_|\\___|    |_|\\__,_|\\___|    |_|\\___/ \\___|  %s║\n",  colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",           colourScheme[0].c_str());
    wprintf(L"%s║%s      1.Black  2.Red  3.Green  4.Yellow  5.Blue        %s║\n",       colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());    
    wprintf(L"%s║%s      6.Magenta  7.Cyan  8.White  9.Bright Red         %s║\n",       colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s      10.Bright Black (Gray)      11.Bright Green      %s║\n",       colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());    
    wprintf(L"%s║%s      12.Bright Yellow            13.Bright Blue       %s║\n",       colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s      14.Bright Cyan              15.Bright Magenta    %s║\n",       colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s      16.Bright White             17.Amber             %s║\n",       colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s                 18.Back to Main Menu                  %s║\n",       colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s╚═══════════════════════════════════════════════════════╝\n",           colourScheme[0].c_str());
    wprintf(L"\033[0m");                // reset colour scheme
    _setmode(_fileno(stdout), _O_TEXT); // reset from U16 to default

     // Ask user input to change colour and then clear error flags and input buffer
    cin >> optionChangeColourChoice;                                // ask user choice
    cin.clear();                                                    // Clear error state
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear Input Buffer

    } while (1 > optionChangeColourChoice || optionChangeColourChoice > 19);
    
    // switch to handle all colours
    switch (optionChangeColourChoice)
    {
        // case #: call colourScheme array and then set it to the appropiate colour and then break.
        // colourSchemeDigit corresponds to the user input in options after being decremented
        // The colour scheme is set up so that the digit corresponds to the right choice
        case 1:  colourScheme[colourSchemeDigit] = "\033[30m";             break;  // Black
        case 2:  colourScheme[colourSchemeDigit] = "\033[31m";             break; // Red
        case 3:  colourScheme[colourSchemeDigit] = "\033[32m";             break; // Green
        case 4:  colourScheme[colourSchemeDigit] = "\033[33m";             break; // Yellow
        case 5:  colourScheme[colourSchemeDigit] = "\033[34m";             break; // Blue
        case 6:  colourScheme[colourSchemeDigit] = "\033[35m";             break; // Magenta
        case 7:  colourScheme[colourSchemeDigit] = "\033[36m";             break; // Cyan
        case 8:  colourScheme[colourSchemeDigit] = "\033[37m";             break; // White
        case 9:  colourScheme[colourSchemeDigit] = "\033[91m";             break; // Bright Red 
        case 10: colourScheme[colourSchemeDigit] = "\033[90m";             break; // Bright Black (Gray)
        case 11: colourScheme[colourSchemeDigit] = "\033[92m";             break; // Bright Green
        case 12: colourScheme[colourSchemeDigit] = "\033[93m";             break; // Bright Yellow
        case 13: colourScheme[colourSchemeDigit] = "\033[94m";             break; // Bright Blue
        case 14: colourScheme[colourSchemeDigit] = "\033[96m";             break; // Bright Cyan
        case 15: colourScheme[colourSchemeDigit] = "\033[95m";             break; // Bright Magenta
        case 16: colourScheme[colourSchemeDigit] = "\033[97m";             break; // Bright White
        case 17: colourScheme[colourSchemeDigit] = "\033[38;2;255;191;0m"; break; // Bright White
        case 18: break;                                                           // user choice to go back
    }

}

void changePlayersIcon (string *colourScheme, char *playersIcon)
{
    // Allows the user to change the icons for Player 1 or Player 2/Bot
    // Parameters:
    // - colourScheme: Array for the color scheme settings
    // - playersIcon: Characters representing Player 1 and Player 2

    int iconChoice = 0;     // initialize
    do {                                    // loop until user exits
    cout << "\033[2J\033[1;1H";  // clears console 
    _setmode(_fileno(stdout), _O_U16TEXT);  // setmode to accept U16 characters
    wprintf(L"%s╔═══════════════════════════════════════════════════════╗\n",          colourScheme[0].c_str());
    wprintf(L"%s║%s  _______ _        _______           _______           %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s |__   __(_)      |__   __|         |__   __|          %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |   _  ___     | | __ _  ___     | | ___   ___   %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |  | |/ __|    | |/ _` |/ __|    | |/ _ \\ / _ \\  %s║\n",    colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |  | | (__     | | (_| | (__     | | (_) |  __/  %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    |_|  |_|\\___|    |_|\\__,_|\\___|    |_|\\___/ \\___|  %s║\n", colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║%s                   Change player icon                  %s║\n", colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║%s            1. Change single-player icon               %s║\n", colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s            2. Change AI / Player 2 icon               %s║\n", colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║%s   3. back                                             %s║\n", colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s╚═══════════════════════════════════════════════════════╝\n",          colourScheme[0].c_str());
    wprintf(L"\033[0m");                // reset colour scheme
    _setmode(_fileno(stdout), _O_TEXT); // reset to default text

    cin >> iconChoice;                                              // user input
    cin.clear();                                                    // clear error state
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // clear input buffer
    } while (iconChoice < 1 || iconChoice > 3);

    cout << "\033[2J\033[1;1H";  // clears console 
    _setmode(_fileno(stdout), _O_U16TEXT);  // setmode to accept U16 characters
    wprintf(L"%s╔═══════════════════════════════════════════════════════╗\n",          colourScheme[0].c_str());
    wprintf(L"%s║%s  _______ _        _______           _______           %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s |__   __(_)      |__   __|         |__   __|          %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |   _  ___     | | __ _  ___     | | ___   ___   %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |  | |/ __|    | |/ _` |/ __|    | |/ _ \\ / _ \\  %s║\n",    colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    | |  | | (__     | | (_| | (__     | | (_) |  __/  %s║\n",      colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║%s    |_|  |_|\\___|    |_|\\__,_|\\___|    |_|\\___/ \\___|  %s║\n", colourScheme[0].c_str(), colourScheme[3].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║%s                     Enter New Icon                    %s║\n", colourScheme[0].c_str(), colourScheme[1].c_str(), colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s║                                                       ║\n",          colourScheme[0].c_str());
    wprintf(L"%s╚═══════════════════════════════════════════════════════╝\n",          colourScheme[0].c_str());
    wprintf(L"\033[0m");                // reset colour scheme
    _setmode(_fileno(stdout), _O_TEXT); // reset to default text

    switch (iconChoice) 
    {   // switch to handle which icon to change or exit
        case 1: cin >> playersIcon[0]; break;
        case 2: cin >> playersIcon[1]; break;
        case 3: break;
    }
}

void singlePlayerGame (string *spaces, int *score, string *colourScheme, char *playersIcon)
{   
    // Handles all aspects of a single-player game of Tic-Tac-Toe
    // Updates scores after checking for wins, losses, or ties
    // Parameters:
    // - spaces: Current game board state (array of strings)
    // - score: Array to track scores [Player 1 wins, losses, ties]
    // - colourScheme: Colour settings for drawing the board
    // - playersIcon: Characters representing Player 1 and the bot

    int playerMove = 10;             // initialize
    bool gameOver = false;      // initialize
    bool isValidMove = false;   // initialize

    do {// loop until there's a win or tie

        isValidMove = false;      // reset to false from prev loop 

        // ----- PLAYER 1's Move ----- 
        while (!isValidMove)
        {
            drawBoard(spaces, colourScheme);
            cin >> playerMove;                                                   // playerMove input
            playerMove--;                                                        // decrement for arrays
            cin.clear();                                                         // clear error state
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');       // clear input buffer

            if (playerMove >= 0 && playerMove <= 8 && spaces[playerMove] == " ") // check valid move
            {isValidMove = true;}
        }

        spaces[playerMove] = playersIcon[0];    // initialize
        drawBoard(spaces, colourScheme);        // initialize

        // Check win con Player 1
        if (checkWin(spaces))
        {
            score[0]++;             // add a win to score
            drawBoard(spaces, colourScheme);    // update board
            cout << "\n\t\t\tYOU WIN!!!";
            std::this_thread::sleep_for(std::chrono::seconds(2)); // wait 2s
            break;  // exit here
        }
        // check draw con Player 1
        else if (checkTie(spaces) == true)
        {
            cout << "\n\t\t\tGameOver!!!";  
            std::this_thread::sleep_for(std::chrono::seconds(2)); // wait 2s
            score[2]++; // update score with tie
            break;      // exit here
        }
    
        // ----- Bot's Move -----
        botMove (spaces, playersIcon);

        // Check win con Bot
        if (checkWin(spaces))
        {
            score[1]++;             // add a lose to score
            drawBoard(spaces, colourScheme); // update board
            cout << "\n\t\t\tYOU LOSE!!!";
            std::this_thread::sleep_for(std::chrono::seconds(2)); // wait 2s
            break;  // exit singlePlayerGame here
        }

        // Check Tie con Bot
        else if (checkTie(spaces) == true)  
        {
            cout << "\n\t\t\tGameOver!!!";
            std::this_thread::sleep_for(std::chrono::seconds(2)); // wait 2s
            score[2]++; // update score
            break;  // exit singlePlayerGame here
        }
    }while (true);
    
}

bool checkTie (string *spaces)
{   
    // Checks if the game board state indicates a tie (no moves left)
    // Parameters:
    // - spaces: Current game board state (array of strings)
    // Returns:
    // - true if the game is a tie, false otherwise

    for (int i = 0; i < 9; i++) 
    {if (spaces[i] == " ") { return false;};}
    return true;
}

bool checkWin (string *spaces)
{
    // Checks if there is a win condition on the board
    // Parameters:
    // - spaces: Current game board state (array of strings)
    // Returns:
    // - true if there is a win, false otherwise

    // Define all win cons for any player
    int winCon[8][3] = 
    {   {0, 1, 2}, // Top row
        {3, 4, 5}, // Middle row
        {6, 7, 8}, // Bottom row
        {0, 3, 6}, // Left column
        {1, 4, 7}, // Middle column
        {2, 5, 8}, // Right column
        {0, 4, 8}, // Diagonal from top-left
        {2, 4, 6}  // Diagonal from top-right
    };

    for (int i = 0; i < 8 ; i++)
    {   // iterate over all win cons
        int a = winCon[i][0];
        int b = winCon[i][1];
        int c = winCon[i][2];
        
        if ( (spaces[a] != " ") && (spaces[a] == spaces[b]) && (spaces[b] == spaces[c]) )
        {return true;}
    }
    // return false if no win con found
return false;
}

void botMove (string *spaces, char *playersIcon)
{
    // Handles the bot's move in a single-player game
    // Parameters:
    // - spaces: Current game board state (array of strings)
    // - playersIcon: Characters representing Player 1 and the bot
    if (!checkTie(spaces)) 
    { 
        int botMove;    // initialize bot move
        do 
        {
            botMove = rand() % 9;       // rand num 0-8
            if (spaces[botMove] == " ") // check if the space is !taken
            {
                spaces[botMove] = playersIcon[1];
                break;   // place bot icon
            }
        } while (true);           // Repeat until a valid move is made
    }
}

void multiPlayerGame (string *spaces, int *score, string *colourScheme, char *playersIcon)
{   
    // Handles a two-player PvP game of Tic-Tac-Toe
    // Updates scores after checking for wins, losses, or ties
    // Parameters:
    // - spaces: Current game board state (array of strings)
    // - score: Array to track scores [Player 1 wins, Player 2 wins, ties]
    // - colourScheme: Colour settings for drawing the board
    // - playersIcon: Characters representing Player 1 and Player 2

    int playerMove;             // current player move
    bool isValidMove = false;   // Tracks if the current move is valid

    do
    {
        // ----- PLAYER 1's Move ----- 
        while (!isValidMove)
        {
            drawBoard(spaces, colourScheme); // Display the board
            cin >> playerMove;               // Input player's move
            playerMove--;                    // Adjust for 0-based indexing
            cin.clear();                     // Clear error state
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // clear input buffer

            // Validate the move (within bounds and on an empty space)
            if (playerMove >= 0 && playerMove <= 8 && spaces[playerMove] == " ")
            {isValidMove = true;}
        }

        // Apply Player 1's move
        isValidMove = false;
        spaces[playerMove] = playersIcon[0];    // Place Player 1's icon
        drawBoard(spaces, colourScheme);        // Redraw the board

        // Check win con Player 1
        if (checkWin(spaces))
        {
            score[0]++;     // Increment win score
            drawBoard(spaces, colourScheme);        // update board
            cout << "\n\t\t\tPlayer One Wins!!!";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            break;          // exit the function
        }

        // check tie con Player 1
        else if (checkTie(spaces) == true)
        {
            cout << "\n\t\t\tGameOver!!!";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            score[2]++;     // Increment Tie score
            break;          // exit the function
        }
    
        // ----- PLAYER 2's Move -----
        while (!isValidMove)
        {
            drawBoard(spaces, colourScheme);    // update board
            cin >> playerMove;
            playerMove--;                       // Adjust for 0-based indexing
            cin.clear();                        // clear error state
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // clear input buffer
            
            
            if (playerMove >= 0 && playerMove <= 8 && spaces[playerMove] == " ")
            {isValidMove = true;}
        }

        // Apply Player 2's move
        isValidMove = false;
        spaces[playerMove] = playersIcon[1];    // Place Player 2's icon
        drawBoard(spaces, colourScheme);        // update the board

        // Check win con Player 2
        if (checkWin(spaces))
        {
            score[1]++;     // update score with player 1 loss
            cout << "\n\t\t\tPlayer Two Wins!!!";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            break;
        }
        
        // Check win con Player 2
        else if (checkTie(spaces) == true)
        {
            score[2]++;     // update score with player 1 tie
            cout << "\n\t\t\tGameOver!!!";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            break;
        }
    
    }while (true); // Continue until the game ends with a break
    
}


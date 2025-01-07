
// thread and chrono are purely to make the code wait half a second.
#include <iostream>
#include <chrono>
#include <thread>

using std::cin;
using std::cout;
using std::string;

// Quiz game that presents a series of multiple-choice questions with 4 options.
// The result is displayed as the score out of 16 and the percentage. The spaces,
// \t, and \n are used for UI formatting. If you want to customize the UI, just 
// adjust the spacing or tabs following the \t tags used here.

int main()
{
    system("cls");

    // The questions and options are stored in arrays.
    // The questions are in the `questions` array, while the options are in a 
    // 2D `options` array. Each question corresponds to a set of four options,
    // and the answers are mapped in the `answerKey` array.

    string questions[] = {
        //        UI stars used for better alignment in the terminal.
        //  **************************************************************
        //  *******************************
        "\t    0000. What is the default value of a bool in C++ if set\n"
        "\t                           to global?",
        "\t    0001. Which header is needed for input/output \n"
        "\t                       operations in C++?",
        "\t    0010. What operator is used for accessing members of a \n"
        "\t                         class in C++?",
        "\t    0011. Which keyword is used to define a constant in C++?",
        "\t    0100. What is the size of int on a typical 32-bit \n"
        "\t                             system?",
        "\t    0101. What is the scope of a variable declared inside \n"
        "\t                          a function?",
        "\t    0110. What C++ keyword is used to create a dynamic array?",
        "\t    0111. What is the return type of the main() function \n"
        "\t                            in C++?",
        "\t    1000. What is the keyword used to declare a class in C++?",
        "\t    1001. Which operator is used to allocate memory for \n"
        "\t                       an object in C++?",
        "\t    1010. What number is this question in hexadecimal?",
        "\t    1011. Which C++ feature allows functions with the \n"
        "\t              same name but different parameters?",
        "\t    1100. What number is this question in decimal?",
        "\t    1101. What type of inheritance is used when a derived \n"
        "\t           class inherits from more than one base class?",
        "\t    1110. Which C++ standard is known for introducing \n"
        "\t                           lambdas?",
        "\t    1111. What is the purpose of the virtual keyword \n"
        "\t                           in C++?",
    };
    
    // Array storing the 4 options for each question.
    // 'prettier-ignore-start' prevents Prettier from formatting this block of code. 
    string options[][4] = {
        {"A. 0",                   "B. 1",                      "C. false",             "D. true"                },
        {"A. <stdio.h>",           "B. <iostream>",             "C. <fstream>",         "D. <cstdio>"            },
        {"A. ->",                  "B. .",                      "C. ::",                "D. []"                  },
        {"A. constant",            "B. immutable",              "C. final",             "D. const"               },
        {"A. 2 bytes",             "B. 4 bytes",                "C. 8 bytes",           "D. 16 bytes"            },
        {"A. global",              "B. local",                  "C. class",             "D. namespace"           },
        {"A. malloc",              "B. new",                    "C. calloc",            "D. dynamic"             },
        {"A. void",                "B. char",                   "C. int",               "D. float"               },
        {"A. struct",              "B. class",                  "C. object",            "D. interface"           },
        {"A. malloc()",            "B. new",                    "C. alloc",             "D. calloc()"            },
        {"A. 0x0A",                "B. 0x10",                   "C. 0x1A",              "D. 0x12"                },
        {"A. overloading",         "B. overriding",             "C. shadowing",         "D. hiding"              },
        {"A. 15",                  "B. 12",                     "C. 14",                "D. 16"                  },
        {"A. multiple",            "B. single",                 "C. hierarchical",      "D. multilevel"          },
        {"A. C++98",               "B. C++03",                  "C. C++11",             "D. C++14"               },
        {"A. to define constants", "B. to enable polymorphism", "C. to create a class", "D. to handle exceptions"}};
    // prettier-ignore-end 

    // Array storing the correct answers for each question.
    char answerKey[] = {'C', 'B', 'B', 'D',
                        'B', 'B', 'B', 'C',
                        'B', 'B', 'A', 'A',
                        'B', 'A', 'C', 'B'};

    // Calculate the number of questions (size of the array).
    int size = sizeof(questions) / sizeof(options[0][0]);

    // Game variables initialization.
    char guess;         // The user's guess for each question.
    int score = 0;      // Tracks the user's score.

    // Loop through the questions and display them, along with the options.
    for (int i = 0; i < size; i++)
    {
        cout << "\t **************************************************************\n";
        cout << questions[i] << '\n';
        cout << "\t **************************************************************\n";

        // Display the options for the current question.
        for (int j = 0; j < sizeof(options[i]) / sizeof(options[i][0]); j++)
        {
            cout << "\t\t\t\t" << options[i][j];
            cout << "\n";
        }

        cout << "\t\t\t\t";
        cin >> guess;       // Take the user's input.

        // Convert the guess to uppercase to handle case-insensitivity.
        guess = toupper(guess);

        // Check if the guess is correct and update the score accordingly.
        if (guess == answerKey[i])
        {
            system("cls");
            cout << "\n\n\n"
                 << "\t\t\t\tCORRECT!!\n\n\n\t\t\t\t";
            score++;

            // Wait 0.5 seconds (500,000,000 nanoseconds).
            std::this_thread::sleep_for(std::chrono::nanoseconds(500000000));
            system("cls");
        
        }
        else
        {
            system("cls");
            cout << "\n\n\n"
                 << "\t\t\t\tWRONG!!\n\n\n\t\t\t\t";

            // Wait 0.5 seconds (500,000,000 nanoseconds).
            std::this_thread::sleep_for(std::chrono::nanoseconds(500000000));
            system("cls");
        }
    }

    // Display the user's final score and percentage.
    cout << "\t **************************************************************\n";
    cout << "\t\t\t\tYou have scored " << score << "/" << size << "\n";
    cout << "\t\t\t\tScore: " << (double(score) / size) * 100 << "%\n";
    cout << "\t **************************************************************\n";

    // end of game.
    return 0;
}
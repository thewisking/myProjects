
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>

// using shortcuts - This form is better than using namespace std
using std::cin;
using std::cout;
using std::string;

// This code validates credit cards based on the Luhn Algorithm
// 1. Double every second digit from the right (Even) and split them
// 2. Add all digits from step 1
// 3. Add every odd digit from right to left (Odd)
// 4. Sum results from steps 2 & 3
// 5. If step 4 is divisible by 10, the credit card is valid
// E.G. 6011 0009 9013 9424
// 1. 2   9   1   9   0   0   1   6
//    4  1 8  2  1 8  0   0   2  1 2
// 2. 29
// 3. 4   4   3   0   9   0   1   0   -->   21
// 4. 50
// 5. 50 % 10 = 0  Your credit has been validated
int main ()
{
    system("cls");

    // Set up the required variables and give them an initial value if needed
    string creditCard;
    int sum = 0;
    int temp;
    int option;

    do
    {
        // display options and take in user input
        cout << "\t\t\t********************\n";
        cout << "\t\t\t Enter your choice:\n";
        cout << "\t\t\t********************\n";
        cout << "\t\t\t1. Validate Card\n";
        cout << "\t\t\t2. Exit\n\t\t\t\t";
        cin >> option;

        // clear error state and input buffer 
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    // setup a switch to handle the options
    switch(option)
    {

        // default case if user doesn't input 1 or 2
        default:
            system("cls");
            cout << "\t\t\t  Invalid Choice \n\n";
            break;

        // case where user wants to exit
        case 2:
        system("cls");
        cout << "\t\t\t Thanks for Visiting!!";
        return 0;

        // case where user wants to validate credit card
        case 1:
            system("cls");

            // User input (Can include spaces)
            cout << "\t\t\tEnter your credit card: \n\t\t\t";
            getline(cin , creditCard);

            // remove spaces from the input
            creditCard.erase(std::remove(creditCard.begin(), creditCard.end(), ' '), creditCard.end());

            // Restart the do while if user inputs anything other than digits and spaces
            if (!std::all_of(creditCard.begin(), creditCard.end(), ::isdigit)) 
                {
                system("cls");
                cout << "\t   Invalid input. Please enter only digits and spaces.\n\n";
                break;
                }

            // EVEN - Grab every second digit from right to left
            for (int i = creditCard.size() - 2; i >= 0  ; i-= 2)
                {
                // double each digit and convert it from char to int from ASCII table
                temp = 2 * (creditCard[i] - '0');

                // if the doubled # is two digits separate them. and add them to current sum
                sum += temp % 10 + (temp / 10 % 10);
                }

            // ODD
            for (int i = creditCard.size() -1; i >= 0; i -= 2)
                {
                // convert it from char to int from ASCII table and add them to current sum
                sum += (creditCard[i] - '0');
                }

            // clear terminal
            system("cls");

            // Display the result
            if (sum % 10 == 0)
                {
                cout << "\t\t    Your credit has been validated\n\n";
                }
            else
                {
                cout << "\t\t     Your credit card is invalid\n\n";
                }
            break;
            }
        
    } while (option != 2);
    

    return 0;
}

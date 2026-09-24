#include "solver.hpp"

#include <cstddef>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

using countdown::Value;

int main() {
    srand(static_cast<unsigned int>(std::time(nullptr)));

    const vector<Value> allNumbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 25, 50, 75, 100};
    // Set limits for the exercise
    const int howMany = 6;
    const Value lowLimit = 100, highLimit = 999;

    vector<Value> numbers;
    Value target = 0;

    int mode = 1;
    int quit;

    cout << "Choose a mode below for the game" << endl;
    cout << "1: Mode normal (" << howMany << " numbers and target are randomly picked)" << endl;
    cout << "2: Mode manual (pick " << howMany << " numbers and a target)" << endl;
    cin >> mode;

    bool manualMode = (mode == 2);

    if (manualMode) {
        for (int i = 1; i < howMany + 1; ++i) {
            Value number;
            cout << "Number " << i << "? ";
            cin >> number;
            numbers.push_back(number);
        }
        cout << endl;

        cout << "Target: ";
        cin >> target;
        cout << endl;
    }

    do {
        // Pick a target to obtain or approach the most
        if (!manualMode) {
            target = static_cast<Value>(rand() % static_cast<int>(highLimit - lowLimit)) + lowLimit;
            numbers.clear();
        }

        // Display chosen inputs
        cout << "-------Inputs-------" << endl;
        cout << "Numbers are: ";
        for (int i = 0; i < howMany; ++i) {
            if (!manualMode)
                numbers.push_back(allNumbers[static_cast<std::size_t>(rand()) % allNumbers.size()]);
            cout << numbers[i] << " ";
        }
        cout << endl;
        cout << "Target is: " << target << endl;
        cout << "--------------------" << endl;

        // Exercise
        // Note: initialize nearest with a number to make sure the nearest is reachable
        Value nearest = countdown::solve(numbers, target, numbers[0]);

        // Display results
        cout << "------Results------" << endl;
        if (nearest == target)
            cout << "Target reached!" << endl;
        else
            cout << "Nearest is: " << nearest << " (" << countdown::absoluteDifference(target, nearest)
                 << " from target)" << endl;
        cout << endl;

        cout << "Relaunch? (y:1, n:0)" << endl;
        cin >> quit;
    } while (quit != 0);

    return 0;
}

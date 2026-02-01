#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <time.h>
#include <vector>

using namespace std;

int solve(vector<int> &numbers, int target, int nearest);
int solve_impl(int number1, int number2, vector<int> numbers, int target, int nearest);

int main(int argc, char **argv) {
    srand(time(NULL));

    const vector<int> allNumbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 25, 50, 75, 100};
    // Set limits for the exercise
    const int howMany = 6, lowLimit = 100, highLimit = 999;

    vector<int> numbers;
    int target = 0;

    int manualMode = 0;
    int quit;

    cout << "Mode manuel (pick " << howMany << " numbers and a target)? (y:1, n:0)" << endl;
    cin >> manualMode;

    if (manualMode) {
        for (int i = 1; i < howMany + 1; ++i) {
            int number;
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
            target = rand() % (highLimit - lowLimit) + lowLimit;
            numbers.clear();
        }

        // Display chosen inputs
        cout << "-------Inputs-------" << endl;
        cout << "Numbers are: ";
        for (int i = 0; i < howMany; ++i) {
            if (!manualMode)
                numbers.push_back(allNumbers[rand() % allNumbers.size()]);
            cout << numbers[i] << " ";
        }
        cout << endl;
        cout << "Target is: " << target << endl;
        cout << "--------------------" << endl;

        // Exercise
        // Note: initialize nearest with a number to make sure the nearest is attaignable
        int nearest = solve(numbers, target, numbers[0]);

        // Display results
        cout << "------Results------" << endl;
        if (nearest == target)
            cout << "Target reached!" << endl;
        else
            cout << "Nearest is: " << nearest << " (" << target - nearest << " from target)" << endl;
        cout << endl;

        cout << "Relaunch? (y:1, n:0)" << endl;
        cin >> quit;
    } while (quit != 0);

    return 0;
}

int solve(vector<int> &numbers, int target, int nearest) {
    int result = (numbers.size() == 1) ? numbers[0] : 0;

    for (size_t i = 0; i < numbers.size() - 1; ++i) {
        for (size_t j = i + 1; j < numbers.size(); ++j) {
            vector<int> copyNumbers = numbers;
            copyNumbers.erase(copyNumbers.begin() + i);
            copyNumbers.erase(copyNumbers.begin() + j - 1);
            result = solve_impl(numbers[i], numbers[j], copyNumbers, target, nearest);
            if (abs(target - result) < abs(target - nearest)) {
                nearest = result;
                cout << "Numbers are: ";
                for (size_t i = 0; i < numbers.size(); ++i)
                    cout << numbers[i] << " ";
                cout << endl;
            }
        }
    }

    if (abs(target - result) < abs(target - nearest)) {
        nearest = result;
        cout << "New nearest: " << nearest << " Numbers are: ";
        for (size_t i = 0; i < numbers.size(); ++i)
            cout << numbers[i] << " ";
        cout << "(*)" << endl;
    }

    return nearest;
}

int solve_impl(int number1, int number2, vector<int> numbers, int target, int nearest) {
    int result = 0;

    // Intermediate result is already nearer
    if (abs(target - number1) < abs(target - nearest))
        nearest = number1;

    if (abs(target - number2) < abs(target - nearest))
        nearest = number2;

    // Addition
    if ((number1 > 0) && (number2 > 0)) {
        numbers.push_back(number1 + number2);
        result = solve(numbers, target, nearest);
        if (result != nearest) {
            cout << number1 << '+' << number2 << " = " << number1 + number2 << endl;
            nearest = result;
        }
        numbers.pop_back();
    }

    // Product
    if ((number1 > 1) && (number2 > 1)) {
        numbers.push_back(number1 * number2);
        result = solve(numbers, target, nearest);
        if (result != nearest) {
            cout << number1 << 'x' << number2 << " = " << number1 * number2 << endl;
            nearest = result;
        }
        numbers.pop_back();
    }

    // Substraction
    if ((number1 > number2) && (number2 != 0)) {
        numbers.push_back(number1 - number2);
        result = solve(numbers, target, nearest);
        if (result != nearest) {
            cout << number1 << '-' << number2 << " = " << number1 - number2 << endl;
            nearest = result;
        }
        numbers.pop_back();
    } else if ((number2 > number1) && (number1 != 0)) {
        numbers.push_back(number2 - number1);
        result = solve(numbers, target, nearest);
        if (result != nearest) {
            cout << number2 << '-' << number1 << " = " << number2 - number1 << endl;
            nearest = result;
        }
        numbers.pop_back();
    }

    // Division
    if ((number1 != 0) && (number2 > 1) && ((number1 % number2) == 0)) {
        numbers.push_back(number1 / number2);
        result = solve(numbers, target, nearest);
        if (result != nearest) {
            cout << number1 << '/' << number2 << " = " << number1 / number2 << endl;
            nearest = result;
        }
        numbers.pop_back();
    } else if ((number2 != 0) && (number1 > 1) && ((number2 % number1) == 0)) {
        numbers.push_back(number2 / number1);
        result = solve(numbers, target, nearest);
        if (result != nearest) {
            cout << number2 << '/' << number1 << " = " << number2 / number1 << endl;
            nearest = result;
        }
        numbers.pop_back();
    }

    return nearest;
}

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

using Value = std::int64_t;

std::uint64_t absoluteDifference(Value lhs, Value rhs) noexcept {
    if (lhs >= rhs)
        return static_cast<std::uint64_t>(lhs) - static_cast<std::uint64_t>(rhs);
    return static_cast<std::uint64_t>(rhs) - static_cast<std::uint64_t>(lhs);
}

bool checkedAddPositive(Value lhs, Value rhs, Value &result) noexcept {
    if (lhs > std::numeric_limits<Value>::max() - rhs)
        return false;

    result = lhs + rhs;
    return true;
}

bool checkedMultiplyPositive(Value lhs, Value rhs, Value &result) noexcept {
    if (lhs > std::numeric_limits<Value>::max() / rhs)
        return false;

    result = lhs * rhs;
    return true;
}

bool checkedSubtract(Value lhs, Value rhs, Value &result) noexcept {
    const Value minimum = std::numeric_limits<Value>::min();
    const Value maximum = std::numeric_limits<Value>::max();

    if ((rhs > 0 && lhs < minimum + rhs) || (rhs < 0 && lhs > maximum + rhs))
        return false;

    result = lhs - rhs;
    return true;
}

Value solve(vector<Value> &numbers, Value target, Value nearest);
Value solve_impl(Value number1, Value number2, vector<Value> numbers, Value target, Value nearest);

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
        Value nearest = solve(numbers, target, numbers[0]);

        // Display results
        cout << "------Results------" << endl;
        if (nearest == target)
            cout << "Target reached!" << endl;
        else
            cout << "Nearest is: " << nearest << " (" << absoluteDifference(target, nearest) << " from target)" << endl;
        cout << endl;

        cout << "Relaunch? (y:1, n:0)" << endl;
        cin >> quit;
    } while (quit != 0);

    return 0;
}

Value solve(vector<Value> &numbers, Value target, Value nearest) {
    Value result = (numbers.size() == 1) ? numbers[0] : 0;

    for (size_t i = 0; i < numbers.size() - 1; ++i) {
        for (size_t j = i + 1; j < numbers.size(); ++j) {
            vector<Value> copyNumbers = numbers;
            copyNumbers.erase(copyNumbers.begin() + i);
            copyNumbers.erase(copyNumbers.begin() + j - 1);
            result = solve_impl(numbers[i], numbers[j], copyNumbers, target, nearest);
            if (absoluteDifference(target, result) < absoluteDifference(target, nearest)) {
                nearest = result;
                cout << "Numbers are: ";
                for (size_t index = 0; index < numbers.size(); ++index)
                    cout << numbers[index] << " ";
                cout << endl;
            }
        }
    }

    if (absoluteDifference(target, result) < absoluteDifference(target, nearest)) {
        nearest = result;
        cout << "New nearest: " << nearest << " Numbers are: ";
        for (size_t index = 0; index < numbers.size(); ++index)
            cout << numbers[index] << " ";
        cout << "(*)" << endl;
    }

    return nearest;
}

Value solve_impl(Value number1, Value number2, vector<Value> numbers, Value target, Value nearest) {
    Value result = 0;

    auto tryOperation = [&](Value left, Value right, char operation, Value operationResult) {
        numbers.push_back(operationResult);
        result = solve(numbers, target, nearest);
        if (result != nearest) {
            cout << left << operation << right << " = " << operationResult << endl;
            nearest = result;
        }
        numbers.pop_back();
    };

    // Intermediate result is already nearer
    if (absoluteDifference(target, number1) < absoluteDifference(target, nearest))
        nearest = number1;

    if (absoluteDifference(target, number2) < absoluteDifference(target, nearest))
        nearest = number2;

    // Addition
    if ((number1 > 0) && (number2 > 0)) {
        Value operationResult;
        if (checkedAddPositive(number1, number2, operationResult))
            tryOperation(number1, number2, '+', operationResult);
    }

    // Product
    if ((number1 > 1) && (number2 > 1)) {
        Value operationResult;
        if (checkedMultiplyPositive(number1, number2, operationResult))
            tryOperation(number1, number2, 'x', operationResult);
    }

    // Subtraction
    if ((number1 > number2) && (number2 != 0)) {
        Value operationResult;
        if (checkedSubtract(number1, number2, operationResult))
            tryOperation(number1, number2, '-', operationResult);
    } else if ((number2 > number1) && (number1 != 0)) {
        Value operationResult;
        if (checkedSubtract(number2, number1, operationResult))
            tryOperation(number2, number1, '-', operationResult);
    }

    // Division
    if ((number1 != 0) && (number2 > 1) && ((number1 % number2) == 0)) {
        const Value operationResult = number1 / number2;
        tryOperation(number1, number2, '/', operationResult);
    } else if ((number2 != 0) && (number1 > 1) && ((number2 % number1) == 0)) {
        const Value operationResult = number2 / number1;
        tryOperation(number2, number1, '/', operationResult);
    }

    // Modulo
    if ((number2 != 0) && (number2 != -1) && (number1 >= number2)) {
        const Value operationResult = number1 % number2;
        tryOperation(number1, number2, '%', operationResult);
    } else if ((number1 != 0) && (number1 != -1) && (number2 >= number1)) {
        const Value operationResult = number2 % number1;
        tryOperation(number2, number1, '%', operationResult);
    }

    return nearest;
}

#include "solver.hpp"

#include <cstddef>
#include <iostream>
#include <limits>
#include <vector>

namespace countdown {
namespace {

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

Value solveImpl(Value number1, Value number2, std::vector<Value> numbers, Value target, Value nearest) {
    Value result = 0;

    auto tryOperation = [&](Value left, Value right, char operation, Value operationResult) {
        numbers.push_back(operationResult);
        result = solve(numbers, target, nearest);
        if (result != nearest) {
            std::cout << left << operation << right << " = " << operationResult << std::endl;
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

} // namespace

std::uint64_t absoluteDifference(Value lhs, Value rhs) noexcept {
    if (lhs >= rhs)
        return static_cast<std::uint64_t>(lhs) - static_cast<std::uint64_t>(rhs);
    return static_cast<std::uint64_t>(rhs) - static_cast<std::uint64_t>(lhs);
}

Value solve(std::vector<Value> &numbers, Value target, Value nearest) {
    Value result = (numbers.size() == 1) ? numbers[0] : 0;

    for (std::size_t i = 0; i < numbers.size() - 1; ++i) {
        for (std::size_t j = i + 1; j < numbers.size(); ++j) {
            std::vector<Value> copyNumbers = numbers;
            copyNumbers.erase(copyNumbers.begin() + i);
            copyNumbers.erase(copyNumbers.begin() + j - 1);
            result = solveImpl(numbers[i], numbers[j], copyNumbers, target, nearest);
            if (absoluteDifference(target, result) < absoluteDifference(target, nearest)) {
                nearest = result;
                std::cout << "Numbers are: ";
                for (std::size_t index = 0; index < numbers.size(); ++index)
                    std::cout << numbers[index] << " ";
                std::cout << std::endl;
            }
        }
    }

    if (absoluteDifference(target, result) < absoluteDifference(target, nearest)) {
        nearest = result;
        std::cout << "New nearest: " << nearest << " Numbers are: ";
        for (std::size_t index = 0; index < numbers.size(); ++index)
            std::cout << numbers[index] << " ";
        std::cout << "(*)" << std::endl;
    }

    return nearest;
}

} // namespace countdown

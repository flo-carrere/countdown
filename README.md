# Countdown

A small command-line countdown solver. The current implementation is written in C++.

## Game rules

Each round supplies six numbers and a target.

- A solution may use any subset of the supplied numbers.
- A solution must use at least two supplied numbers; using a single number by itself is not a solution.
- You do not have to use all six numbers.
- Each supplied number can be used at most once.
- Combine two currently available values at a time using:
  - addition (`+`)
  - multiplication (`*`)
  - subtraction, using the larger value minus the smaller one
  - exact integer division (`/`), when the division has no remainder
  - modulo (`%`), with a non-zero divisor
- The result of an operation can be used in another operation.
- The goal is to reach the target exactly. If that is not possible, the program reports the closest result, measured by absolute distance from the target.

For example, with the numbers `2`, `3`, and `8`, a target of `19` can be reached with `2 * 8 + 3`. With six numbers, any combination of two through six supplied numbers may be used; the unused numbers do not invalidate the solution.

## Modes

1. **Normal mode** randomly selects six numbers and a target.
2. **Manual mode** lets you enter six numbers and a target.

## Build and run

```sh
cmake -S . -B build
cmake --build build
./build/app
```

On Windows, the executable is typically `build\\app.exe`.

> **Implementation note:** the current recursive solver still considers an individual input as a candidate result. The minimum-two rule above is the intended game rule and should be enforced in the solver before the project is considered complete.

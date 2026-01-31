#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <random>
#include <time.h>

using namespace std;

int solve( vector<int>& numbers, int target, int nearest );
int solve_impl( int number1, int number2, const vector<int> numbers, int target, int nearest);

int main( int argc, char** argv )
{
    srand(time(NULL));

    const vector<int> allNumbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 25, 50, 75, 100};
    // Set limits for the exercise
    const int howMany = 6, lowLimit = 100, highLimit = 999;

    vector<int> numbers;
    int target = 0;

    int manualMode = 0;

    cout << "Mode manuel? (y:1, n:0)" << endl;
    cin >> manualMode;

    if( manualMode )
    {
        for( int i = 1 ; i < howMany+1 ; ++i )
        {
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
        // Choose a target to obtain or approach the most
        if( !manualMode )
        {
            target = rand() % (highLimit - lowLimit) + lowLimit;
            numbers.clear();
        }

        // Choose numbers
        cout << "Numbers are: ";
        for( int i = 0 ; i < howMany ; ++i )
        {
            if( !manualMode )
                numbers.push_back(allNumbers[rand() % allNumbers.size()]);
            cout << numbers[i] << " ";
        }
        cout << endl;

        cout << "Target is: " << target << endl;

        // Exercise
        int nearesult = solve( numbers, target, numbers[0] );

        if( nearesult == target )
            cout << "Le compte est bon!" << endl << endl;
        else
            cout << nearesult << endl << endl;

    } while( getchar() != ' ' );

    return 0;
}

int solve( vector<int>& numbers, int target, int nearest )
{
    int result = numbers[0];

    for( size_t i = 0 ; i < numbers.size() ; ++i )
    {
        for( size_t j = i + 1 ; j < numbers.size() ; ++j )
        {
            vector<int> copyNumbers = numbers;
            copyNumbers.erase(copyNumbers.begin()+i);
            copyNumbers.erase(copyNumbers.begin()+j-1);
            result = solve_impl( numbers[i], numbers[j], copyNumbers, target, nearest);
            if( abs(target-result) < abs(target-nearest) )
            {
                nearest = result;

                cout << "New nearest: " << nearest << " Numbers are: " ;
                for( size_t i = 0 ; i < numbers.size() ; ++i )
                    cout << numbers[i] << " ";
                cout << endl;
            }
        }
    }

    if( abs(target-result) < abs(target-nearest) )
        nearest = result;

    return nearest;
}

int solve_impl( int number1, int number2, vector<int> numbers, int target, int nearest )
{
    int result = 0;

    // Intermediate result is already nearer
    if( abs(target-number1) < abs(target-nearest) )
        nearest = number1;

    if( abs(target-number2) < abs(target-nearest) )
        nearest = number2;

    // Addition
    numbers.push_back(number1+number2);
    result = solve(numbers, target, nearest);
    if( result != nearest )
    {
        cout << number1 << '+' << number2 << " = " << number1 + number2 << endl;
        nearest = result;
    }
    numbers.pop_back();

    // Product
    if( (number1 != 1) && (number2 != 1) )
    {
        numbers.push_back(number1*number2);
        result = solve(numbers, target, nearest);
        if( result != nearest )
        {
            cout << number1 << 'x' << number2 << " = " << number1 * number2 << endl;
            nearest = result;
        }
        numbers.pop_back();
    }

    // Substraction
    if( number1 > number2 )
    {
        numbers.push_back(number1-number2);
        result = solve(numbers, target, nearest);
        if( result != nearest )
        {
            cout << number1 << '-' << number2 << " = " << number1 - number2 << endl;
            nearest = result;
        }
        numbers.pop_back();
    }
    else if( number2 > number1 )
    {
        numbers.push_back(number2-number1);
        result = solve(numbers, target, nearest);
        if( result != nearest )
        {
            cout << number2 << '-' << number1 << " = " << number2 - number1 << endl;
            nearest = result;
        }
        numbers.pop_back();
    }

    // Division
    if( (number2 != 1) && ((number1 % number2) == 0) )
    {
        numbers.push_back(number1/number2);
        result = solve(numbers, target, nearest);
        if( result != nearest )
        {
            cout << number1 << '/' << number2 << " = " << number1 / number2 << endl;
            nearest = result;
        }
        numbers.pop_back();
    }

    if( (number1 != 1) && ((number2 % number1) == 0) )
    {
        numbers.push_back(number2/number1);
        result = solve(numbers, target, nearest);
        if( result != nearest )
        {
            cout << number2 << '/' << number1 << " = " << number2 / number1 << endl;
            nearest = result;
        }
        numbers.pop_back();
    }

    return nearest;
}

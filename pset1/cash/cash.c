#include <stdio.h>
#include <cs50.h>
#include <math.h>

void calc();

// Variable declaration and definition
const int COIN_LIST[4] = {1, 5, 10, 25};
int coin_amount = 0;
int change;

int main()
{
    // Input validation
    do
    {
        double raw = get_double("Change: ");
        change = floor(raw * 100);
        printf("%lf\n", raw);
    }
    while (change <= 0);
    printf("%i\n", change);

    // Calculate and print calculated amount of coins
    calc(3);
    printf("Coins: %i\n", coin_amount);

}

// Recursive calculator
void calc(int i)
{
    coin_amount += floor(change / COIN_LIST[i]);
    change = change % COIN_LIST[i];



    if (i > 0)
    {
        calc(i - 1);
    }
}
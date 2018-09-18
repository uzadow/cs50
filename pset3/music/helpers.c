// Helper functions for music
#include <stdio.h>

#include <cs50.h>
#include <math.h>
#include <string.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    string a = &fraction[0];
    string b = &fraction[2];
    double c = atoi(a);
    double d = atoi(b);
    return c / d * 8;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    // Index of octave indicator in "note"
    int oi = strlen(note) - 1;

    // Number of semitones away from A4
    int n = 0;

    // Number of octave
    string o = &note[oi];

    // Add 12 semitones per octave
    n += (12 * (atoi(o) - 4));

    // Add/subtract one semitone if #/b avaible
    if (strlen(note) == 3)
    {
        if (note[1] == '#') n++;
        else if (note[1] == 'b') n--;
    }

    // Add semitones according to note
    n += ((note[0] - 'A') * 2);
    switch (note[0])
    {
        case 'C' :
        case 'D' :
        case 'E' :
            n -= 13;
            break;
        case 'F' :
        case 'G' :
            n -= 14;
            break;
    }

    // returns frequency of semitones away from A4
    return round(pow(2, (n / 12.0)) * 440);
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    return !(strcmp(s, ""));
}
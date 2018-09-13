# Questions

## What's `stdint.h`?

a header for integer types

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

to have a fixed number of bits

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

one, four, four and two; 1, 4, 4, 2

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

BM

## What's the difference between `bfSize` and `biSize`?

bfSize is the bitmap file size; thebiSize is the data structure size

## What does it mean if `biHeight` is negative?

bitmap is a top-down DIB and its origin is the upper-left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

if the file to open doesn't exist

## Why is the third argument to `fread` always `1` in our code?

because only one element is used

## What value does line 65 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

a value of 3

## What does `fseek` do?

it sets the file position of the stream to the given offset

## What is `SEEK_CUR`?

a constant

## Whodunit?

It was Professor Plum with the candlestick in the library.

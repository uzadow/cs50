# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

a synonym for the disease silicosis

## According to its man page, what does `getrusage` do?

get resource usage

## Per that same man page, how many members are in a variable of type `struct rusage`?

15

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

because RUSAGE is automatically updating it's contents, thus making it unneccessary to change it's content. So the pure reference is enough

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

it first initializes with the char it gets from fgetc, thus moving the pointer within the file to the next char. As long as this char is not the end of the file, it shall rerun fgetc to update c with the next characters and move the "cursor" in the file further. This continues until EOF is finally reached.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

fgetc can check each character independently

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

To prevent them from accidentally getting overwritten

from nltk.tokenize import sent_tokenize

def lines(a, b):
    """Return lines in both a and b"""

    A = {"test"}
    B = {"test"}
    A.remove("test")
    B.remove("test")
    line = ""

    for letter in a:
        if letter == "\n":
            A.add(line)
            line = ""
        else:
            line += letter
    if line != "":
        A.add(line)

    for letter in b:
        if letter == "\n":
            B.add(line)
            line = ""
        else:
            line += letter
    if line != "":
        B.add(line)

    return A.intersection(A,B)

def sentences(a, b):
    """Return sentences in both a and b"""

    A = {"test"}
    B = {"test"}
    A.remove("test")
    B.remove("test")

    la = sent_tokenize(a)
    lb = sent_tokenize(b)

    for sentence in la:
        A.add(sentence)

    for sentence in lb:
        B.add(sentence)

    return A.intersection(B)


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    A = {"test"}
    B = {"test"}
    A.remove("test")
    B.remove("test")

    for i in range(len(a) - n + 1):
        substr = ""
        for j in range(n):
            substr += a[i + j]
        A.add(substr)

    for i in range(len(b) - n + 1):
        substr = ""
        for j in range(n):
            substr += b[i + j]
        B.add(substr)

    return A.intersection(B)

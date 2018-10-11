from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def distances(a, b):
    """Calculate edit distance from a to b"""

    # Variable initialisation
    la = len(a)
    lb = len(b)
    matrix = [0] * (la + 1)

    # Filling of the 0 Row/Column
    for i in range(la + 1):
        matrix[i] = [0] * (lb + 1)
        matrix[0][i] = (i, Operation.INSERTED)

    for j in range(lb):
        matrix[j + 1][0] = (j + 1, Operation.DELETED)

    matrix[0][0] = (0, None)

    # Filling of the rest of the Matrix
    for i in range(la):
        for j in range(lb):
            matrix[i + 1][j + 1] = cost(matrix, a, b, i + 1, j + 1)

    return matrix

def cost(matrix, a, b, i, j):
    """Actual calculator (giving out a tuple (cost, operation)"""
    values = [[matrix[i - 1][j][0] + 1, Operation.DELETED], [matrix[i][j - 1][0] + 1, Operation.INSERTED], [matrix[i - 1][j - 1][0], Operation.SUBSTITUTED]]
    if (a[i - 1] != b[j - 1]):
        values[2][0] += 1

    #sort values
    values.sort(key = minCost)

    #return values.sort(key = minCost)[0]
    return values[0]

def minCost(e):
    """Returns the cost of a tuple"""
    return e[0]
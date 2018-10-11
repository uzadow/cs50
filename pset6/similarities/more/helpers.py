from enum import Enum
from numpy import *


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.enum.lower())


def distances(a, b):
    """Calculate edit distance from a to b"""

    # Variable initialisation
    la = len(a) + 1
    lb = len(b) + 1
    matrix = [[0 for x in range(lb)] for y in range(la)]
    matrix[0][0] = (0, None)

    # Filling out Matrix;
    for ia in range(1, la):
        for ib in range(1, lb):
            matrix[0][ib] = (ib, Operation.INSERTED)
            matrix[ia][0] = (ia, Operation.DELETED)
            matrix[ia][ib] = cost(matrix, a, b, ia, ib)

    return matrix

def cost(matrix, a, b, i, j):
    """Actual calculator (giving out a tuple (cost, operation)"""
    values = [[matrix[i - 1][j][0] + 1, Operation.DELETED], [matrix[i][j - 1][0] + 1, Operation.INSERTED], [matrix[i - 1][j - 1][0], Operation.SUBSTITUTED]]
    if (a[i - 1] != b[j - 1]):
        values[2][0] += 1

    #sort values
    values.sort(key = minCost)

    #return values.sort(key = minCost)[0] aka lowest cost
    return values[0]

def minCost(e):
    """Returns the cost of a tuple"""
    return e[0]
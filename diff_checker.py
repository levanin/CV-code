from collections import deque as queue
import copy

"""Various functions used in a diff checker program. The line_edits function will return whether a line is deleted, added, or changed.
Author: Shai Levin"""

def cost_table(s1, s2):
    """Builds a 2d array where the values are the costs of the diff
    between substrings of the strings of s1 and s2"""
    n = len(s1)
    m = len(s2)
    cost = [[0 for _ in range(m + 1)] for _ in range(n + 1)]
    for i in range(n + 1):
        for j in range(m + 1):
            if i == 0:
                cost[i][j] = j
            elif j == 0:
                cost[i][j] = i
            elif s1[i - 1] == s2[j - 1]:
                cost[i][j] = cost[i - 1][j - 1]
            else:
                cost[i][j] = 1 + min(cost[i - 1][j - 1], cost[i - 1][j], cost[i][j - 1])
    return cost


def line_edits(c1, c2):
    """Determines the optimal difference between the strings c1 and c2 using the edit-distance algorithm"""
    s1 = c1.splitlines()
    s2 = c2.splitlines()
    n = len(s1)
    m = len(s2)
    ctable = cost_table(s1, s2)
    output = []
    while m > 0 or n > 0:
        if m > 0 and n > 0 and s1[n - 1] == s2[m - 1]:  # if the lines are equal, transfer
            output.append(('T', s1[n - 1], s2[m - 1]))
            m -= 1
            n -= 1
        else:  # else check whether it is a Del, Sub, In in that priority
            check = (ctable[n - 1][m], ctable[n][m - 1], ctable[n - 1][m - 1])
            key = check.index(min(check))
            if key == 2:
                a, b = bracketing_helper(s1[n - 1], s2[m - 1])
                output.append(('S', a, b))
                m -= 1
                n -= 1
            elif key == 0:
                output.append(('D', s1[n - 1], ''))
                n -= 1
            else:
                output.append(('I', '', s2[m - 1]))
                m -= 1
    return output[::-1]


def lcs(s1, s2):
    """Builds a 2d array where the values are the lengths of the longest common
    substrings of the substrings of s1 and s2"""
    n = len(s1)
    m = len(s2)
    # initialise 2d array with size n+1 x m+1
    lcs_array = [[None for _ in range(m + 1)] for _ in range(n + 1)]
    # build array of LCS counters
    for i in range(n + 1):
        for j in range(m + 1):
            if i == 0 or j == 0:
                lcs_array[i][j] = 0
            elif s1[i - 1] == s2[j - 1]:
                lcs_array[i][j] = lcs_array[i - 1][j - 1] + 1
            else:
                lcs_array[i][j] = max(lcs_array[i - 1][j], lcs_array[i][j - 1])
    return lcs_array


def bracketing_helper(s1, s2):
    """Adds double square brackets to s1 and s2 for any characters not
     in the largest common sub-sequence of the two strings"""
    lcs = queue(longest_common_substring(s1, s2))
    lcs2 = copy.deepcopy(lcs)
    s1_diff = ""
    s2_diff = ""
    for c in s1:
        if len(lcs) > 0 and lcs[0] == c:
            lcs.popleft()
            s1_diff += c
        else:
            s1_diff += "[[" + c + "]]"
    for c in s2:
        if len(lcs2) > 0 and lcs2[0] == c:
            lcs2.popleft()
            s2_diff += c
        else:
            s2_diff += "[[" + c + "]]"

    return s1_diff, s2_diff


def longest_common_substring(s1, s2):
    """Finds the longest common substring of 2 strings s1 and s2 using a bottom up DP approach"""
    lcs_array = lcs(s1, s2)
    n = len(s1)
    m = len(s2)
    l = lcs_array[n][m]
    result = ""
    while l != 0:
        if s1[n - 1] == s2[m - 1]:
            result = s1[n - 1] + result
            l -= 1
            n -= 1
            m -= 1
        elif lcs_array[n][m - 1] >= lcs_array[n - 1][m]:
            m -= 1
        else:
            n -= 1
    return result
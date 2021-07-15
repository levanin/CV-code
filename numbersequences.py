import operator
import random

#predicting number sequences using a naive genetic model
#Auth: Shai Levin
#Date: Sept 2020

def is_valid_expression(object, function_symbols, leaf_symbols):
    if type(object) == int:
        return True
    elif type(object) == str and object in leaf_symbols:
        return True
    elif type(object) == list and len(object) == 3 and type(object[0]) == str and object[0] in function_symbols:
        return is_valid_expression(object[1], function_symbols, leaf_symbols) and is_valid_expression(object[2], function_symbols, leaf_symbols)
    else:
        return False


def depth(expression, d=None):
    if d == None:
        d = 0
    if type(expression) != list:
        return d
    depthleft = depth(expression[1], d+1)
    depthright = depth(expression[2], d+1)

    return max(depthleft, depthright)


def evaluate(expression, bindings):
    if type(expression) == int:
        return expression
    elif type(expression) == str:
        return bindings[expression]
    else:
        return bindings[expression[0]](evaluate(expression[1], bindings), evaluate(expression[2], bindings))


def random_expression(function_symbols, leaves, max_depth, depth=None):
    if depth == None:
        depth = 0
    if random.choice([True, False]) or depth == max_depth:
        return random.choice(leaves)
    else:
        return list([random.choice(function_symbols), random_expression(function_symbols, leaves, max_depth, depth=depth+1), random_expression(function_symbols, leaves, max_depth, depth=depth+1)])


def generate_rest(initial_sequence, expression, length):
    i = len(initial_sequence)
    if length == 0:
        return []
    bindings = {'+': lambda x, y: x+y,
                '-': lambda x, y: x-y, '*': lambda x, y: x*y}
    for i in range(i, i+length):
        bindings['x'] = initial_sequence[i-2]
        bindings['y'] = initial_sequence[i-1]
        bindings['i'] = i
        initial_sequence.append(evaluate(expression, bindings))
    return initial_sequence[-length:]


def predict_rest(sequence):
    leaves = ['x', 'y', -2, -1, 0, 1, 2]
    function_symbols = ['+', '-', '*']
    l = len(sequence)
# start with the first 3 values, and see if they generate the other 2 with a random expression
    while True:
        exp = random_expression(function_symbols, leaves, 3)
        if generate_rest(sequence[:3], exp, l-3) == sequence[3:l]:
            break
    return generate_rest(sequence, exp, 5)


print(predict_rest([5,8,17,24,37,48]))

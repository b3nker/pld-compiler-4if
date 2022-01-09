#!/usr/bin/env python3

import string
import random

largeur = 50  # taille des entiers utilisés
profondeur = 14  # attention aux 'maximum recursion depth exceeded'
binop_length = 5  # nombre d'opérandes pour les opérations binaires
operators = '+-*'

symbols = []


def getsymbols():
    return symbols


def clearsymbols():
    global symbols
    symbols = []


def program(indent=0):
    out = []
    out.append('int main() {')

    n_decl = 4  # random.randrange(1, 25)
    n_stmt = 10  # random.randrange(1, 1215)
    out.append('\n'.join([
        declaration(indent+1)
        for _ in range(n_decl)
    ]))
    out.append('\n'.join([
        statement(indent+1)
        for _ in range(n_stmt)
    ]))
    out.append('  return ' + '+'.join(symbols) + ';')
    out.append('}')
    return '\n'.join(out)


def function(indent=0, name=None):
    out = []
    if name is None:
        name = ''.join(random.choices(string.ascii_letters, k=9))
    out.append('int ' + name + '() {')

    n_decl = 4  # random.randrange(1, 25)
    n_stmt = 10  # random.randrange(1, 1215)
    out.append('\n'.join([
        declaration(indent+1)
        for _ in range(n_decl)
    ]))
    out.append('\n'.join([
        statement(indent+1)
        for _ in range(n_stmt)
    ]))
    out.append('  return ' + '+'.join(symbols) + ';')
    out.append('}')
    return '\n'.join(out)


def divmod_excel(n):
    a, b = divmod(n, 26)
    if b == 0:
        return a - 1, b + 26
    return a, b


def to_excel(num):
    chars = []
    while num > 0:
        num, d = divmod_excel(num)
        chars.append(string.ascii_lowercase[d - 1])
    return ''.join(reversed(chars))


def declaration(indent=0):
    types = ['int']

    decls = []
    for _ in range(random.randrange(1, 2)):
        # ident = random.choice(string.ascii_letters) + ''.join(random.choices(
        #     string.ascii_letters + string.digits, k=3))
        ident = to_excel(len(symbols) + 1)
        if True:  # random.random() > 0.5:
            decls.append(f"{ident} = {expression()}")
        else:
            decls.append(f"{ident}")
        symbols.append(ident)

    return ('  ' * indent) + random.choice(types) + ' ' + (', \n    ' + ('  ' * indent)).join(decls) + ";"


def expression():
    return expr(profondeur)


def num(depth):
    return str(random.randrange(0, largeur))


def expr(depth):
    if depth <= 1:
        return num(depth - 1)
    paren = random.random() > 0.5
    if paren:
        return '(' + num_or_expr(depth - 1) + ')'
    else:
        return num_or_expr(depth - 1)


def num_or_expr(depth):
    x = random.randrange(depth)
    if x == 1 or (x == 2 and len(symbols) == 0):
        return num(depth - 1)
    elif x == 2:
        return random.choice(symbols)
    elif x == 3:
        return binop(depth - 1)
    else:
        return expr(depth - 1)


def binop(depth):
    if depth <= 1:
        return num(depth - 1)
    n = random.randrange(2, binop_length)
    op = random.choice(operators)
    return f' {op} '.join([num_or_expr(depth - 1) for _ in range(n)])


def statement(indent=0):
    ident = random.choice(symbols)
    return ('  ' * indent) + f"{ident} = {expression()}" + ';'


if __name__ == "__main__":
    print(program())

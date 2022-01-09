from fuzz import declaration, statement, getsymbols, clearsymbols
import random


names = list(map(lambda i: 'f' + str(i), range(30)))
funcs = map(lambda i: function(i), range(30))


def function(index, indent=0):
    name = names[index]

    if len(names) == index + 1:
        return 'int ' + name + '(int end) { return end; }'

    out = []
    out.append('int ' + name + '(int param1) {')
    clearsymbols()

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
    out.append('  return ' + '+'.join(getsymbols()))
    out.append('+ ' + names[index + 1] + '(param1 + 5);')
    out.append('}')
    return '\n'.join(out)


print('\n\n'.join(reversed(list(funcs))))

print('\nint main() { return f0(42); }')

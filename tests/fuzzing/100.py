import random
N = 100
print("int func(" + ", ".join(["int x" + str(i) for i in range(N)]) + ") {")
print("  int a = 128;")
for i in range(N):
    op = ("+" if i % 2 == 0 else "-")
    print("  a = a " + op + " x" + str(i) + ";")
print("  return a;")
print("}")
print("int main() { return func(" +
      ", ".join([str(i * i % 100 + 5) for i in range(N)]) + ") % 255; }")

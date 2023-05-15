# Cinder

Cinder is a programming language that I made for my high school senior project.

It is a dynamically typed, bytecode interpreted language written in C99 based on [clox by Robert Nystrom](https://github.com/munificent/craftinginterpreters)

Treat this language as a toy in its current state, as it is unfinished and untested.

## Example

[tests/native_time.ci](./tests/native_time.ci)

---

```Rust
def fib(n) {
    if (n < 2) return n;
    return fib(n - 2) + fib(n - 1);
}

let start = clock();
print fib(35);
print clock() - start;
```

```bash
$ make
$ ./cinder native_time.ci
9.22746e+06
2.625
```

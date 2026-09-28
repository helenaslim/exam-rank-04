# Exam Rank 04

Solutions and study notes for the **42 Exam Rank 04**. The exercises focus on
Unix processes, pipes, file descriptors, and recursive-descent parsing.

## Exercises

| Exercise | Main topic | Location |
| --- | --- | --- |
| `ft_popen` | Execute a program through a pipe | [`ft_popen/`](ft_popen/) |
| `picoshell` | Build a pipeline of commands | [`picoshell/`](picoshell/) |
| `vbc` | Parse and evaluate arithmetic expressions | [`vbc/`](vbc/) |

## `ft_popen`

### Objective

Implement:

```c
int ft_popen(const char *file, char *const argv[], char type);
```

The function launches `file` with `argv` using `execvp` and returns the
parent-side end of a pipe:


### Process layout

For read mode, the child redirects `stdout` to the pipe's write end and the
parent keeps the read end. For write mode, the child redirects `stdin` to the
pipe's read end and the parent keeps the write end. Every unused descriptor
must be closed in both processes.

Allowed functions: `pipe`, `fork`, `dup2`, `execvp`, `close`, `exit`.

Example:

```c
int fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
```

Compile the standalone implementation with:

```sh
cd ft_popen
cc -Wall -Wextra -Werror -c popen.c
```

## `picoshell`

### Objective

Implement a function that executes commands connected like a shell pipeline.
Each command is represented by a NULL-terminated argument vector, for example:

```c
char *cmds[][4] = {
	{"echo", "hello", NULL},
	{"wc", "-c", NULL},
	NULL
};
```

The output of one command becomes the input of the next command. The usual
pipeline setup is:

1. Create a pipe when another command follows.
2. Fork the current command.
3. Connect the previous pipe to `stdin`.
4. Connect the new pipe to `stdout`.
5. Close descriptors that are no longer needed.
6. Wait for all children before returning.

The implementation must propagate failures from `pipe` and `fork`, avoid file
descriptor leaks, and prevent the parent from hanging by closing each unused
pipe end.

## `vbc`

### Objective

Evaluate an arithmetic expression supplied as the only command-line argument.
The language contains:

```text
expression := term ('+' term)*
term       := factor ('*' factor)*
factor     := digit | '(' expression ')'
```

Multiplication has higher precedence than addition, and parentheses can be
nested. Values are single digits (`0` through `9`) and whitespace does not
need to be supported.

Examples:

```sh
cd vbc
cc -Wall -Wextra -Werror vbc.c -o vbc
./vbc '3*4+5'       # 17
./vbc '3+4*5'       # 23
./vbc '(3+4)*5'     # 35
./vbc '1+'          # Unexpected end of input
./vbc '1+2)'        # Unexpected token ')'
```

The parser in [`vbc/vbc.c`](vbc/vbc.c) follows the grammar with three
functions:


The syntax tree is evaluated recursively and released with `destroy_tree`.
Unexpected input must print the required diagnostic and exit with status `1`.

## Suggested validation

Run the following checks from each exercise directory:

```sh
cc -Wall -Wextra -Werror *.c
./a.out
```

For `vbc`, test precedence, nested parentheses, missing operands, unmatched
parentheses, and unexpected trailing characters. For the pipe exercises, test
both directions, empty output, command failure, multiple commands, and file
descriptor cleanup.

## Exam reminders

- Check every system call that can fail.
- Close pipe ends immediately after they are duplicated or no longer needed.
- A child must terminate if `execvp` fails.
- The parent must wait for children when the subject requires it.
- Keep parsing functions small and make ownership of allocated nodes explicit.

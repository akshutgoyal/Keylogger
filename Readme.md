# Keylogger - Project (Arch Linux Tested)

A local keystroke monitor.

- All inputs taken from the keyboard input device at the location (dev/input/event4)
- Keystrokes are written to the keystrokes.log file
- Non printable events such has /t, /b etc have been automatically converted to [SPACE], [BACKSPACE], etc
- Exits cleanly on a hotkey (Ctrl+Alt+Q), flushing the log and printing session stats (total keystrokes, runtime)

Log line format:

```
[DD/MM/YY HH:MM:SS.mmm] token
[04/10/26 22:54:38.407] h
[04/10/26 22:54:38.500] [SPACE]
```

## Files and how they are managed

The program is split into **modules**, one concern per file. The rule is:

- **`.h` (header) declares** — function prototypes.
- **`.c` (source) defines** — the actual function bodies and variables.

| file | responsibility |
|------|----------------|
| `main.c` | Open the device, install signal handlers, run the read loop, detect the exit hotkey, print stats. |
| `keys.h` / `keys.c` | Layout tables, modifier state machine, keycode → character / `[TOKEN]`. |
| `logger.h` / `logger.c` | Open the log file, write a timestamped line, flush + close on exit. |
| `Makefile` | Build rules: compile each `.c` to `.o`, then link them into `keylogger`. |

## Build and run

```
make                # build ./keylogger
./keylogger         # record; exit with Ctrl+Alt+Q (or Ctrl+C)
cat keystrokes.log  # view the captured, timestamped keystrokes
make clean          # remove *.o and the binary
```

**Permissions:** `/dev/input/event4` is readable only by `root` and members of
the `input` group. Either add your user to that group (then log out/in, or
`newgrp input`), or run with `sudo`.

# PTASKR: A terminal todo list
**ptaskr** is a lightweight, minimalistic todo list application designed to be just a slight
step above plain text files. GUI-Luddites welcome.

Passing a filename on the command line will allow you to open a specific list file; otherwise
list `~/.config/ptaskr/default.list` will be used.

## How to use

| Keyboard | Function |   
| :---: | --- |   
| <kbd>&uarr;</kbd> <kbd>&darr;</kbd> <kbd>PgUp</kbd> <kbd>PgDn</kbd> | Navigate list |
| <kbd>&rarr;</kbd> <kbd>&larr;</kbd> | Add/remove selected task progress |
| <kbd>Ctrl</kbd>+<kbd>&uarr;</kbd>&ensp;<kbd>Ctrl</kbd>+<kbd>&darr;</kbd> | Shift selected task up/down |
| <kbd>Ctrl</kbd>+<kbd>&rarr;</kbd>&ensp;<kbd>Ctrl</kbd>+<kbd>&larr;</kbd> | Increase/decrease selected task goal |
| <kbd>Ctrl</kbd>+<kbd>E</kbd> | Edit selected task description |
| <kbd>Ctrl</kbd>+<kbd>D</kbd> | Delete selected task |
| <kbd>Ctrl</kbd>+<kbd>R</kbd> | Rename list |
| <kbd>Ctrl</kbd>+<kbd>S</kbd> | Save list to file |
| <kbd>F1</kbd>&ensp;<kbd>Ctrl</kbd>+<kbd>X</kbd> | Quit ptaskr |

## Known bugs
When selecting `[New Task]` on macOS, the input for the description seems to get filled with some
nonsense data. I have no idea why this happens.

## Dependencies
- `libncurses5-dev`
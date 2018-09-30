# PTASKR: A terminal todo list
**ptaskr** is a lightweight, minimalistic todo list application designed to be just a slight
step above plain text files. GUI-Luddites welcome.

Passing a filename on the command line will allow you to open a specific list file; otherwise
list `~/.config/ptaskr/default.list` will be used.

# Installation
I don't know how to make deb files and GNU docs scare me so you should build from source.
```bash
git clone https://github.com/pjpollina/ptaskr
cd ptaskr
make && sudo make install
```
Requires libncurses development headers.

# Usage
Run `ptaskr [filename]` from your terminal of choice and you're good to go. No fancy options required.   
To add a task to your list, highlight the [New Task] option and hit <kbd>Enter</kbd> (or <kbd>Return</kbd> if you want to Think Different&trade;).
The full list of controls is listed below

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

# Credits
Copyright &copy; 2018 PJ Pollina. Released under the MIT License. See [LICENSE](LICENSE) for more information.
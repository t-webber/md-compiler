# Markdown Compiler

Converts a `.md` file to a `.html` file.

## Usage

> Usage: `md2html <file>.md`.
>
> Output: `<file>.html`, in the same directory than `<file>.md`.

## Compilation

To compile the project, I highly recommand using [xmake](https://www.xmake.io).
The supported plateforms can be found [here](https://xmake.io/#/?id=supported-platforms).

Build using `xmake` then run with `xmake run`.

The executable file's path is `./build/<operating system>/<structure>/release/md2html`.

## Known issues

 - The stars written are ignored
 - In verbatim, the name of the language must be whole: no abbreviations are supported yet (e.g. js for javascript).
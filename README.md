# Football Highlight Wizard

A C++ application to identify potential highlights from a football (soccer) game event log.

## Description

This program reads a text file containing time-stamped events from a football match and identifies key moments like goals, red cards, etc., outputting them as highlights.

## Features (Planned/Implemented)

*   [x] Reads game events from a CSV-like text file.
*   [x] Identifies basic highlights (Goals, Red Cards).
*   [ ] More sophisticated highlight detection (e.g., near misses, saves - requires more detailed input).
*   [ ] Command-line arguments for input/output files.
*   [ ] (Future) Integration with video timestamps.
*   [ ] (Future) GUI.

## Input File Format

The input file should be a text file where each line represents a game event in the following format:

`<minute>,<EVENT_TYPE>,<description>`

Example (`events.log`):

Supported `EVENT_TYPE`s currently: `GAME_START`, `HALF_TIME`, `FULL_TIME`, `GOAL`, `YELLOW_CARD`, `RED_CARD`, `FOUL`, `SUBSTITUTION`, `OTHER`.

## Building

This project uses CMake.

1.  **Clone the repository:**
    ```bash
    git clone <your-repo-url>
    cd Football-Highlight-Wizard
    ```
2.  **Configure using CMake:**
    ```bash
    cmake -S . -B build
    ```
3.  **Build the project:**
    ```bash
    cmake --build build
    ```
    (Alternatively, on Linux/macOS: `cd build && make`)
    (Alternatively, on Windows with Visual Studio: Open the generated solution file in the `build` directory)

## Running

After building, the executable will be in the `build` directory (or a subdirectory like `build/Debug`).

```bash
./build/FootballHighlightWizard <path/to/your/events.log>
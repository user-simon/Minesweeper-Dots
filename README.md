![title img](img/title.png)

## Dots is a sleek, modern implementation of Minesweeper, focused on high-scores and with cross-platform compatability

------

## Features
#### :blue_square:  Sleek design
#### :baby_bottle:  Easy to use*
#### :memo: Configurable
#### :triangular_flag_on_post:  Keeps track of high-scores

*\* Provided the user is familiar with Minesweeper and is literate.*

------

## About
The project was born out of a personal "need" for a dedicated Minesweeper app which keeps track of high-scores for different difficulties. During the development I decided
to also spice up (spice down?) the visuals a tad, simplifying things as much as possible. Because of the somewhat abstract representation of numbers, Dots is probably best suited for those who are familiar with the classic Minesweeper and its colors.

#### Keybinds
**Mouse1** opens cells, **Mouse2** toggles flags ... and of course there is functionality for the "macro" feature often seen in Minesweeper implementations where the spacebar opens the neighbours of cells with the correct amount of neighbouring flags, this can be used either with **Spacebar** or with **Mouse3**.

The difficulty can be changed with the **Number-keys** (1-3 for beginner, intermediate and expert, respectively) and **Escape** resets the game.

## Design
The cells all have a simplified design, with squares of different colors replacing icons.

<p align="center">
<img src="img/design.png" width="50%">
</p>

... and the UI is right where you left it, with flags to the top left and the timer to the top right.

<p align="center">
<img src="img/ui.png" width="75%">
</p>

## High-scores
High-scores are automatically loaded, updated, and saved during the lifetime of the program. The current high-score for each difficulty is displayed in the title bar, following the name of the difficulty.

## Custom configuration
Difficulties can be added and modified by changing the [config.json](src/config.json) file which contains data pertinent to each difficulty as well as the default difficulty to be loaded when the program starts. In modifying this file, you could change difficulty names, mine counts, grid sizes, and the high-scores (denoted as `record`).

You could also add your own, custom difficulties by adding a json structure like the existing ones to the difficulty array. These can be accessed by pressing the **Number key** corresponding to the difficulties' index within the array, this unfortunetely limits the total amount of difficulties to 9.

#### Example:

The json data:
```json
{
    "mines": 2,
    "name": "Custom difficulty",
    "record": -1,
    "size": [
        25,
        25
    ]
}
```

would yield a grid looking as such:

<p align="center">
<img src="img/custom_diff.png" width="75%">
</p>

## Building
The project uses C++17 and [SFML](https://www.sfml-dev.org/) for rendering and user input so you'll have to link against that. Because the program is intended to run on multiple platforms, the lib `sfml-main-s` is used to use `main` as an entry-point, as opposed to `WinMain` on Windows, et c.

## Todo
- [ ] Create a release for Linux
- [ ] Add notice when a new high-score is set
- [ ] Add functionality for a 10th difficulty by utilizing the `0` key for index 9
- [ ] Improve code documentation

## Credit
The font used for the project is [Silkscreen](https://kottke.org/plus/type/silkscreen/index.html).

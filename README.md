![title img](img/title.png)

## Dots is a sleek, modern Minesweeper client, focused on high-scores and with cross-platform compatability

------

## Features
#### :blue_square: Sleek design
#### :baby_bottle: Easy to use*
#### :memo: Configurable
#### :triangular_flag_on_post: Keeps track of statistics

*\* Provided the user is familiar with Minesweeper and is literate.*

------

## About
The project was born out of a personal "need" for a dedicated Minesweeper client which keeps track of statistics for different difficulties. During the development I decided
to also spice up (spice down?) the visuals a tad, simplifying things as much as possible. Because of the somewhat abstract representation of numbers, Dots is probably best suited for those who are familiar with the classic Minesweeper and its colors.

#### Keybinds
**Mouse1** opens cells, **Mouse2** toggles flags ... and of course there is functionality for the "macro" feature often seen in Minesweeper implementations where the spacebar opens the neighbours of cells with the correct amount of neighbouring flags, this can be used either with **Spacebar** or with **Mouse3**.

The difficulty can be changed with the **Number-keys** (1-3 for beginner, intermediate and expert, respectively) and **Escape** resets the game.

## Design
The cells all have a simplified design, with squares of different colors replacing icons.

<p align="center">
<img src="img/design.png" width="75%">
</p>

... and the UI is right where you left it, with flags remaining to the top left and the timer to the top right.

<p align="center">
<img src="img/ui.png" width="40%">
</p>

## Statistics
Statistics are automatically loaded, updated, and saved during the lifetime of the program. The current high-and average score for each difficulty is displayed in the title bar, following the name of the difficulty.

## Custom configuration
Difficulties can be added and modified by changing the [config.json](src/config.json) file which contains data pertinent to each difficulty as well as the default difficulty to be loaded when the program starts. In modifying this file, you could change difficulty names, mine counts, grid sizes, high-scores (denoted as `record`), times played, and the average scores.

You could also add your own, custom difficulties by adding a json structure like the existing ones to the difficulty array. These can be accessed by pressing the **Number key** corresponding to the difficulties' index within the array, this unfortunately limits the total amount of difficulties to 9.

Note that statistics such as the average or high-score don't have to be included upon first addition to the config file.

#### Example:

Adding the following json data to the config file:
```json
{
    "mines": 5,
    "name": "Custom difficulty",
    "size": [
        15,
        10
    ]
}
```

and pressing the **Number key 4** would yield this result:

<p align="center">
<img src="img/custom_diff.png" width="55%">
</p>

## Installing
Simply download and extract the latest release at a location of your choosing and you're good to go.

## Updating
Generally, in order to update one simply has to download the latest version and replace the config file to keep existing statistics; in some cases, however, 
the structure of the config file might be different between different versions, in which case you'd have to manually transfer the old statistics to the provided,
updated config file.

## Building
The project uses C++17 and [SFML](https://www.sfml-dev.org/) for rendering and user input so you'll have to link against that. Because the program is intended to run on multiple platforms, the lib `sfml-main-s` is used to use `main` as an entry-point, as opposed to `WinMain` on Windows, et c.

You also need to include [nlohmann's json library](https://github.com/nlohmann/json) as that is used for serializing/deserializing the difficulty data.

## Todo
- [ ] Create a release for Linux
- [ ] Add functionality for a 10th difficulty by utilizing the `0` key for index 9
- [ ] Improve code documentation
- [ ] Make the window size scale depending on monitor resolution, so the window doesn't go out of bounds

## Credit
The font used for the project is [Silkscreen](https://kottke.org/plus/type/silkscreen/index.html).

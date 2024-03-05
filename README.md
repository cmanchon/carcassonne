

# Carcassonne
In this repository, I developed the french board game Carcassonne in C for a school project, with an AI explained below.

## What is Carcassonne?
Carcassonne is a game where players play tiles on a board to build cities, roads and other structures. The goal is to place pawns named "meeples" on those structures to earn points, and to build structures completly to earn more points.

## How to run
While in the `carcassonne-game` folder, run `make` to compile the program, and then execute the `main` file to play on terminal. Add the `clear`option to clear the terminal beforehand.

## How the AI works
The AI uses a fonction that counts the points a move could make, saves every move it could make and then chooses which move to play, depending on its difficulty. It's pretty simple, but it works surprisingly well! 


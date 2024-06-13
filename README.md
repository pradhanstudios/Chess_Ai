# Chess_AI
WIP chess AI with C++ and an interface in Python (Pygame)


# Engine
All of the following are located in "logic" folder 

And all of the concepts here can be found on the Chess Programming Wiki

The engine currently includes:
* **Movegen**
    * Magic Bitboards
    * Kogge Stone Generators (for king safety evaluation)

* **Search**
    * Quiescence Search
        * Alpha Beta Pruning
        * Stand Pat Pruning
        * Delta Pruning
    * Iterative Deepening
    * Alpha Beta Pruning
    * Principal Variation Search
    * Late Move Reductions
    * Reverse Futility Pruning
    * Testing ~~Internal Iterative Reductions~~
    * ~~Null Move Pruning~~ is buggy
* **Move Ordering**
    * PV Node
    * Killer Heuristic
    * Promotions
    * MVV-LVA
* **Evaluation** (The following values are all tuned by "tuner" folder)
    * Tempo
    * Piece Square Tables
    * Threat by Knight or Bishop
    * Threat by Rook
    * Pawn
        * Passed Pawn Rank
        * Protected Pawn Rank
        * Doubled Pawn
        * Isolated Pawn
    * Bishop
        * Bishop Pair
    * Rook
        * Rook on Open file
        * Rook on Semi Open file
    * King
        * Pieces Attacking and Defending King
        * King on Open file
        * King on Semi Open file

## Prerequisites

Before running this project, the packages listed within `requirements.txt` must be installed.

```pip install -r requirements.txt```

## Running

Run the `game.py` file in the project's root directory using Python.

```python game.py```
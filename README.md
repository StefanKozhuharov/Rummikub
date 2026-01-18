# Rummikub
This project is a console implementation of a simplified **Rummikub-style** tile game.
Players take turns placing combinations on the table, adding to existing ones, or stealing tiles (special rule).
The goal is to **get rid of all tiles in your hand**.

---

## Game Rules (Short Summary)

### Valid combinations
There are **two types** of combinations:

#### Group
- **3 or 4 tiles**
- Same value (e.g. 7,7,7)
- All **different colours**
- Jokers may replace missing tiles

#### Series (Run)
- **At least 3 tiles**
- Same colour
- Values must form a consecutive sequence (e.g. 4-5-6)
- Jokers may fill gaps

---

## Jokers
- Joker value is treated as a wildcard.
- In scoring, Joker counts as **30 points**.
- When stealing a Joker from the table, you **must replace it** with a valid tile from your hand that keeps the table combination valid.

---

## Initial 30 Rule
Each player must first place combinations with at least **30 total points** (using only tiles from their hand).
After completing this once, the player can:
- add tiles to table combinations
- steal tiles
- modify the table

---

## Turn Flow

Each turn the player can choose actions until they either:
- **draw a tile** (turn ends automatically), or
- **end the turn** (only possible if they already played something)

---

### Main Turn Actions
#### `1 - Play new combinations`
Allows the player to play one or more combinations from their hand to the table.

- Input format supports multiple combinations separated by `|`
After selection you must confirm:
- `y` = play
- `n` = reselect
- `d` = draw tile (ends turn)

---

#### `2 - Add tile(s) to existing table combination`
Lets the player choose a table combination and add tiles from hand to it.

- You select tile indexes from your hand.
- The game checks if the resulting table combination is still valid.
- If invalid -> it reverts.

---

#### `3 - Draw a tile (ends turn)`
The player draws **1 tile** and the turn ends immediately.

---

#### `4 - Steal tile(s) from the table`
Special action: steal tiles from an existing table combination, but you MUST use stolen tiles on the same turn.

Steps:
1. Select which combination from the table to steal from.
2. Select tile positions inside that combination.
3. If the remaining table combination becomes invalid:
 - either stealing is rejected, OR
 - the remaining series may be split into **two valid series** (if possible)
4. After stealing you choose what to do with stolen tiles:
   
##### Steal mode commands
- `c` = combine stolen tiles + tiles from hand to create a new valid combination and place it on the table
- `a` = place stolen tiles back onto existing table combinations (must keep them valid)
- `s` = cancel and reselect which tiles to steal
- `d` = draw tile (ends turn)

If steal succeeds, used hand tiles are removed properly.

---

#### `5 - End turn`
Only available after the player successfully played something on the table.
Ends the turn without drawing.

---

## Win Conditions

The game ends when:
1. A player reaches **0 tiles in hand** → immediate winner
2. The deck becomes empty → winner is the player with the **lowest hand score**

Scoring:
- Joker = 30 points
- Normal tile = its number value

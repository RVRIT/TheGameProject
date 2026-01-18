## The Game

A C++ implementation of the well-known card-based board game **"The Game"**.

This project supports **2+ players** and includes the original gameplay mechanics:
- higher cards go on **ascending** piles
- lower cards go on **descending** piles

It also implements the **Backwards Trick**:
- you may place a card **10 lower** on an ascending pile
- you may place a card **10 higher** on a descending pile

## Rules of the Game

The objective is to play as many cards as possible from the deck (**cards range from 2 to 98**) without getting stuck.

- There are **four piles**:
  - **2 ascending piles**
  - **2 descending piles**
- On **ascending piles**, cards must normally be placed in increasing order.
- On **descending piles**, cards must normally be placed in decreasing order.
- **Backwards Trick**:
  - you can place a card with value **10 lower** than the top card of an ascending pile
  - or **10 higher** than the top card of a descending pile

The game ends when:
- all cards from the deck have been played (**win**)
- one player is unable to play any card (**loss for all players**)

## Features

- Local multiplayer co-op
- System for updating player rating after each finished game
- In-game chat module where player can discuss strategies **without mentioning cards' values**
- SFML-based graphical user interface

## Dependencies

- **SFML** - GUI implementation
- **Crow** + **cpr** - client-server networking
- **sqlite-orm** - database management
- **OpenSSL** - password hashing

> In order to play the game all dependencies must be installed and available on your build system.

## How to play

1. The host must first create a lobby.
2. Players can join said lobby by either:
     - selecting the lobby from the lobby list or
     - entering the **Lobby ID** in the dedicated textbox.
3. Once there are at least **2 players** and everyone is marked as **ready**, the host can start the game.
4. Players are then moved into the game scene.
5. The current player is displayed at the top of the screen together with the cards they played that turn.
6. On your turn:
   - you must place **at least 2 cards** if the deck still contains cards or
   - **at least 1 card** if the deck is empty.
7. To end your turn you must press on the deck to the right of your hand.

## Source

The project is inspired by the original board game:

**The Game** — https://officialgamerules.org/game-rules/the-game/

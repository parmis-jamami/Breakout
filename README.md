# 🎮 Breakout Game

A console-based implementation of the classic **Breakout** game, developed in **C++**.

The player controls a paddle at the bottom of the screen and tries to destroy all blocks by bouncing a ball while preventing it from falling. The game also includes user accounts, game saving/loading, game history, high scores, sound effects, and customizable ball styles.

---

## 📌 Table of Contents

- [About the Project](#-about-the-project)
- [Features](#-features)
- [Gameplay](#-gameplay)
- [Controls](#-controls)
- [Game Rules](#-game-rules)
- [User System](#-user-system)
- [Save & Load System](#-save--load-system)
- [Game History](#-game-history)
- [Settings](#️-settings)
- [Project Structure](#-project-structure)
- [How to Run](#-how-to-run)
- [Game Data](#-game-data)
- [Authors](#-authors)

---

## 🕹️ About the Project

**Breakout Game** is a terminal-based recreation of the classic arcade game.

The main goal is simple: control the paddle, keep the ball inside the game area, and destroy every block.

The project goes beyond the basic Breakout gameplay by providing several additional systems such as:

- User registration and login
- Individual high scores
- Save and load functionality
- Game history
- Sound effects
- Different ball styles
- Pause and restart options
- Game timer

The entire game runs inside the **Windows Console**.

---

## ✨ Features

### 🎮 Gameplay

- Classic Breakout-style gameplay
- 60 destructible blocks
- Paddle movement
- Ball collision with walls, blocks, and paddle
- Different ball directions depending on paddle collision
- Score system
- Three-heart life system
- Win and game-over conditions
- Game timer

### 👤 User Accounts

Players can create and manage their own accounts.

Available account features include:

- Create account
- Login
- Logout
- Delete account
- Personal high score
- Separate saved game for each user

### 💾 Save & Load

Players can save an unfinished game and continue it later.

The saved state includes:

- Current score
- Remaining hearts
- Paddle position
- Ball position
- Ball direction and speed
- Remaining blocks
- Current game time
- Current game state

Each player has their own save file.

### 📊 Game History

Finished games are stored in the game history.

For each game, the following information is recorded:

- Username
- Final score
- Win/Loss result
- Remaining hearts
- End date and time
- Total game time

Game records are sorted based on score.

### 🔊 Sound Effects

The game includes sound effects for:

- Breaking a block
- Losing the ball
- Victory
- Game over

Sound can also be enabled or disabled from the settings menu.

### 🎨 Ball Customization

Players can choose between different ball styles:

`●` `🟣` `🔵` `🟠` `🌏` `🥎` `🏀`

---

## 🎯 Gameplay

At the beginning of the game, the ball stays attached to the paddle.

Move the paddle to choose the initial position of the ball and press **Space** to launch it.

The ball will bounce between the walls, blocks, and paddle.

Every destroyed block gives:

**+10 points**

The goal is to destroy all blocks before losing all three hearts.

---

## ⌨️ Controls

| Key | Action |
|-----|--------|
| `A` | Move paddle left |
| `D` | Move paddle right |
| `SPACE` | Launch the ball |
| `P` | Pause the game |
| `W` | Move up in menus |
| `S` | Move down in menus |
| `ENTER` | Select menu option |
| `Q` | Return/Exit in supported menus |
| `←` / `→` | Navigate game history pages |

---

## ❤️ Game Rules

The player starts each new game with:

```text
3 Hearts
```

If the ball falls below the paddle, one heart is lost.

```text
❤️ ❤️ ❤️
   ↓
❤️ ❤️ 💔
```

The ball then returns to the paddle.

### 🏆 Winning

Destroy all blocks:

```text
Remaining Blocks = 0
```

and the game ends with a victory.

### 💔 Losing

Lose all three hearts:

```text
Remaining Hearts = 0
```

and the game ends with **Game Over**.

---

## 👤 User System

The game includes a simple account system.

When starting a new game, players can either:

```text
Login to your account
Create account
```



User information is stored locally inside:

```text
users.txt
```

The player's high score is automatically updated whenever a new personal record is reached.

---

## 💾 Save & Load System

Players can pause the game using:

```text
P
```

The pause menu provides:

```text
Continue
Restart
Save the game
Quit
```

Selecting **Save the game** stores the current game state.

Save files follow this format:

```text
save_<username>.txt
```

For example:

```text
save_player1.txt
```

This means every account has its own independent saved game.

After loading, the game continues using the stored paddle position, ball position, score, hearts, timer, and remaining blocks.

---

## 📊 Game History

Completed games are stored inside:

```text
gameHistory.txt
```

The history displays information such as:

```text
Game 1:

Player won the game! 🎉
Score: 600
Remaining hearts: 2
End date and time: ...
```

If multiple games exist, players can navigate between history pages using the arrow keys.

---

## ⚙️ Settings

The Settings menu contains:

```text
Ball shape
Clear game history
Active sound
Log out
Delete account
Quit
```

### 🎨 Ball Shape

Allows the player to change the appearance of the ball.

### 🗑️ Clear Game History

Removes all previous game records.

### 🔊 Active Sound

Turns game sound effects on or off.

### 🚪 Log Out

Logs the current user out of the game.

### ❌ Delete Account

Deletes the current account and its associated data.

---

## 🗂️ Project Structure

A typical project directory can look like this:

```text
Breakout/
│
├── Breakout.cpp
├── users.txt
├── gameHistory.txt
│
├── save_<username>.txt
│
└── sounds/
    ├── breakingBlock.wav
    ├── Fall.wav
    ├── victory.wav
    └── gameOver.wav
```

Some `.txt` files are automatically created or updated while the game is running.

---




## 🚀 How to Run

### 1. Clone the repository

```bash
git clone <repository-url>
```

### 2. Enter the project directory

```bash
cd Breakout
```

### 3. Compile the program

For example, using a compatible C++ compiler:

```bash
g++ Breakout.cpp -o Breakout.exe -lwinmm
```

### 4. Run the game

```bash
Breakout.exe
```

> **Note:** The project uses Windows-specific APIs such as `windows.h`, `conio.h`, and `PlaySound`, so it should be compiled and executed on Windows.

---

## 📁 Game Data

The game uses local text files to store persistent information.

| File | Purpose |
|------|---------|
| `users.txt` | Stores user accounts and high scores |
| `gameHistory.txt` | Stores completed game records |
| `save_<username>.txt` | Stores the saved game of a specific user |
| `sounds/*.wav` | Contains game sound effects |

---







## 🧠 Main Concepts Used

This project demonstrates several fundamental C++ programming concepts:

- Structures (`struct`)
- Functions
- Arrays
- Two-dimensional arrays
- Loops
- Conditional statements
- File handling
- References
- Strings
- Console input handling
- Time management
- Basic collision detection
- Game loops
- Persistent data storage

---

## 🖥️ Main Menu

The main menu provides access to:

```text
New Game
Load Game
Help
Game History
Setting
Exit
```

The selected option is highlighted and can be changed using `W` and `S`.

---








## 👥 Authors

Developed by:

- [Fateme Azizi](https://github.com/F4temeAzizi)
- [Parmis Jamami](https://github.com/parmis-jamami)

---


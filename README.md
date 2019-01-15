# CardsAgainstK
###### By Camilla Cheng, Joan Chirinos

## Project Description
StuyCS-themed Cards Against Humanity clone, written in C as a System Level Programming final project.

*\* This card pack features 20 "Mr. K" white cards, as well as cards of your other favorite CS teachers! Fun for the whole class!*

## How to Run:
#### For both client and server
1. ```$ git clone <REPO_CLONE_URL>```
2. ```$ cd <REPO_NAME>```
3. ```<PATH_TO_REPO> $ make```

|                     For Server                    |                           For Players                           |
| ------------------------------------------------- | --------------------------------------------------------------- |
|       4.  ```<PATH_TO_REPO> $ make server```      |        6. ```<PATH_TO_REPO> $ make client <SERVER_IP>```        |
| 5. Follow instructions on terminal to set up game | 7. Wait for remaining clients to join, then play CardsAgainstK! |

## Devlog
### 2018-12-31 -- _Joan Chirinos_
- Added black and white cards from csv files
### 2018-12-31 -- _Camilla Cheng_
- Added custom cards
### 2019-01-04 -- _Joan Chirinos_
- Got the basic server without client working
### 2019-01-04 -- _Camilla Cheng_
- Added helper functions for getting black and white cards
### 2019-01-07 -- _Camilla Cheng_
- Added white card distribution to clients
- Server/client working, but not getting correct output/input
### 2019-01-07 -- _Joan Chirinos_
- Added deck shuffling, created struct deck implementation
- Edited white card distribution to run smoother
### 2019-01-08 -- _Camilla Cheng_
- Fixed compiler errors
- Added black card distribution
- Displayed white cards to clients, not working
### 2019-01-08 -- _Joan Chirinos_
- Established a foundation of methods in the ```main()```
  - This was used to lay down a groundwork of what we needed to work on, and in what order they would show up
### 2019-01-09 -- _Camilla Cheng, Joan Chirinos_
- Fixed bug caused by freeing of cards
- Fixed parse_cards
- Prettifyed a little
### 2019-01-09 -- _Camilla Cheng_
- Implemented Czars and czar rotation
- Implemented card submission
- Implemented broadcasting selected cards to czar
### 2019-01-09 -- _Joan Chirinos_
- Finished implementing most client-side Czar functions
- Implemented ```endgame_check()```
- Tweaked function calls in ```main()```
- Finished implementing most server-side Czar functions
### 2019-01-10 -- _Camilla Cheng, Joan Chirinos_
- Some groundwork on server and client
- Cleaned some print statements
### 2019-01-11 -- _Joan Chirinos_
- Cleaned up printing-screen into one function
- Implemented player names
- Attempted to implement score
  - Ran into a bug: scores work on Ubuntu for Windows, and Ubuntu machines (homer), but do not work on MacOS
### 2019-01-13 -- _Camilla Cheng_
- Added many white cards
- Robustified existing code
- Prettified
### 2019-01-14 _Camilla Cheng_
- Added pretty fireworks
- General beautification of prints
- Started socketing
- Added player, score limits
### 2019-01-14 _Joan Chirinos_
- Finished implementing sockets
- Final round of testing via homer server and Camilla's laptop (works!)
### 2019-01-15 _Camilla Cheng, Joan Chirinos_
- Updated README xD

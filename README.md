# CardsAgainstK
###### By Camilla Cheng, Joan Chirinos

## Project Description
StuyCS-themed Cards Against Humanity clone, written in C as a System Level Programming final project.

## How to Run:
#### For both client and server
1. ```$ git clone <REPO_CLONE_URL>```
2. ```$ cd <REPO_NAME>```
3. ```<PATH_TO_REPO> $ make```

|                     For Server                    |                           For Players                           |
| ------------------------------------------------- | --------------------------------------------------------------- |
|       4.  ```<PATH_TO_REPO> $ make server```      |        4. ```<PATH_TO_REPO> $ make client <SERVER_IP>```        |
| 5. Follow instructions on terminal to set up game | 5. Wait for remaining clients to join, then play CardsAgainstK! |

## Dev Log
* 2018-12-31 // Joan Chirinos
  - added black and white cards from csv files
* 2019-01-04 // Joan Chirinos
  - got the basic server without client working
* 2019-01-04 // Camilla Cheng
  - added helper functions for getting black and white cards
  * 2019-01-07 // Camilla Cheng
  - added white card distribution to clients
  - server/client working, but not getting correct output/input
* 2019-01-07 // Joan Chirinos
  - added deck shuffling, created struct deck implementation
  - edited white card distribution to run smoother
* 2019-01-08 // Camilla Cheng
  - fixed compiler errors
  - added black card distribution displayed white cards to clients, not working
* 2019-01-08 // Joan


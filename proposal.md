# Ideas for upgrading the snake game to meet capstone requirements
Organised by ruberic section

## Loops, Functions, IO
The project reads data from a file and processes the and processes the data - Create fixed obsticles that cant be hit from a config file
    Better Idea use a config file to specify the grid size, number of fruits spawned at a time etc and read it in to startup game.
The Project Accepts user input and processes it - implement the spacebar as a game pause button?

## OOP
Have multiple types of obstacles created, 1 type instantly ends the game (kills the snake), the other type reduces snake length and thus score by 1
Parent class obstacle with two children nonlethal and lethal
    maybe can have more types that differ only graphically
    other type that causes that doesnt damage the snake but causes it to forcibly turn when it runs into it (maybe this should happen for all of them)

## memory management 
??? -- think of something 
Use references - easy enough
others wont be terribly difficult

## Concurrency
One of the obstacles is poised - or maybe some of the food is occasionally poisoned, when hit it starts a new thread which sporadically does damage to the snake - reducing score / lenght until it dies
Just need to use multithreading and use a mutex or lock 
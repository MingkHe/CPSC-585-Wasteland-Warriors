# --Wasteland Warrior--
## Milestone #2

For this milestone, we have implemented each of the basic subsystems in our game. Our goal was to get each basic system to
comunicate within in our framework. The input system is capturing input from the keyboard and controller. The audio system 
is able to start/stop and pause/play music files. The physics system is monitoring collisions for objects and the ground plane.
The physics engine is also handling the movement of the car on the ground plane. The rendering system is handling the rendering
of the red box and the ground plane.

### To Build and Run:
To build, download or clone the Wasteland Warrior files from https://github.com/MingkHe/CPSC-585-Wasteland-Warriors/tree/Wasteland_Warrior
then open the Wasteland Warrior solution file in Visual Studio 2017, found in the folder CPSC-585-Wasteland-Warriors\Wasteland_Warrior.
Then build and run the program.

Please note that you may need to change the Windows SDK version first depending on which version you are running. Also, the code needs to
be compiled using only the x86 Debug mode.


### Input Controls: (currently keyboard only)

    Driving:
        W to accelerate forward
        S to Brake and Reverse
        A/D for steering left and right respectively
        SPACE for Handbrake

    Camera:
        Mouse to move camera

    Music:
        N to start/pause/resume music
        M to stop music
        Left Shift to switch music

    ESCAPE to quit game

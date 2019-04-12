# -- Wasteland Warrior --

## Version 1.0

For the final submission we:
    - Updated the map
    - Improved shadows
    - Increased the paceing for the game
    - Provide player with feedback on damage dealt and enemy health
    - Added the remaining game modes
    - Added a gun as reward for completing the objective
    - Implemented release mode
    - Added our narative of being trapped and escaping
    - Cleaned up a lot of warnings
    - Fixed most game play bugs.

### To Build and Run:
To build, download or clone the Wasteland Warrior files from https://github.com/MingkHe/CPSC-585-Wasteland-Warriors/tree/Wasteland_Warrior
then open the Wasteland Warrior solution file in Visual Studio 2017, found in the folder CPSC-585-Wasteland-Warriors\Wasteland_Warrior.
Then build and run the program.

*Please note that you may need to change the Windows SDK version first depending on which version you are running.

### Gameplay:

    As the Wasteland Warrior your goal is to survive the apocolypse and escape to freedom. You will face 5
    waves of enemies who are trying to destroy your vehicle. Once you complete all the waves, the game will end.
    There are two ways to attack your enemies: You can either ram them until their vehicles are destroyed, or you 
    can shoot them. During each wave you will have different objectives. The wave will not end until all the 
    objetives are completed.
    
    Game Modes:
        Survival:
            Defeat all the enemies.
            
        Headhunter:
            An enemy is trying to escape, If this enemy (Purple on the radar) is defeated, the gun will activate.
            
        Checkpoint:
            Reach all the checkpoints to activate the gun.
            
        Payload:
            Collect the payload and deliver it to the checkpoints to to activate the gun.
            
        Boss Battle
            Defeat the boss to finish last wave.
    
### Input Controls:

    Driving:
        W or Right Trigger to Accelerate forward
        S or Left Trigger to Brake and Reverse
        A/D or Left Stick for steering left and right respectively
        SPACE or B on gamepad for Handbrake
        
        R or Y on gamepad to reset your vehicle after being flipped or stuck
        
    Weapon:
         left mouse or X button to shoot gun when activated

    Camera:
        Move Mouse while holding the Left Mouse Button or move the Right Stick to move the camera
        V or Right Shoulder Button to toggle the view between third person, Hood Cam, and Cockpit view
        Hold down E or Left Shoulder Button to look behind you

    Screen:
        UP Arrow or UP on Dpad to move up
        DOWN Arrow or DOWN on Dpad to move down
        ENTER or A on gamepad to select option
        M or Menu button on gamepad to enter pause menu
        F to toggle fullscreen/windowed
    
    Start Menu options:
        - start: start the game
        - quit: exit the game
        
    Pause Menu options:
        - resume: resume the game
        - restart: restart the game
        - menu: go to start menu
        
*Any gamepad is supported. However, Xbox 360 controller buttons are used as a default to explain gamepad input options.

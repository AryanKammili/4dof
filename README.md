Hello!

This is the project repo for my robot arm. I am building a robot arm with 4 degrees of freedom to pick up objects with almost 20 inches of reach. The robot arm's code will all be in this repo along with future .urdfs and other important files meant for Isaac Sim. My current plan is to use Isaac Sim to use reinforcement learning on the robot arm to move to specific positions. I am building my own closed loop control system for the stepper motors using a velocity based PID controller with magnetic encoders I mounted to the stepper motors. 

I have currently finished the code to read data from the magnetic encoder register and to move the motors using pwm signals. I am currently working on the PID controller. Physical fabrication of the robot arm is 50% done with the top two joints complete using PLA components and cycloidal gearboxes.

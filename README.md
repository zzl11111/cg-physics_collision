# Rigid Body Simulator
A Rigid Body Simulator is a sophisticated tool used in the fields of physics, engineering, and computer graphics to model the motion and interaction of solid, inflexible objects. Unlike soft body simulators, which account for deformations and fluid dynamics, a rigid body simulator assumes that the objects being simulated do not deform or change shape under applied forces or collisions. This simplification allows for more efficient calculations and is particularly useful in scenarios where object deformation is negligible or irrelevant. Common applications of rigid body simulators include video game physics, robotics, and virtual reality environments, where they provide realistic and real-time responses to physical interactions. Here we introduce a Rigid Body Simulator, which can simulate collisions and friction between two of the most fundamental shapes in geometry - balls and boxes.

## Compile

```
cmake -B build/ -DCMAKE_EXPORT_COMPILE_COMMANDS=True
cmake --build ./build -j 4
```

## Run

To run the program:
```
cd build
./main
```
Rress SPACE key to begin the simulation.

## Change the case

You can change the test case by changing the value of the `caseID` variable in the `scene.cpp`.
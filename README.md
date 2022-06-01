# car_code

The idea behind this project was inspired by the robotic arms in factories that goes back and forth in a product line. Just like the arm, this car moves back and forth between two obstacles (walls) in a straight line.

The principle used here is the PID (Proportional Integral Derivative). We used 2 ultrasonic sensors (front and rear) to measure the distance between the car and the walls. Then, an error signal is generated in the PID library, depending on some setpoint. The car slows down as it move forward until it reaches the setpoint. Once the setpoint is reached, the car moves in the opposite direction until it reaches the other setpoint, and the cycle continues.

Made by students in Damascus University in Computer and Automation Engineering Department: Ashraf AlAssi, Ibrahim AlSabagh, Amir AlAwa and Mahmoud AlHariri

Image:
![car](https://user-images.githubusercontent.com/89457541/171475271-ddb2f183-f3d0-49f5-a43b-35d2feb56584.png)

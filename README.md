📝 Project Overview
I created this project to bridge the gap between environmental sensing and human-robot interaction. Usually, a temperature sensor just shows numbers on a screen. In this project, I used those numbers to give a robot a "soul."
By processing data from a DHT11 sensor, the Arduino calculates the robot's "emotional state." If the room is too hot, the robot gets angry; if it's too humid, it actually starts to sweat!
🧠 How it works:
The Brain: Arduino reads the heat and moisture levels every 2 seconds.
The Logic: I wrote a custom "Mood Engine" that categorizes the data into three temperature zones and a humidity threshold.
The Face: The FluxGarage_RoboEyes library renders these emotions onto an I2C OLED display with smooth, non-blocking animations.  

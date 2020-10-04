# EyeSpy™
IP camera that monitors continous motion detection and alert the user with live video coverage

<h1>SUMMARY</h1>
EyeSpy™ is a proposed new idea that tracks continuous motion detection and allows the user to view live video streaming at any time. The two-axis gimbal designed prototype can cover an overall 180° wide-angle view both horizontal and vertical. The sensors are placed in a way that an object can be tracked in all the quadrants – that enable the system to be a complete surveillance mechanism with more accurate readings. The eye embodied with the camera can track the motion and provide live video streaming to the user.

<h2>SCOPE</h2>
EyeSpy™ consists of four sensors, two each in horizontal and vertical direction having a coverage of 180-degree in both the axis. This design exhibits the following features:
<ul>
<li>The eye shall continuously monitor any object by its movement and follows the object until the object is out of sight or idle.</li>
<li>Whenever the object is out of sight or idle, the eye makes a move towards its home position.</li>
<li>The camera attached to the centre of the eye captures the image of the object being monitored by the eye.</li>
<li>The design allows the user to stream the video via wireless communication in any output terminal.</li>
<li>HMI that enables the user to watch live video stream from the camera webserver.</li>
</ul>

![GitHub Logo](/block_diagram.png) 

<h3>OPERATIONAL OVERVIEW</h3>
<p>
The system comprises of four motion detection sensors and two stepper motors.Two sensors are attached in the horizontal axis and the rest two are attached in the vertical
axis. Similarly, one motor drives the eye in horizontal axis and the other in vertical axis. Whenever a motion happens within the range of any four sensor. The sensor detects the motion and the position of the sensors drives the eye in the direction of motion with the aid of motors attached to it.
</p>

![GitHub Logo](/CAD_DESIGN.jpg)

<h5>HMI</h5>
<p>The IP address of the ESP-32 cam is used by the user to stream the video in any output terminal. This gives the user real-time information about the events that is happening within the premises</p>

<h4>BENEFITS</h4>
EyeSpy is a security camera that can monitor the actions happening in a specific area. The design is a proposed new idea that can be used for both industrial and commercial
purposes.
<ol>
<li>Continuous tracking system - 
The four sensors on each side continuously monitor the location and tracks an object when detected.</li>
<li>Power efficient - 
The eye moves only when an action occurs else remains in an home position.</li>
<li>Wireless and Real-time information - 
The Wi-Fi module attached to the camera allows the user to stream the video anywhere.</li>
</ol>


<h3>CODE</h3>
/source_code.c - Source code!
[GitHub](/source_code.c)





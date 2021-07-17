# Automatic-precautionary-guard
automated systems to assist guarding corona Virus precautions for Closed Rooms (e.g. Halls, offices, etc..)

### what is this about?
to help apply corona virus precautions we implemented a system that is to be used in closed rooms as assistance to help keep people safety, the system consists of 2 applications, first is the **embedded application** based on AVR microcontroller which is responsible for controling Room doors, when to open and when to Not open, the door is opened in any of these cases:
* someone enters the room and the rooms haven't reached it's maximum capacity
*  someone leaving the room

the door is locked for entrants in any of these cases:
* the entrant doesn't wear the mask (this is the main function of the second system
* the room had reached the maximum capacity allowed as per room precaution rules

the system deals with temperature and humidity adjustments to prevent spread of infection, as well as ensuring social distancing rules between seats through an array of pressure sensors installed in the rooms, the second system is **pure AI software application** based on Tensorflow trained model for detecting face masks for the entrants.


### suggested improvements
* instead of the array of pressure sensors to ensure social distancing between seats, we can use Deep learning Model for detecting if there's people are exceeding the permissible distance or not.
* the face mask detection software demo best functioning with single entrant scan, we didn't have sufficient data or data quality to be able to do more than this simple model, there's huge room to improve it's accuracy to use it directly inside the room on all attendees.
* the face mask detection software demo can be embedded with reasberry pi to be a fully automated mask detection embedded system, but for the leak of time and hardware we made it only software, there's a room for improvement


## face mask detection samples


<img align="middle" src="screenshots/Screenshot (157).png">
<img align="middle" src="screenshots/Screenshot (158).png">
<img align="middle" src="screenshots/Screenshot (159).png">
<img align="middle" src="screenshots/Screenshot (160).png">
<img align="middle" src="screenshots/Screenshot (161).png">
<img align="middle" src="screenshots/Screenshot (162).png">
<img align="middle" src="screenshots/Screenshot (163).png">
<img align="middle" src="screenshots/Screenshot (164).png">

## simulation of the embedded system


<img align="middle" src="screenshots/Screenshot (171).png">
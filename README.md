# Warbird
Motorcycle run by on-board microcontroller
![](https://github.com/erik-dunteman/warbird/blob/master/Images/WarBird.jpg)

## Origin
Summer 2018, I built a custom Cafe Racer style motorcycle. 
Upon diving into the wiring diagram, which was difficult to find and horribly documented, I decided that I wanted better control of what was going on. I gutted the entire bike wiring system (with the exception of the engine timing chip) and rebuilt it from scratch using a microcontroller and relay module. 

![](https://github.com/erik-dunteman/warbird/blob/master/Images/WarBird_BA.jpg)

## Tech Premise
Previously, user-control buttons and switches acted as in-line circuit components for brake lights, starter circuits, etc.
With the Arduino replacing those circuits, I rewired the buttons into sensors, which triggered actions from the Arduino if certain conditions were met. For example, if the footbrake or the handbrake sensors "Read" user input, it instructs the Arduino to kick on the brake light relay.
The most interesting modification made was the removal of the key system, in exchange for an NFC reading module. The bike "unlocks" to a passive NFC transponder chip that I implanted into my hand. I am the key.

## Tech Stack
- Arduino Mega Microcontroller
- C++ Control Software
- Relay Module
- NFC Module
- Passive NFC Transponders
- Custom Wiring Harness

## Mech Stack
- Steel Subframe Welding
- Solidworks 3D CAD
- 3D Printing
- Air-Fuel Mixture Tuning

## Base Bike
1983 Yamaha Virago XV750 Midnight Edition

## Artistic Inspiration
The Cafe Racer style is marked by a visibly stripped bike frame, a perfectly horizontal line following the tank and seat bottoms, low clip-on handlebars, and a cowled seat. These builds are often done with 80s era cruisers such as the BMW R100, Honda CX500, or the Yamaha Virago XV750. I chose the Virago for its monoshock swingarm in the rear, creating a visibly open space under the seat and concentrating all engine bulk low and to the front.
The paint job was designed and done by me, color matched to the WW2 Army training biplane, #117, owned and piloted by my father.
![](https://github.com/erik-dunteman/warbird/blob/master/Images/117.jpg)

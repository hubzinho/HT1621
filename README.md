# HT1621
**My experience with reverse engineering of the Multimeter SmartMultimeter S10 BSide**

Recently, I damaged my SmartMultimeter S10 BSide, but I couldn't help noticing the display he had. He caught my attention immediately, so I decided to investigate what I could do to reuse him on another project.

I spent some time searching the internet for someone who had done something similar, but I found no reference to the reverse engineering of this specific model. The chip responsible for controlling the LCD was partially scraped, which made my mission even more challenging.

However, after a while, I remembered a video on YouTube that talked about a chip much used on displays of this type, which because it was used on customizable displays, could be the key to solving my problem. The similarity between the chip shown in the video and what I had in my hands was surprising, so I decided to investigate further.

That's when I discovered that the chip I had was the HT1621, a component widely used in applications with customizable displays. As I searched for more information about him, I found a library for Arduino, although it was turned to another type of display. I couldn't find a specific library for my model, so I decided to use the one I found and adapt the code to my display.

The adaptation was not easy and required a number of changes to the original code so that it would work correctly with the display I had. I spent considerable time on the project, adjusting the logic and details, but in the end, the result was very positive! The code may have gotten a little rustic on the parts I needed to change, but overall, it worked well.

**The Display**

![16 00 58_29b2f993](https://github.com/user-attachments/assets/fb7c274c-7820-4db5-a6d2-11a311013a87)


Reverse Engineering Project: Smart Multimeter LCD Display with HT1621 Controller
This repository contains a reverse engineering project performed on an LCD display of a smart multimeter that uses the HT1621 driver. The aim of this project is to document and share the process of analysis and decoding of the display operation, as well as provide examples of code and schemes to replicate and understand the operation of the HT1621 controller.

Contents of the Repository
Documentation: Detailed description of the reverse engineering process, including diagrams and technical explanations.

Source Code: Examples of code for communication with the HT1621 driver and manipulation of the LCD display.

Schemes: Schematic diagrams of the circuit used in the project.

Tutorials: Step by step to replicate the project and use the HT1621 driver in other projects.

How to Contribute
Contributions are welcome! Feel free to open issues, send pull requests or share your own experiences and improvements.
There are still some errors that need to be corrected, any contribution to the code will be welcome.

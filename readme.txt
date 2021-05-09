-----------------------------------------------------------------------------------
Assignment 3: Shaders and Lighting
README File
-----------------------------------------------------------------------------------
Name: Gabriel Vidaurri
abc123: wre774

-----------------------------------------------------------------------------------
What Works
-----------------------------------------------------------------------------------
 --Everything that was required in the assignment works. (Further details below.)--

1.) Multiple, Interactive Lights
    a. Point Light: 
       The program given to us by the professor already had a point light utilized 
       in it that rotates about the monkey sphere.
    b. Spotlight: 
       The default spotlight is inside the floating monkey sphere in the center 
       shooting upwards at the cylinder. You'll notice that there is a circle on 
       the cylinder above it where the spotlight is shooting towards whenever it 
       is on. The code for this was written in the display() function in main.cpp, 
       as well as in main() in phong3.vert and phong3.frag. (Toggle this feature
       on/off with the 'f' key on the keyboard.) 
    c. Calculation of Color on Surface for Multiple Lights: 
       The gl_FragColor near the end of main() in phong3.frag calculates the 
       colors of multiple surface lights.
2.) Flashlight on the End of the Gun
    a. Spotlight Attached to Gun:
       The spotlight is attached to the gun and always shoots forward in the 
       direction the camera is facing with a radius of about 5 degrees. The code 
       for this was written in the display() function in main.cpp, as well as in 
       main() in phong3.vert and phong3.frag.
    b. Toggle the Spotlight Position with the 's' Key:
       You can toggle between having the spotlight on the gun or its default
       position, used in part 1 by pressing the 's' key on the keyboard. The code 
       for this was primarily written in the appropriate case in the keyboard() 
       function as well as in the display() function of main.cpp.
    c. Toggle the Spotlight Position with the 'f' Key:
       You can toggle the spotlight on/off by pressing the 'f' key on the keyboard. 
       The code for this was primarily written in the appropriate case in the 
       keyboard() function as well as in the display() function of main.cpp.
    d. Rendering of Little Cylinder on Top of Muzzle of Gun Where Spotlight
       Position Is:
       A small cylinder on top of the muzzle of the gun is rendered, which
       represents where the spotlight position is. The code for this was written
       in the display() function of main.cpp.

-----------------------------------------------------------------------------------
What Doesn't Work
-----------------------------------------------------------------------------------
 --There isn't anything that does not work that was required of the assignment.--
MILESTONE 1 -
Procedural Terrain - Alexis Ward:
I did the procedural terrain section
	- created a function to compute FBM that works while creating new chunks
	- Use the left mouse button to remove any block immediately surrounding the player
	- Use the right mouse button to add any block within 2 - 3 blocks from the player

Efficient Terrain Rendering and Chunking - Nicholas Magarino:
Chunking of the terrain was done, essentially, by separating the terrain into
separate 16x256x16 collections of BlockTypes.  Instead of rendering several cubes,
the blocks per chunks are rendered per face, where only faces adjacent to empty
blocks are rendered.  By rendered, that means that the VBOs for the faces are only
set up for such faces, and thus only those faces are drawn.  Adjacent blocks
that are in different chunks are checked based on whether or not a chunk exists
adjacent to the block, and then checking the block at that adjacent world position
spot within the entire terrain.  
Within the Terrain class, chunks are stored with a mapping from a QPair<int, int> to
a Chunk*, where the QPair represent the x,z coordinates of an entire chunk.  The first
chunk has a corner at 0,0 and subsequent chunks are drawn from there.

Game Engine Update Function and Player Physics - Sangeun Lee:
Player Physics & update function

Velocity was a glm::vec3 to store the amount in each direction

Position was the camera’s eye.

Player’s camera is the MyGl’s camera.

The MouseEvent updates cursor’s screen x and y which rotates the camera when it is on the left/right side of the middle or top/bottom side of the middle of the screen.

The WASD, Space bar are stored in boolean values when they are released and held.

Timer update uses QDateTime::CurrentMSecsSinceEpoch() and finds the delta time every single update.

Player Physics had to scale the time update to get a reasonable number

The player’s physics/velocity reduces like friction/gravity as time updates. 

The key presses would increase the player’s velocity in a direction.

The block beneath the player supports the player if it is not empty. 

The player is grounded if the block beneath is solid. The player will fall if it is not solid. 

For collision: 
I tried to use future position forecasting to stop the player from moving anymore if the next step collides.

I tried to use each position’s collision at one time and move it back if the position collides with other things. 

I’m trying to use ray marching to see if the player’s vertices will ever collide on it’s course of move. 

MILESTONE 2 - 
L-System Rivers - Sangeun Lee:
I did the L-System, creating 2 rivers. 

The Terrain has 2 rivers in a stack. 
One is more linear and follows one pattern. It does not branch. It has been given a tendency to rotate to one side. 
Another is a more standard L system tree shape, but still randomized to have different widths and length branches so that it looks more natural. 
It has larger width and longer distance. 
Both rivers have lakes at the end of their branches. 
For the L-System, I used the common X -> S[-FX]+FX and F -> drawLine. So I got a string of expansion first then iterated through the QString to get each char->action. 
I made the River Class to represent the river which has a turtle, a map for character to string, and a map for character to function pointers. The function pointers were rather finicky so I just directly called the function needed to draw each point in the river. The Turtle Class is the marker for the L-system drawing. 

In Terrain, the terrain had to carve out the nearby land near the water. The height field of the terrain is quite high so there are sharp contrasts between the levels.  

Every time you create a terrain, it creates rivers. 

It yet does not continue rivers through the chunks nor creates each one unique to a chunk/section.

Player Collision detection with ray marching technique has been added from last week's code since it is working better. It might actually get stuck in the bottom of the feet some times. Player mouse function is now centered in the middle. 
Player physics has been altered a bit.

Texturing and Texture Animation - Alexis Ward:
I did the texturing and texture animation
	- Loads the image of all blocks as a texture for OpenGL
	- Changed the lambert shader to use this texture, animate if needed, and apply a Blinn-Phong specular           reflection (with intensities depending on the block’s “hardness” - grass and dirt the least reflective,           water and lava the most)
	- Changed MyGL, Terrain, and ShaderProgram to support these features (including making blocks transparent           based on their “a” value in their colors)


Swimming, Multithreaded Terrain Generation, and Sandbox Mode - Nicholas Magarino:
I implemented SandBox Mode by essentially changing functionality in MyGL based on a bool flag member variable in
MyGL.  This flag is turned true or false with the F key.  If true, MyGL will essentially skip the physics and 
collision testing done in player mode, and is thus unaffected by gravity or collisions.  The key press events
are also made different when the flag is true -- there is no jumping, and players can move up or down at a constant rate with E and Q.  
For swimming, a red or blue overlay is drawn on screen only if the block at the camera's eye (position) is a 
water or lava block.  The overlay is a Drawable quad rendered in NDC space, and placed extremely close to the 
near clip plane.  For swimming physics, a flag in the Player class is switched on if the block at the player's
feet (2 units below the camera position) is a water or lava block.  In swimming, the player's downward movement
decreases at a noticeably slower, constant rate.  The same holds true for vertical and horizontal movement --
the increment at which velocity increases when swimming is reduced.  Notably, I removed some friction calculations from the player movement methods in order to allow for slower, constant movement.  Holding space
allows the player to move up in the water at a constant rate, working similarly to moving up in Sandbox mode. 
AS AN IMPORTANT NOTE, the swimming physics seem to work differently on Macs than PCs.  When holding space bar,
on Macs, the player does move up, but in a much more notably jittery way.  When swimming forward, the player
speeds up extremely dramatically, instead of moving at a constant rate, for some reason.  I'm not sure why this
is the case yet.
For multithreading, I created a QRunnable "Chunker", which takes in a terrain, a chunk, and a mutex.  The chunker's run function will, for the given chunk, set the block types within it, and create VBO data for the chunk.   These runnables are passed to a QThreadPool, and once they've all run, each chunk has its VBO data
binded and passed to the GPU in an additional create function.  Such terrain generation, for now, only occurs
for newly generated terrain, and not the original test scene terrain.

MILESTONE 3 - 
Inventory and onscreen GUI - Nicholas Magarino:
I implemented an inventory screen that can be opened with the I key, which displays 9 block types and the amount
of each that the player is currently holding.  When the inventory is open, the user can use the arrow keys to
toggle between block types, highlighted by a yellow square outline.  When the player uses a certain block, the
amount of blocks of that type the player's holding will decrease, and the amount will increase as the player
destroys a block of a certain type.  The inventory is a quad drawn in NDC space with a UV texture applied to it. 
The yellow square selection is a square drawn with GL_LINES.  The numbers are separate drawable numbers that are
also drawn with a texture, and the texture applied is updated based on the number of blocks the player has for
each type.  
In addition, some other fixes were done for previous milestone issues.  Rendering blocks behind transparent blocks was fixed by doing two render passes on the entire terrain, one to only draw opaque blocks, the another
to draw transparent ones.  Also, multithreading was updated to use two types of QRunnables, one that sets block
types for a chunk, and another that created the VBOs for a chunk.  Still, the multithreading doesn't speed up the chunking process significantly.  Lastly, the VBOs were made as interleaved VBOs, containing the sequence
(position, normal, UV, "block").  "Block" is a value used for texture animation and reflectivity.
There are still some issues where some faces along chunk edges don't update when chunks are added adjacently.

Caves, Procedural Sky - Sangeun Lee:


River
	Has been extended to create a long/large expanded river.
	Fixing of negative direction chunking has allowed more natural river formation. 
	After creating river blocks positions, create the chunks that contain the river. After that create the water blocks in the positions. Then carve out from the terrain. 

	Two rivers are now present. One linear, and another delta. 
Caves
	L-System turtle walk used. It is similar to the River. It uses random generator to determine what the ceiling height is. It is flooded with LAVA when it reaches a certain depth. The Caves also are littered with Coal blocks randomly using random generator. 
	
Sky
	Working off of the base sunset code, The sun was removed. I tried to create another planet but it did not work out since either one would cover up the other. So I settled for one planet. The planet was played around with having different noise colors. I decided the circular shape was boring so I noise remade the boundaries of the planet. This made the planet more of a blob. The noise was made with fbm (uv * uTime), around 3 to 4 times. 



AI assessment task

== Building & Running ==
Build using Visual Studio.

`SDL.dll' must be in the program's working directory.
It should be copied there as a post-build command, but
if that doens't work then it must be done manually.

The `res' folder must also be in the program's working
directory. This will be next to the executable when running
outside of Visual Studio.

== Play Instructions ==
The menus can be navigated using the up and down arrow
keys. Menu items can be selected using the return key.

The character can be controlled using the arrow keys.
The goal of the game is to take the key and escape through
the door without being caught by any enemies.

 == Review ==

Implementing the path finding algorithm was rather difficult,
as I haven't worked with path finding very much in the past,
so a lot of the terminology was quite alien to me; Thus, it
took quite a bit of reading before I was able to understand
the concepts enough to start implementing the A* algorithm.

The technical design document helped to make things go more
smoothly by allowing me to break up tasks more effectively.

The performance of A* is fairly slow, especially with a large
number of nodes. This isn't a big deal with something a small
map, but it could become an issue if there's many agents on
a large map with many nodes.

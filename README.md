# Back-to-black-threaded

Gives result of a simulated back-to-black board game, with all the pieces flipped in parallel. 
The bits work as the pieces on the board, and each thread flips multiples of a different number.
The results are the numbers flipped to black.
<br>
<br>

## Compile & run

Compile the application using the following command:

    $ gcc -Wall -o flip flip.c -lpthread

Run the application using the following command:

    $ ./flip

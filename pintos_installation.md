# "pintos" HELPER script
    * "pintos --qemu" -- -h (pintos by default uses box)
        * "pintos --qemu -- <os command>"
    * pintos-gdb // pintos version of gdb            
    * "--" is delimiter for pintos helper script flags (to the left of delim.), actual pintos flags (to the right of delim.)

# Project 2: Userprog setup (filesystem)
    * make // generates build file where output of code is
    * "pintos-mkdisk filesys.dsk --filesys-size=2" // set HDD to 2MB 
    * pintos --qemu -- -f -q // tell pintos to format HDD in a way it can recognize, so must bootup pintos 
        * "-f" // formats the HDD, pass this when running tests so it starts from scratch
        * "-q" // quits pintos window
    * "make check" // runs all tests (DO THIS WITHIN BUILD DIRECTORY) 
    * pintos --qemu -- <test name> // runs the singular test by name of test

# Pintos GDB
    * Window 1 (Pintos): 
        * "pintos --qemu --gdb -- run args-none" // halts qemu, waiting for GDB to start
    * Window 2 (GDB):
        * pintos-gdb kernel.o // Tell GDB of the contents of the code through the binary
        * "pintos-gdb"         
        * target remote localhost:1234 // connect GDB to pintos
        * TODO: learn how to use GDB

* Make sure you're in the same directory as the install script when sourcing

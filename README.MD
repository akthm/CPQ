CPL language simplified compiler 


This was built and tested on wsl 2 with : 
    Bison version 3.8.2
    flex version 2.6.4
using the make file run: 
    make 
    to compile cpq executable.
Usage: 
    ./cpq <source-file>.ou


Design:

    - For better performance i've built a string search map,
        designed especially for the implementation of this 
        compiler, (see stringHasher, synMapper, labelMapper, idMapper).
        in theory it's speed is **linear** to input id (string ) length, but 
        since it is bounded(see defines in utils.h), speed is constant.

    - for Id type and initialization testing, i've used synMapper
        version of the hasher built **Globally** for ease of 
        access and free in all code parts, along side the speed 
        of global data structure is better, so i've opted for
        that rather than passing it between rules.

    * also made global is a linked list NodeAdder, used to chain
        id's from declaration rule to later be added to our 
        tree data structure.

    -  id , label and tmp params sizes are bound for faster and 
        easier implementation, note they are defined in utils.h 
        for quick adjustment of sizes.
    
    - for binary operators in bison file, i've opted for a large 
        factory for all calculations

    - newlabel, newtemp, gen, functions from homework are implemented
        in utils.
    
    - I didn't implement switch, case rules, But i did direct 
        jump values to lines using final screening and a temp middle
        file that is removed at the end of execution, in utils.

        -- During this screening, legal id's containing capital letters are replaced with temps
            in the final QUAD code.

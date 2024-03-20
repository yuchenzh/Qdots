# Introducing Qdots

Qdots is a function object that can write out the heat realease rate, similar to Qdot, without overriding the previous solutions when restarting, in OpenFOAM-10.


# Compile
```bash
    wmake
```

# Usage
```C++
    Qdots
    {
        type Qdots;
        libs ("libQdots.so");
        writeControl writeTime;
    }
```

## Case example
Execute
```bash
    Allrun
```
You will find that the Qdot in 20 folder is overrided with a all zero file, but the Qdots survives.

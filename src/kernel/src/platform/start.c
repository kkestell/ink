extern void NimMain();

// crt0.s jumps here after saving boot info to r15
void
start()
{
    NimMain();
}
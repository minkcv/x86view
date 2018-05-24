
void kernel_main()
{
    char* vidmem = (char*)0xB8000;
    *vidmem = 0xF << 4;
    *vidmem = 'X';
}

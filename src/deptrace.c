/* sovereign pure stub: ptrace always returns 0 */
long ptrace(int request, int pid, int addr, int data)
{
    return 0;
}

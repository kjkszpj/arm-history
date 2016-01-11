//
// Created by Peijie You on 16/1/11.
//

int main()
{
    int i;
    while (1)
    {
        asm volatile("SVC 0" : : : );
        volatile int a = 0;
        for (i = 0; i < 3000000; i++) i += a;
    }
}
#include<stdlib.h>
#include<stdio.h>
#include<dos.h>

void motor_on()
{
   __asm__(
   "movw $0x3f2,%dx\n
    movb $0x14,%al\n
    out %al,%dx\n");
}
void motor_off()
{
   __asm__(
   "movw $0x3f2,%dx\n
    movb $0x04,%al\n
    out %al,%dx\n");
}

void main()
{
   motor_on();
   delay(6000);
   motor_off();
}

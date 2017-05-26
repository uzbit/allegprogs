DOSSEG
    .MODEL SMALL
    .STACK 200h
    .DATA

Message     db  "e:\programmin\asm\Word.up$"
                  
    .CODE
    
START:

    lea     dx,Message
    mov     ah,3Ch
    mov     cx,2
    int     21h         
                        

    mov     ax,4c00h    
    int     21h         

END START

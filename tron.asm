syscall:
    beq $v0, $zero, main
    addi $k1, $zero, 10
    beq $v0, $k1, syscall10
    addi $k1, $zero, 12
    beq $v0, $k1, syscall12
    addi $k1, $zero, 18
    beq $v0, $k1, syscall18
    

syscall10:
    j syscall10


syscall12:
    #addi $t0, $zero, 16  #0x0010
    lw $v0, 16($zero)
    beq $v0, $zero, else12  #if 0 then no key is pressed
    #addi $t0, $zero, 20  #0x0014
    lw $v0, 20($zero) #get the value of the keypress
    #addi $t0, $zero, 16
    #addi $t1, $zero, 0
    sw $zero, 16($zero)  #increment to the next thing into keyboard 
    
    j end12

    else12:
        addi $v0, $zero, 0
    end12:
        jr $k0

syscall18:
    #addi $t0, $zero, 32  #0x0020
    sw $a0, 32($zero)
    #addi $t0, $zero, 36  #0x0024
    sw $a1, 36($t0)
    #addi $t0, $zero, 40  #0x0028
    sw $a2, 40($t0)
    #addi $t0, $zero, 44  #0x002c
    addi $k1, $zero, 1
    sw $k1, 44($t0)

    jr $k0

main:
# this is the Game of Tron
    #allocate 2d array appropriate size (64x64) 
    addi $t0, $zero, -4096  #(128x128)
    sll $t0, $t0, 2  #int is 4 bytes 
    add $sp, $sp, $t0  #allocate memory on the stack for 2d array


    addi $s0, $zero, 1  #s0 = xdirection
    addi $s1, $zero, 0  #s1 = ydirection

    addi $a0, $s2, 0
    addi $a1, $s3, 0
    addi $a2, $s4, 255
    
    addi $v0, $zero, 18
    syscall

startloop:
    addi $v0, $zero, 12
    syscall

    beq $v0, $zero, startloop  #no key press
    addi $t0, $zero, 119      #"w" in ascii = 119
    beq $v0, $t0, wpress
    addi $t0, $zero, 97     #"a" in ascii = 97
    beq $v0, $t0, apress
    addi $t0, $zero, 115   #"s" in ascii = 115
    beq $v0, $t0, spress
    addi $t0, $zero, 100    #"d" in ascii = 100
    beq $v0, $t0, dpress
wpress:
    addi $s0, $zero, 0
    addi $s1, $zero, -1

    j endpress

apress:
    addi $s0, $zero, -1
    addi $s1, $zero, 0


    j endpress
spress:
    addi $s0, $zero, 0
    addi $s1, $zero, 1

    j endpress
dpress:
    addi $s0, $zero, 1
    addi $s1, $zero, 0

    j endpress


endpress:
    add $s2, $s2, $s0  #a0 is the xcoord
    add $s3, $s3, $s1   #a1 is the ycoord 
    addi $s4, $zero, 255  #blue



    #check array to see if the new xcoordinate,ycoordinate is already visited
    addi $t0, $s2, 0  #i = xcoord
    addi $t1, $s3, 1  #j = ycoord
    #addi $t1, $t1, 1  #this will make j 1-indexed
    addi $t2, $zero, 64  #num of columns 

    #i * m + j
    mult $t0, $t2       # i * m
    mflo $t0 
    add $t0, $t0, $t1   # (i * m) + j
    addi $t3, $zero, 1
    sub $t0, $t0, $t3
    # $t0 now stores the index corresponding to [xcord][ycoord]

    sll $t0, $t0, 2  # int is 4 bytes 
    add $t0, $sp, $t0   #above is to add the offset to addr 
    lw $t2, 0($t0)  #load the value [xcord][ycoord] to $t2 
    addi $t3, $zero, 1  #t3 = 1
    beq $t2, $t3, endloop  #if t2 = 1, then it's already been visited #if so, jump to ‘‘ending screen’’
    #not visited, we draw on the monitor
    
    sw $t3, 0($t0)  #mark that this place is now visited


    addi $a0, $s2, 0
    addi $a1, $s3, 0
    addi $a2, $s4, 0
    
    addi $v0, $zero, 18
    syscall

    j startloop

endloop:
    #end game
    #ending screen goes here
    addi $v0, $zero, 10
    syscall 

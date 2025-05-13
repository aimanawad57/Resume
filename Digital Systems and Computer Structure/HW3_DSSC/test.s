.text
main:   # Put your code here
		lw      t2, 8(x0)
		addi    t3, t2, 12
		sw      t3, 16(x0)

        add		t6, x0, x0
        beq		t6, x0, finish

deadend: beq	t6, x0, deadend        

finish:
        lw		t4, 0(x0)
        lw		t5, 4(x0)
        sw		t5, 0xFF(t4)
        beq		t6, x0, deadend



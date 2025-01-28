
	.arm
	.align 2

	.global	initLib
	.type	initLib,	%function
initLib:
	ldr	r2, =saved_stack
	str	sp, [r2]
	str	lr, [r2,#4]
	ldr r2, =g_temporaryInitLibStack
	ldr r2, [r2]
	cmp r2, #0
	movne sp, r2
	bl	__system_allocateHeaps
	bl	__libc_init_array
	ldr	r2, =saved_stack
	ldr sp, [r2]
	ldr	lr, [r2,#4]
 	bx	lr


	.global	__ctru_exit
	.type	__ctru_exit,	%function

__ctru_exit:
	ldr	r2, =saved_stack
	str	sp, [r2]
	str	lr, [r2,#4]
	bl	__appExit

	@ldr	r2, =saved_stack
	@ldr	sp, [r2]
	bl	__libctru_exit
	bl	__libc_fini_array
	ldr	r2, =saved_stack
	ldr sp, [r2]
	ldr	lr, [r2,#4]
 	bx	lr

	.bss
	.align 2
saved_stack:
	.space 8

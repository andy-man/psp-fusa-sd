	.set noreorder
	.set noat

############################################################

# Try and do a fast copy with the cpu
	.global copy16to16_1024f2
	.ent	copy16to16_1024f2

copy16to16_1024f2:
	lui	$a2, 0x4000
	or	$a1, $a1, $a2
	li	$a2, 0
	li	$t1, 217
        li	$t2, 393216
	ori	$t2, $t2, 0x2400

3:	
	li	$t7, 4
	addiu	$a0, $a0, (1024*2)
        
1:
# 480 * 2 / 64
	addiu	$a1, $a1, (96*2)
	li	$t0, (480/10)
2:
	addu	$a2, $a1, $t2
######################################################################
	lw	$t3, 0($a0)
	lw	$t4, 4($a0)
	lw	$t5, 8($a0)
###
	sh	$t3, 0($a1)
	sh	$t3, 0($a2)

	srl	$t3, $t3, 16

	sh	$t3, 2($a1)
	sh	$t3, 4($a1)

	sh	$t3, 2($a2)
	sh	$t3, 4($a2)
###
	sh	$t4, 6($a1)
	sh	$t4, 6($a2)
	srl	$t4, $t4, 16
	sh	$t4, 8($a1)
	sh	$t4, 8($a2)
###
	sh	$t5, 10($a1)
	sh	$t5, 12($a1)

	sh	$t5, 10($a2)
	sh	$t5, 12($a2)

	srl	$t5, $t5, 16

	sh	$t5, 14($a1)
	sh	$t5, 14($a2)

######
	lw	$t3, 12($a0)
	lw	$t4, 16($a0)
######
	sh	$t3, 16($a1)
	sh	$t3, 18($a1)

	sh	$t3, 16($a2)
	sh	$t3, 18($a2)

	srl	$t3, $t3, 16

	sh	$t3, 20($a1)
	sh	$t3, 20($a2)
###
	sh	$t4, 22($a1)
	sh	$t4, 22($a2)

	srl	$t4, $t4, 16

	sh	$t4, 24($a1)
	sh	$t4, 26($a1)

	sh	$t4, 24($a2)
	sh	$t4, 26($a2)
######################################################################
	addiu	$t0, $t0, -1
	addiu	$a0, $a0, 20
	bnez	$t0, 2b

# horizontal loop 480
	addiu	$a1, $a1, 28

	addiu	$t1, $t1, -1
	addiu	$t7, $t7, -1

	beqz	$t7, 3b # skip line
	addiu	$a0, $a0, (544*2)

	bnez	$t1, 1b
# vertical loop 272
	nop

	jr		$ra
	nop

	.end 	copy16to16_1024f2

############################################################

# Try and do a fast copy with the cpu
	.global copy16to16f2
	.ent	copy16to16f2

copy16to16f2:
	lui	$a2, 0x4000
	or	$a1, $a1, $a2
	li	$a2, 0
	li	$t1, 217
        li	$t2, 393216
	ori	$t2, $t2, 0x2400

3:	
	li	$t7, 4
	addiu	$a0, $a0, (512*2)
        
1:
# 480 * 2 / 64
	addiu	$a1, $a1, (96*2)
	li	$t0, (480/10)
2:
	addu	$a2, $a1, $t2
######################################################################
	lw	$t3, 0($a0)
	lw	$t4, 4($a0)
	lw	$t5, 8($a0)
###
	sh	$t3, 0($a1)
	sh	$t3, 0($a2)

	srl	$t3, $t3, 16

	sh	$t3, 2($a1)
	sh	$t3, 4($a1)

	sh	$t3, 2($a2)
	sh	$t3, 4($a2)
###
	sh	$t4, 6($a1)
	sh	$t4, 6($a2)
	srl	$t4, $t4, 16
	sh	$t4, 8($a1)
	sh	$t4, 8($a2)
###
	sh	$t5, 10($a1)
	sh	$t5, 12($a1)

	sh	$t5, 10($a2)
	sh	$t5, 12($a2)

	srl	$t5, $t5, 16

	sh	$t5, 14($a1)
	sh	$t5, 14($a2)

######
	lw	$t3, 12($a0)
	lw	$t4, 16($a0)
######
	sh	$t3, 16($a1)
	sh	$t3, 18($a1)

	sh	$t3, 16($a2)
	sh	$t3, 18($a2)

	srl	$t3, $t3, 16

	sh	$t3, 20($a1)
	sh	$t3, 20($a2)
###
	sh	$t4, 22($a1)
	sh	$t4, 22($a2)

	srl	$t4, $t4, 16

	sh	$t4, 24($a1)
	sh	$t4, 26($a1)

	sh	$t4, 24($a2)
	sh	$t4, 26($a2)
######################################################################
	addiu	$t0, $t0, -1
	addiu	$a0, $a0, 20
	bnez	$t0, 2b

# horizontal loop 480
	addiu	$a1, $a1, 28

	addiu	$t1, $t1, -1
	addiu	$t7, $t7, -1

	beqz	$t7, 3b # skip line
	addiu	$a0, $a0, (32*2)

	bnez	$t1, 1b
# vertical loop 272
	nop

	jr		$ra
	nop

	.end 	copy16to16f2

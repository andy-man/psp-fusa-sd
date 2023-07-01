
	.set noreorder
	.set noat

############################################################

# Copy 32bit colour buffer to 16bit colour
# void copy32to16(unsigned int *in, unsigned short *out)
	.global copy32to16f1
	.ent	copy32to16f1

copy32to16f1:
# Each line is 480 * 4 / 32 units
# Need to increment 32 * 4 after

#	lui	$a2, 0x4000
#	or	$a1, $a1, $a2
	li	$t1, 217
        li	$t2,393216
	ori	$t2,$t2,0x2400
#	cache	0x1e, 0($a0)  # fill cache line

3:	li	$t3, 4
	addiu	$a0, $a0, (512*4)

1:
	addiu	$a1, $a1, (288*2)
	li	$t0, (480/16)
2:
	cache	0x1e, 0($a0)			# fill cache line
	lv.q	C000, 0($a0)
	lv.q	C010, 16($a0)
	lv.q	C020, 32($a0)
	lv.q	C030, 48($a0)
	vt5650.q C100, C000
	vt5650.q C102, C010
	vt5650.q C110, C020
	vt5650.q C112, C030
#	cache	0x1e, 0($a0)  # fill cache line
	sv.q	C100, 0($a1)
	sv.q	C110, 16($a1)
	vnop		        # VFPU BUG Patch
	addu	$a1, $t2
	sv.q	C100, 0($a1)
	sv.q	C110, 16($a1)
	vnop		        # VFPU BUG Patch
	sub	$a1, $t2

	addiu	$a0, $a0, 64
	addiu	$t0, $t0, -1
	bnez	$t0, 2b
########
	addiu	$a1, $a1, 32

	addiu	$t1, $t1, -1
	addiu	$t3, $t3, -1

	beqz	$t3, 3b # skip line
	addiu	$a0, $a0, (32*4)

	bnez	$t1, 1b
########
	nop

	jr		$ra
	nop

	.end	copy32to16f1

############################################################

	.global copy16to16_1024f1
	.ent	copy16to16_1024f1

copy16to16_1024f1:
	lui	$a2, 0x4000
	or	$a1, $a1, $a2
	li	$t1, 217
        li	$t2, 0x60000
	ori	$t2, $t2,0x2400
#	cache	0x1e, 0($a0)  # fill cache line

3:	
	li	$t7, 4
	addiu	$a0, $a0, (1024*2)

1:
# 480 * 2 / 64
	addiu	$a1, $a1, (288*2)
	li	$t0, (480/32)
2:
	cache	0x1e, 0($a0)			# fill cache line
  	lv.q	C000,  0($a0)			# C000 =  0($a0)
	lv.q	C010, 16($a0)			# C010 = 16($a0)
	lv.q	C020, 32($a0)			# C020 = 32($a0)
	lv.q	C030, 48($a0)			# C030 = 48($a0)
#	cache	0x1e, 64($a0)			# fill cache line
	addiu	$t0, $t0, -1
	sv.q	C000,  0($a1), wb		#  0($a1) = C000
	sv.q	C010, 16($a1), wb		# 16($a1) = C010
	sv.q	C020, 32($a1), wb		# 32($a1) = C020
	sv.q	C030, 48($a1), wb		# 48($a1) = C030
	vnop		        # VFPU BUG Patch
        addu	$a1, $t2
	sv.q	C000,  0($a1), wb		#  0($a1) = C000
	sv.q	C010, 16($a1), wb		# 16($a1) = C010
	sv.q	C020, 32($a1), wb		# 32($a1) = C020
	sv.q	C030, 48($a1), wb		# 48($a1) = C030
	vnop		        # VFPU BUG Patch
	sub	$a1, $a1, $t2
	vnop
	addiu	$a0, $a0, 64
	bnez	$t0, 2b

# horizontal loop 480
	addiu	$a1, $a1, 64

	addiu	$t1, $t1, -1
	addiu	$t7, $t7, -1

	beqz	$t7, 3b # skip line
	addiu	$a0, $a0, (544*2)

	bnez	$t1, 1b
# vertical loop 272
	nop

	jr		$ra
	nop

	.end 	copy16to16_1024f1

############################################################

# Try and do a fast copy with the cpu
	.global copy16to16f1
	.ent	copy16to16f1

copy16to16f1:
#	lui	$a2, 0x4000
#	or	$a1, $a1, $a2
	li	$t1, 217
        li	$t2, 393216
	ori	$t2, $t2, 0x2400
#	cache	0x1e, 0($a0)  # fill cache line

3:	
	li	$t7, 4
	addiu	$a0, $a0, (512*2)
        
1:
# 480 * 2 / 64
	addiu	$a1, $a1, (288*2)
	li	$t0, (480/32)
2:
	cache	0x1e, 0($a0)			# fill cache line
  	lv.q	C000,  0($a0)			# C000 =  0($a0)
	lv.q	C010, 16($a0)			# C010 = 16($a0)
	lv.q	C020, 32($a0)			# C020 = 32($a0)
	lv.q	C030, 48($a0)			# C030 = 48($a0)
#	cache	0x1e, 64($a0)			# fill cache line
	addiu	$t0, $t0, -1
	sv.q	C000,  0($a1), wb		#  0($a1) = C000
	sv.q	C010, 16($a1), wb		# 16($a1) = C010
	sv.q	C020, 32($a1), wb		# 32($a1) = C020
	sv.q	C030, 48($a1), wb		# 48($a1) = C030
	vnop		        # VFPU BUG Patch
        addu	$a1, $t2
	sv.q	C000,  0($a1), wb		#  0($a1) = C000
	sv.q	C010, 16($a1), wb		# 16($a1) = C010
	sv.q	C020, 32($a1), wb		# 32($a1) = C020
	sv.q	C030, 48($a1), wb		# 48($a1) = C030
	vnop		        # VFPU BUG Patch

	sub	$a1, $a1, $t2
	vnop
	addiu	$a0, $a0, 64
	bnez	$t0, 2b

# horizontal loop 480
	addiu	$a1, $a1, 64

	addiu	$t1, $t1, -1
	addiu	$t7, $t7, -1

	beqz	$t7, 3b # skip line
	addiu	$a0, $a0, (32*2)

	bnez	$t1, 1b
# vertical loop 272
	nop

	jr		$ra
	nop

	.end 	copy16to16f1

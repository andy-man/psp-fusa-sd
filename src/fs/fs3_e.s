# Copy 32bit colour buffer to 16bit colour
# void copy32to16(unsigned int *in1, unsigned int *in2, unsigned short *out1, unsigned short *out2)
	.global copy32to16f3
	.ent	copy32to16f3
################################################################################
copy32to16f3:
# Each line is 480 * 4 / 32 units
# Need to increment 32 * 4 after

#	a0 - frm buf src
#	a1 - frm buf dest
#	a2 - frm buf src + 1 line
#	a3 - ifrm buf dest
	li	$t1, 136
	li	$a2, (512*4)   # +1line
	addu	$a2, $a2, $a0
	li	$a3, (263*2*768) # ifrm buf
	addu	$a3, $a3, $a1

1:
	addiu	$a1, $a1, (288*2)  # frmbuf > skip unused pxls
	addiu	$a3, $a3, (288*2)  # ifrmbuf > skip unused pxls
	li	$t0, (480/16)	   # 16px for every step

2:
# frmbuf
	cache	0x1e, 0($a0)  # fill cache line
	lv.q	C000, 0($a0)
	lv.q	C010, 16($a0)
	lv.q	C020, 32($a0)
	lv.q	C030, 48($a0)
	vt5650.q C100, C000
	vt5650.q C102, C010
	vt5650.q C110, C020
	vt5650.q C112, C030
	sv.q	C100, 0($a1)
	sv.q	C110, 16($a1)
	vnop		        # VFPU BUG Patch

	addiu	$a0, $a0, 64
	addiu	$a1, $a1, 32

# ifrmbuf

	cache	0x1e, 0($a2)  # fill cache line
	lv.q	C000, 0($a2)
	lv.q	C010, 16($a2)
	lv.q	C020, 32($a2)
	lv.q	C030, 48($a2)
	vt5650.q C100, C000
	vt5650.q C102, C010
	vt5650.q C110, C020
	vt5650.q C112, C030
	sv.q	C100, 0($a3)
	sv.q	C110, 16($a3)

	addiu	$t0, $t0, -1

	addiu	$a2, $a2, 64
	addiu	$a3, $a3, 32

	bnez	$t0, 2b
	vnop		        # VFPU BUG Patch


	addiu	$t1, $t1, -1

	addiu	$a0, $a0, (544*4)
	addiu	$a2, $a2, (544*4)

	bnez	$t1, 1b
	nop

	jr		$ra
	nop

	.end	copy32to16f3
################################################################################
	.global copy16to16_1024f3
	.ent	copy16to16_1024f3
copy16to16_1024f3:
#	lui	$a2, 0x4000
#	or	$a1, $a1, $a2
	li	$t1, 136
	li	$a2, (1024*2)   # +1line
	addu	$a2, $a2, $a0
	li	$a3, (263*2*768) # ifrm buf
	addu	$a3, $a3, $a1

1:
	addiu	$a1, $a1, (288*2)  # frmbuf > skip unused pxls
	addiu	$a3, $a3, (288*2)  # ifrmbuf > skip unused pxls
	li	$t0, (480/32)	   # 32px for every step
2:
# frmbfr
	cache	0x1e, 0($a0)  # fill cache line
  	lv.q	C000,  0($a0)			# C000 =  0($a0)
	lv.q	C010, 16($a0)			# C010 = 16($a0)
	lv.q	C020, 32($a0)			# C020 = 32($a0)
	lv.q	C030, 48($a0)			# C030 = 48($a0)

	sv.q	C000,  0($a1), wb		#  0($a1) = C000
	sv.q	C010, 16($a1), wb		# 16($a1) = C010
	sv.q	C020, 32($a1), wb		# 32($a1) = C020
	sv.q	C030, 48($a1), wb		# 48($a1) = C030
	vnop		        # VFPU BUG Patch

	addiu	$a0, $a0, 64
	addiu	$a1, $a1, 64

# ifrmbfr

	cache	0x1e, 0($a2)  # fill cache line
  	lv.q	C000,  0($a2)			# C000 =  0($a0)
	lv.q	C010, 16($a2)			# C010 = 16($a0)
	lv.q	C020, 32($a2)			# C020 = 32($a0)
	lv.q	C030, 48($a2)			# C030 = 48($a0)

	sv.q	C000,  0($a3), wb		#  0($a1) = C000
	sv.q	C010, 16($a3), wb		# 16($a1) = C010
	sv.q	C020, 32($a3), wb		# 32($a1) = C020
	sv.q	C030, 48($a3), wb		# 48($a1) = C030

	addiu	$t0, $t0, -1

	addiu	$a2, $a2, 64
	addiu	$a3, $a3, 64

	bnez	$t0, 2b
	vnop		        # VFPU BUG Patch

# horizontal loop 480

	addiu	$t1, $t1, -1

	addiu	$a0, $a0, (1568*2)
	addiu	$a2, $a2, (1568*2)
	bnez	$t1, 1b
# vertical loop 272
	nop

	jr		$ra
	nop

	.end 	copy16to16_1024f3

################################################################################
# Try and do a fast copy with the cpu
	.global copy16to16f3
	.ent	copy16to16f3

copy16to16f3:
#	lui	$a2, 0x4000
#	or	$a1, $a1, $a2
	li	$t1, 136
	li	$a2, (512*2)   # +1line
	addu	$a2, $a2, $a0
	li	$a3, (263*2*768) # ifrm buf
	addu	$a3, $a3, $a1

1:
	addiu	$a1, $a1, (288*2)  # frmbuf > skip unused pxls
	addiu	$a3, $a3, (288*2)  # ifrmbuf > skip unused pxls
	li	$t0, (480/32)	   # 32px for every step
2:
# frmbfr
	cache	0x1e, 0($a0)  # fill cache line
  	lv.q	C000,  0($a0)			# C000 =  0($a0)
	lv.q	C010, 16($a0)			# C010 = 16($a0)
	lv.q	C020, 32($a0)			# C020 = 32($a0)
	lv.q	C030, 48($a0)			# C030 = 48($a0)

	sv.q	C000,  0($a1), wb		#  0($a1) = C000
	sv.q	C010, 16($a1), wb		# 16($a1) = C010
	sv.q	C020, 32($a1), wb		# 32($a1) = C020
	sv.q	C030, 48($a1), wb		# 48($a1) = C030
	vnop		        # VFPU BUG Patch

	addiu	$a0, $a0, 64
	addiu	$a1, $a1, 64

# ifrmbfr

	cache	0x1e, 0($a2)  # fill cache line
  	lv.q	C000,  0($a2)			# C000 =  0($a0)
	lv.q	C010, 16($a2)			# C010 = 16($a0)
	lv.q	C020, 32($a2)			# C020 = 32($a0)
	lv.q	C030, 48($a2)			# C030 = 48($a0)

	sv.q	C000,  0($a3), wb		#  0($a1) = C000
	sv.q	C010, 16($a3), wb		# 16($a1) = C010
	sv.q	C020, 32($a3), wb		# 32($a1) = C020
	sv.q	C030, 48($a3), wb		# 48($a1) = C030


	addiu	$a2, $a2, 64
	addiu	$a3, $a3, 64

	addiu	$t0, $t0, -1

	bnez	$t0, 2b
	vnop		        # VFPU BUG Patch

# horizontal loop 480

	addiu	$t1, $t1, -1
	addiu	$a2, $a2, (544*2)
	addiu	$a0, $a0, (544*2)
	bnez	$t1, 1b
# vertical loop 272
	nop

	jr		$ra
	nop

	.end 	copy16to16f3

	.set noreorder
	.set noat

	.global setFrmBufInternal
	.ent    setFrmBufInternal

setFrmBufInternal:
	nop # reserved
	nop # reserved
	j 0x00000000 # reserved :)
	nop

	.end setFrmBufInternal

	.global SetModeF
	.ent    SetModeF

SetModeF:
	nop # reserved
	nop # reserved
	j 0x00000000 # reserved :)
	nop

	.end SetModeF

	.global stuba
	.ent    stuba

stuba:
	nop # 0 --- start
	nop
	nop
	nop
	nop
	nop
	nop
	nop # 7 --- SPB
	nop
	nop # 9 -- Vcount
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	.end stuba

.text
	
.global	io_hlt
.global key_int

io_hlt:
	hlt
	ret
key_int:
	int $33
	ret

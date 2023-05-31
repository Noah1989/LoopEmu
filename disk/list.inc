; list command (argc b, *argv[] de)
; (ix points to standard i/o device)

list	jr	list0
	db	'list',0
list0	rst	error ; TODO


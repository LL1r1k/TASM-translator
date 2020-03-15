dec ax
dec bx
dec al
dec BYTE PTR [bx]
dec WORD PTR [bx]
dec WORD PTR [bx + 5h]
dec WORD PTR [bx + 5]
dec WORD PTR [bx + 0101b]
dec WORD PTR [bx + 00000101b]
dec WORD PTR [bx + 10000101b]
dec WORD PTR [bx + 1010000101b]
dec WORD PTR [bx + 540]
dec WORD PTR [bx + 0effh]
dec WORD PTR [bp]
dec WORD PTR [bp + 2h]
dec WORD PTR [bx + b2h]
	  
xchg ax, dx
xchg dx, ax
xchg dx, bx
xchg bl, dl
xchg ax, [bp]
xchg al, [bp]
xchg ax, [bp + 2h]
xchg ax, [bx]
xchg ax, [bx + 0b020h]
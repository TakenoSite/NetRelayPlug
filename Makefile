
C		:= gcc 
FUNCFLAG	:= -DRUNITSESSION -DRUNITKEEPALIVE 
PRINTFLAG	:= -DD_LOG -DE_LOG  -DLOG -DDEBUG -DAUTH_LOG -DSHOWTABLE -DSHOWCONFIG -DDLOG  
CFALG		:= $(FUNCFLAG) $(PRINTFLAG) -O2 -Wall 

CFILE := *.c 
LIB   := *.a
main:
	$(C) $(CFILE) $(LIB) -o $@ $(CFALG)


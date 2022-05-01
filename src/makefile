CC = gcc-11
EXE_PATH = ../bin
CFLAG = -lgfortran -L../lib -lsofa_c_20210512 -ltesteph
EXE_TAR = SolarTerms


OBJ = jd2cst.o main.o sun2earth.o TermsCheck.o utc2tdb.o

$(EXE_PATH)/$(EXE_TAR) : $(OBJ)        
	@-mkdir -p $(EXE_PATH)
	$(CC) -o $@ $^ $(CFLAG)

$(OBJ) : solarterms.h

%.o : %.c
	$(CC) -o $@ -c $<
	

.PHONY :  clean cleanall
clean :
	-rm -rf *.o
cleanall : 
	-rm -rf *.o $(EXE_PATH)
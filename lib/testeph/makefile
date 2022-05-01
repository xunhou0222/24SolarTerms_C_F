lib_tar=libtesteph.a
FC=gfortran-11

$(lib_tar) : eph.o split.o
	ar -rc ../$@ $^

eph.o : eph.f
	$(FC) -o $@ -c $<
split.o : split.f
	$(FC) -o $@ -c $<


.PHONY : clean
clean :
	-rm *.o
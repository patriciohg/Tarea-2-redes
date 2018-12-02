SRC2=src

default: all

all:
	@cd $(SRC2) && make

clean:

	@cd $(SRC2) && make clean
	@rm -rf *~

distclean: clean
	@cd $(SRC2) && make distclean

.PHONY: all clean distclean

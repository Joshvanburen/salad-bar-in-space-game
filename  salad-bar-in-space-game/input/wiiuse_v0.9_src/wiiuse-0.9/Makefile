# Minimal Makefile
#
# By: Michael Laforest <paralizer -AT- users -DOT- sourceforge -DOT- net>
#
# $Header$

#
# Change this to your GCC version.
#
CC = gcc
CCX = g++

####################################################
#
# You should not need to edit below this line.
#
####################################################

###############################
#
# Build targets.
#
###############################

Release:
	@cd src/ && make
	@cd api/ && make
	@cd gfx/ && make

	@mv src/wiiuse.so .
	@mv api/wiiuse-example .
	@mv gfx/wiiuse-gfx .

debug:
	@cd src/ && make debug
	@cd api/ && make debug
	@cd gfx/ && make debug

	@mv src/wiiuse.so .
	@mv api/wiiuse-example .
	@mv gfx/wiiuse-gfx .

clean:
	@cd src/ && make clean
	@cd api/ && make clean
	@cd gfx/ && make clean

distclean:
	@cd src/ && make distclean
	@cd api/ && make distclean
	@cd gfx/ && make distclean

	@rm wiiuse.so wiiuse-example wiiuse-gfx &> /dev/null
	@find . -type f \( -name "*.save" -or -name "*~" \) -delete &> /dev/null

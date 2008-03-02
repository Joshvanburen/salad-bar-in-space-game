#
#	wiiuse Makefile
#

####################################################
#
# You should not need to edit below this line.
#
####################################################

all clean install:
	@$(MAKE) -C src $@
	@$(MAKE) -C example $@
	@$(MAKE) -C example-sdl $@

distclean:
	@$(MAKE) -C src $@
	@$(MAKE) -C example $@
	@$(MAKE) -C example-sdl $@
	@find . -type f \( -name "*.save" -or -name "*~" -or -name gmon.out \) -delete &> /dev/null


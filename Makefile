all:
ifeq ($(PLATFORM), LINUX)
	@make --no-print-directory -f linux.mk
else
	@echo "Specify a platform (make PLATFORM=<platform>)"
endif

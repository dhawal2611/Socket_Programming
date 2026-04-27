# List of subdirectories containing their own Makefiles
SUBDIRS = client server

# Default target: loops through each subdir and runs 'make'
all: $(SUBDIRS)

# This calls 'make' inside each subdirectory
$(SUBDIRS):
	$(MAKE) -C $@

# Clean target: loops through each subdir and runs 'make clean'
clean:
	@for dir in $(SUBDIRS); do \
		if [ -d $$dir ]; then \
			$(MAKE) -C $$dir clean || true; \
		fi \
	done

.PHONY: all clean $(SUBDIRS)
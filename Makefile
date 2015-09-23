all:
	$(MAKE) $(LINUX_MAKE_FLAGS) -C $(LINUX_DIR) M=$(CURDIR) modules

install:
	$(MAKE) $(LINUX_MAKE_FLAGS) -C $(LINUX_DIR) M=$(CURDIR) modules_install

clean:
	$(MAKE) $(LINUX_MAKE_FLAGS) -C $(LINUX_DIR) M=$(CURDIR) clean

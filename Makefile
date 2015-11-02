CONFIG_MODULE_SIG=n

all:
	$(MAKE) $(LINUX_MAKE_FLAGS) -C $(LINUX_DIR) M=$(CURDIR) modules

install:
	$(MAKE) $(LINUX_MAKE_FLAGS) -C $(LINUX_DIR) M=$(CURDIR) modules_install

clean:
	$(MAKE) $(LINUX_MAKE_FLAGS) -C $(LINUX_DIR) M=$(CURDIR) clean

tags:
	ctags telit_spi.c telit_spi.h


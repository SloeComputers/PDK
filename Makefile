
targets = rp2040 rp2350 LPC1114 LPC11U24 LPC1768 LPC810 nRF51 nRF52 native # ATtiny85 ATtiny84

all: $(targets)

.PHONY: template
template:
	@scripts/INSTALL_TEMPLATE.py

.PHONY: docs
docs:
	doxygen scripts/Doxyfile

include build.make

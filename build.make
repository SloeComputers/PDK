#-------------------------------------------------------------------------------
# Copyright (c) 2021 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

.PHONY: clean debug spotless test

clean:
	rm -rf build

debug:
	PDK/scripts/build.py -s -d native

spotless:
	PDK/scripts/build.py -s $(targets)

test:
	PDK/scripts/build.py --test native

#-------------------------------------------------------------------------------
# Build rules for PDK target platforms

build_targets := \
   native vst3 \
   LPC810 LPC1114 LPC11U24 LPC1768 \
   nRF51 nRF52 \
   rp2040 rp2350 tiny2350 \
   ATtiny84 ATtiny85

.PHONY: $(build_targets)

$(build_targets):
	PDK/scripts/build.py $@

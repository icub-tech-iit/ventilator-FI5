
EMWI_GFX_LIB = ewgfx-m7

ifeq ($(EMWI_SURFACE_ROTATION),90)
  EMWI_GFX_LIB := $(EMWI_GFX_LIB)-r90
else ifeq ($(EMWI_SURFACE_ROTATION),180)
  EMWI_GFX_LIB := $(EMWI_GFX_LIB)-r180
else ifeq ($(EMWI_SURFACE_ROTATION),270)
  EMWI_GFX_LIB := $(EMWI_GFX_LIB)-r270
endif

EMWI_GFX_LIB := $(EMWI_GFX_LIB)-gcc

EMWI_GFX_FILES :=                                                              \
                    ewextgfx.c                                                 \


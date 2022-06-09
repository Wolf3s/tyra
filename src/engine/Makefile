TYRA_DIR = ./

BIN2S = $(PS2SDK)/bin/bin2s

IOP_MODULES = 	libsd.o \
				usbd.o \
				usbhdfsd.o \
				audsrv.o

EE_OBJS =								\
	models/math/matrix.o				\
	models/math/plane.o					\
	models/math/point.o					\
	models/math/vector3.o				\
	models/math/ray.o					\
	models/mesh_frame.o					\
	models/bounding_box.o				\
	models/mesh_material.o				\
	models/mesh.o						\
	models/sprite.o						\
	models/texture.o					\
	modules/audio.o						\
	modules/camera_base.o				\
	modules/file_service.o				\
	modules/file_manager.o				\
	modules/gif_sender.o				\
	modules/light.o						\
	modules/pad.o						\
	modules/renderer.o					\
	modules/texture_repository.o		\
	modules/timer.o						\
	modules/vif_sender.o				\
	utils/math.o						\
	utils/string.o						\
	loaders/bmp_loader.o				\
	loaders/dff_loader.o				\
	loaders/mdl_loader.o				\
	loaders/obj_loader.o				\
	loaders/png_loader.o				\
	vu1_progs/draw3D.o					\
	engine.o

all: $(IOP_MODULES) $(EE_OBJS) 
	ar rcs $(LIB_NAME) $(EE_OBJS)
	rm -f $(EE_OBJS)
	rm -f $(IOP_MODULES)
	rm -f *.s
	echo "Success!"

# %.vcl: %.vclpp
#	 $(EE_VCLPP) $< $@

# %.vsm: %.vcl
#	$(EE_VCL) $< >> $@

%.o: %.vsm
	$(EE_DVP) $< -o $@

clean:
	rm -f $(EE_OBJS)
	rm -f $(LIB_NAME)
	rm -f *.s 

libsd.s: $(PS2SDK)/iop/irx/libsd.irx
	echo "Embedding LIBDS..."
	$(BIN2S) $< $@ libsd_irx

usbd.s: $(PS2SDK)/iop/irx/usbd.irx
	echo "Embedding USB Driver..."
	$(BIN2S) $< $@ usbd_irx

audsrv.s: $(PS2SDK)/iop/irx/audsrv.irx
	echo "Embedding AUDSRV Driver..."
	$(BIN2S) $< $@ audsrv_irx

usbhdfsd.s: $(PS2SDK)/iop/irx/usbhdfsd.irx
	echo "Embedding USBHDFSD Driver..."
	$(BIN2S) $< $@ usbhdfsd_irx

include Makefile.pref
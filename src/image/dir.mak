SUBDIRS=

SRC= MLImage.C MLImageFormat.C MLImageReader.C MLImageWriter.C \
bmp/MLImageReaderBMP.C bmp/MLImageFactoryBMP.C \
gif/MLImageReaderGIF.C gif/MLImageFactoryGIF.C \
jpg/MLImageReaderJPG.C jpg/MLImageWriterJPG.C jpg/MLImageFactoryJPG.C \
pnm/MLImageReaderPNM.C pnm/MLImageWriterPNM.C pnm/MLImageFactoryPNM.C \
rgb/MLImageReaderRGB.C rgb/MLImageFactoryRGB.C \
tga/MLImageReaderTGA.C tga/MLImageFactoryTGA.C \
tif/MLImageReaderTIF.C tif/MLImageWriterTIF.C tif/MLImageFactoryTIF.C

image_sources=$(SRC)
image_dlibs=jpeg tiff
image_slibs=
image_install=$(prefix)/lib/moonlight

SHARED=image

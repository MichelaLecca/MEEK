# MEEK
iMage EnhancEment Kit

This is the implementation of MEEK, a basic tool for enhancing images acquired undeer low-light, backligth and spotlight. MEEK has been tested as an enhancer of archaeological images, which depict places or objects that are difficult to be well illuminated, such as excavation, crypts, mosaics and paintings.

MEEK embeds three algorithms for image enhancement:
1) SuPeR, a Retinex inspired algorithm for improving non uniform low-light images, also mitigiating/removing possible color catsts due to the illuminations;
2) REK, a relighting method based on the von Kries model and specifically designed for the enhancement of images with strong backlight and spotlight;
3) CLAHE, a popular algorithm for enhancing the image contrast by redistributing the image luminance.



extern void vncResizeServer(int num);
extern Bool vncRandRInit (ScreenPtr pScreen);

typedef struct
{
  int scrnum;
  int randrSelected; 
  int width;           /* current value */
  int maxWidth;
  int paddedWidth;
  int paddedWidthInBytes;
  int height;          /* current value */
  int maxHeight;
  int depth;
  int bitsPerPixel;
  int sizeInBytes;
  int ncolors;
  char *pfbMemory;
  XWDColor *pXWDCmap;
  XWDFileHeader *pXWDHeader;
  Pixel blackPixel;
  Pixel whitePixel;
  unsigned int lineBias;
  Bool pixelFormatDefined;
  Bool rgbNotBgr;
  int redBits, greenBits, blueBits;
} vfbScreenInfo;

typedef vfbScreenInfo *vfbScreenInfoPtr;
extern vfbScreenInfo *vfbFirstScreen(void);





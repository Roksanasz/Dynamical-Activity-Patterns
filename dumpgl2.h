#define FALSE 0
#define TRUE 1

 #define RGB3 3                  // 3 bytes of color info per pixel
 #define RGBA 4                  // 4 bytes of color+alpha info

 bool sg_glWritePPMFile(const char *filename, GLubyte *buffer, int win_width, int win_height, int mode)
 {
     int i, j, k, q;
     unsigned char *ibuffer;
	 FILE *fp;
     int pixelSize = mode==GL_RGBA?4:3;

     ibuffer = (unsigned char *) malloc(win_width*win_height*RGB3);




     if ( (fp = fopen(filename, "wb")) == NULL ) {
         printf("Warning: cannot open %s\n", filename);
         return false;
     }

     fprintf(fp, "P6\n# CREATOR: glReadPixel()\n%d %d\n%d\n",
             win_width, win_height, 255);
     q = 0;
     for (i = 0; i < win_height; i++)
         for (j = 0; j < win_width; j++)
             for (k = 0; k < RGB3; k++)
                 ibuffer[q++] = (unsigned char)
                     *(buffer + (pixelSize*((win_height-1-i)*win_width+j)+k));
     fwrite(ibuffer, sizeof(unsigned char), RGB3*win_width*win_height, fp);
     fclose(fp);
     free(ibuffer);

     printf("wrote file (%d x %d pixels, %d bytes)\n",
            win_width, win_height, RGB3*win_width*win_height);
     return true;
 }


 // dump the screen buffer to a ppm file
 bool sg_glDumpWindow(int win_width, int win_height) {
     GLubyte *buffer;
     bool result;
 	 static int counter = 0;
     char fname[32];

     buffer = (GLubyte *) malloc(win_width*win_height*RGBA);

     // read window contents from color buffer with glReadPixels
     glFinish();
     glReadPixels(0, 0, win_width, win_height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);


     printf("(frame %d)...",counter);
	 sprintf(fname,"frame%04d.ppm",counter++);


	 //result = sg_glWritePPMFile(fname, buffer, win_width, win_height,GL_RGBA );
     free(buffer);

     return result;
 }

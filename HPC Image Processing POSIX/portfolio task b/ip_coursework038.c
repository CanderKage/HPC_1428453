#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <malloc.h>
#include <signal.h>

/******************************************************************************
  Displays two grey scale images. On the left is an image that has come from an 
  image processing pipeline, just after colour thresholding. On the right is 
  the result of applying an edge detection convolution operator to the left
  image. This program performs that convolution.
   
  Things to note:
    - A single unsigned char stores a pixel intensity value. 0 is black, 256 is
      white.
    - The colour mode used is GL_LUMINANCE. This uses a single number to 
      represent a pixel's intensity. In this case we want 256 shades of grey,
      which is best stored in eight bits, so GL_UNSIGNED_BYTE is specified as 
      the pixel data type.
    
  To compile adapt the code below wo match your filenames:  
    "cc -o ip_coursework ip_coursework38_a.c -lglut -lGL -lm -lrt "where 
lrt for accuracy of time
   
  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
#define width 100 
#define height 72

unsigned char image[], results[width * height];

void detect_edges(unsigned char *in, unsigned char *out) {
  int i;
  int n_pixels = width * height;

  for(i=0;i<n_pixels;i++) {
    int x, y; // the pixel of interest
    int b, d, f, h; // the pixels adjacent to x,y used for the calculation
    int r; // the result of calculate
    
    y = i / width; 
    x = i - (width * y);

    if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
      results[i] = 0; 
    } else {
      b = i + width;
      d = i - 1;
      f = i + 1;
      h = i - width;

      r = (in[i] * 4) + (in[b] * -4) + (in[d] * -1) + (in[f] * -1)
          + (in[h] * -4);

      if (r > 0) { // if the result is positive this is an edge pixel
        out[i] = 255;
      } else {
        out[i] = 0;
      }
    }
  }
}

void tidy_and_exit() {
  exit(0);
}

void sigint_callback(int signal_number){
  printf("\nInterrupt from keyboard\n");
  tidy_and_exit();
}

static void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glRasterPos4i(-1, -1, 0, 1);
  glDrawPixels(width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image);
  glRasterPos4i(0, -1, 0, 1);
  glDrawPixels(width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, results);
  glFlush();
}

static void key_pressed(unsigned char key, int x, int y) {
  switch(key){
    case 27: // escape
      tidy_and_exit();
      break;
    default: 
      printf("\nPress escape to exit\n");
      break;
  }
}
int time_difference(struct timespec *start, struct timespec *finish, 
                    long long int *difference) { //time difference calculation
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}


int main(int argc, char **argv) {
  signal(SIGINT, sigint_callback);
 
  printf("image dimensions %dx%d\n", width, height); //prints the image height width
  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);   //timer starts
  detect_edges(image, results);  // carrying time function after the images is loaded


  clock_gettime(CLOCK_MONOTONIC, &finish); //timer ends after the image function of detecting edges loaded
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9));    //time interval output

 
  glutInit(&argc, argv);  //initialize graphics 
  glutInitWindowSize(width * 2,height);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_LUMINANCE);
      
  glutCreateWindow("6CS005 Image Processing Coursework");
  glutDisplayFunc(display);
  glutKeyboardFunc(key_pressed);
  glClearColor(0.0, 1.0, 0.0, 1.0); 

  glutMainLoop(); 

  tidy_and_exit();
  
  return 0;
}

unsigned char image[] = {
  
};

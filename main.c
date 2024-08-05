#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct {
   unsigned int left_clicks;
   unsigned int right_clicks;
   unsigned int middle_clicks;
   unsigned int pixels_travelled;
} MouseMetrics;

int abs( int x ){
  return x < 0 ? -x : x;
}


MouseMetrics update_metrics( char event[3], MouseMetrics metrics){
  int left, right, middle;
  signed char x, y;
  metrics.left_clicks += event[0] & 0x1;
  metrics.right_clicks += event[0] & 0x2;
  metrics.middle_clicks += event[0] & 0x4;
  metrics.pixels_travelled += abs( event[1] ) + abs(event[2]);
  return metrics;
}

void print_event( char data[3] ){
  int left, right, middle;
  signed char x, y;
  left = data[0] & 0x1;
  right = data[0] & 0x2;
  middle = data[0] & 0x4;

  x = data[1];
  y = data[2];
  printf("x=%d, y=%d, left=%d, middle=%d, right=%d\n", x, y, left, middle, right);
}

int open_file(char* filepath){
  int fid = open(filepath, O_RDONLY);
  if( fid == 0 ){
    fprintf(stderr, "Cannot open %s", filepath);
    return EXIT_FAILURE;
  }
  fprintf(stdout, "%s opened", filepath);
  return fid;
}

MouseMetrics read_file(int fid, MouseMetrics metrics){
  int bytes;
  char data[3];
  bytes = read( fid, data, sizeof(data) );
  if( 0 < bytes){
    print_event(data);
    return update_metrics(data, metrics);
  }
  return metrics;
}

void close_file( int fid ){
  close(fid);
}

void show_metrics( MouseMetrics metrics ){
  printf("Veces que tocaste click: \n\tizquierdo: %d\n\tderecho: %d\n\tmedio: %d\n Pixeles Navegados: %d", metrics.left_clicks, metrics.right_clicks, metrics.middle_clicks, metrics.pixels_travelled);
}
int main(){
  int fid = open_file("/dev/input/mice");
  MouseMetrics metrics = (MouseMetrics){0, 0, 0, 0};
  while( 1 ){
    metrics = read_file(fid, metrics);
  }
  close_file(fid);
  return 0;
}

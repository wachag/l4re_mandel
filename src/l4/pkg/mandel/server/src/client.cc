#include "shared.hh"
#include <l4/re/env>
#include <l4/re/util/cap_alloc>
#include <l4/sys/types.h>
#include <iostream>

int w=0;
int h=0;
int mandelbrot(int, int);
int mandelbrot(int ia, int ib){
  double ca=-2.0+ia*(4.0/w);
  double cb=-2.0+ib*(4.0/h);
  double za=0;
  double zb=0;
  int iter=0;
  for(iter=0;iter<100;iter++){
    double za1=za*za-zb*zb+ca;
    double zb1=2*za*zb+cb;
    za=za1;
    zb=zb1;
    if(za*za+zb*zb>4)return iter;
  }
  return iter;
}

int main(int argc, char **argv)
{
  auto env=L4Re::Env::env();
  auto server=env->get_cap<Mandel>("mandelserver");
  if(!server.is_valid()){
    return 1;
  }
  server->getScreen(&w,&h);
  while(true){
    int has;
    server->hasPoint(&has);
    if(!has)return 0;
    int a,b,n;
    a=0;
    b=0;
    n=0;
    server->getPoint(&a,&b,&n);
    for(int i=0;i<n;i++){
      server->setPoint(a,b,mandelbrot(a,b));
      a++;
      if(a==w){
        a=0; b++;
      }
    }
  }
  return 0;
}

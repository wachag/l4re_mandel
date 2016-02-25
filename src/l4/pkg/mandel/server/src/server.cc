#include "shared.hh"
#include <l4/re/env>
#include <l4/re/util/cap_alloc>
#include <l4/re/util/object_registry>
#include <l4/re/util/br_manager>
#include <l4/sys/cxx/ipc_epiface>
#include <l4/re/util/video/goos_fb>
#include <l4/sys/semaphore>
#include <l4/re/util/event>
#include <l4/event/event>
#include <l4/re/event_enums.h>
#include <l4/util/keymap.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

static L4Re::Util::Video::Goos_fb gfb;
static L4Re::Video::View::Info fbi;
static L4Re::Util::Event event;

int w=100;
int h=100;

static void *fbmem_vaddr;
static unsigned bpp;

static void put_pixel(int x, int y, int fullval)
{
  unsigned v = 0;
  unsigned long offset = (unsigned long)fbmem_vaddr + y * fbi.bytes_per_line + x * fbi.pixel_info.bytes_per_pixel();
  v  = ((fullval >> (8  - fbi.pixel_info.r().size())) & ((1 << fbi.pixel_info.r().size()) - 1)) << fbi.pixel_info.r().shift();
  v |= ((fullval >> (16 - fbi.pixel_info.g().size())) & ((1 << fbi.pixel_info.g().size()) - 1)) << fbi.pixel_info.g().shift();
  v |= ((fullval >> (24 - fbi.pixel_info.b().size())) & ((1 << fbi.pixel_info.b().size()) - 1)) << fbi.pixel_info.b().shift();
   switch (bpp)
    {
    case 8: *(unsigned char  *)offset = v; break;
    case 14: case 15: case 16: *(unsigned short *)offset = v; break;
    case 24: case 32: *(unsigned int   *)offset = v; break;
    default:
      printf("unhandled bitsperpixel %d\n", bpp);
    };
}


static void update_rect(int x, int y, int w, int h)
{
  gfb.refresh(x, y, w, h);
}

const int maxn=100;

static L4Re::Util::Registry_server<L4Re::Util::Br_manager_hooks> server;
class MandelServer: public L4::Epiface_t<MandelServer, Mandel> {
  unsigned int x;
  unsigned int y;
  public:
    MandelServer():x(0),y(0){}
    int op_getPoint(Mandel::Rights, int & a, int & b, int & n){
      int i;
      n=0;
      a=x;
      b=y;
      for(i=0;i<maxn;i++){
        x++;
        if(x==w){
          x=0; y++;
        }
        if(y>=h)n=i;
      }
      n=i;
      return 0;
    }

    int op_hasPoint(Mandel::Rights, int & res){
      res= !(y>=h);
      return 0;
    }

    int op_getScreen(Mandel::Rights, int & wi, int & he){
      wi=w;
      he=h;
      return 0;
    }

    int op_setPoint(Mandel::Rights, int a, int b, int value){
      unsigned r = value;
      unsigned g = 255-value;
      unsigned bl = value/2;

      put_pixel(a, b, (r << 0) | (g << 8) | (bl << 16));
      update_rect(a, b, 1, 1);
      return 0;
    }
};

static inline unsigned color_val(unsigned w, unsigned peak_point, unsigned val)
{
  unsigned third = w / 3;

  unsigned a = abs(val - peak_point);
  if (a > third * 2)
    a = peak_point + w - val;
  if (a > third)
    return 0;

  return ((third - a) * 255) / third;
}

int main(int argc, char **argv)
{
  try { gfb.setup("fb"); } catch (...) { return 1; }
  if (gfb.view_info(&fbi))
    return 2;
  w = fbi.width;
  h = fbi.height;

  if (!(fbmem_vaddr = gfb.attach_buffer()))
    return 3;

  bpp = fbi.pixel_info.bits_per_pixel();

  static MandelServer mandel;
  server.registry()->register_obj(&mandel,"mandelserver");
  server.loop();
  return 0;
}

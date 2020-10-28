#include "kernel/ocl/common.cl"

__kernel void sable_ocl (__global unsigned *in, __global unsigned *out, __global int *diff, __global unsigned *read, __global unsigned *write)
{
  
  __local int change;
  change = 0;
  int x = get_global_id(0);
  int y = get_global_id(1);
  int localx = x/(DIM/TILEX);
  int localy = y/(DIM/TILEY);
  
  if(read[localy*(DIM/TILEY) + localx] != 1){
    int s = in[y*DIM+x] & 3;
    s += (y>0) ? in[(y-1)*DIM+x] >> 2 :0;
    s += (y<DIM-1) ? in[(y+1)*DIM+x] >> 2 :0;
    s += (x<DIM-1) ? in[y*DIM+x+1] >> 2 :0;
    s += (x>0) ? in[y*DIM+x-1] >> 2 :0;

    if(in[y*DIM+x] != s){
      diff[0] +=1;
      change+=1;
    }

    barrier (CLK_LOCAL_MEM_FENCE);
    
    if(change!=0){
      write[localy*(DIM/TILEY) + localx] = 0;
      write[(localy-1)*(DIM/TILEY) + localx] = 0;
      write[(localy+1)*(DIM/TILEY) + localx] = 0;
      write[localy*(DIM/TILEY) + localx-1] = 0;
      write[localy*(DIM/TILEY) + localx+1] = 0;
    }
    read[localy*(DIM/TILEY) + localx] = 1;
    out[y*DIM+x] = s;
  }else{
    out[y*DIM+x] = in[y*DIM+x];
  }

}


// DO NOT MODIFY: this kernel updates the OpenGL texture buffer
// This is a sable-specific version (generic version is defined in common.cl)
__kernel void sable_update_texture (__global unsigned *cur, __write_only image2d_t tex)
{
  int y = get_global_id (1);
  int x = get_global_id (0);
  int2 pos = (int2)(x, y);
  unsigned c = cur [y * DIM + x];
  unsigned r = 0, v = 0, b = 0;

  if (c == 1)
    v = 255;
  else if (c == 2)
    b = 255;
  else if (c == 3)
    r = 255;
  else if (c == 4)
    r = v = b = 255;
  else if (c > 4)
    r = v = b = (2 * c);

  c = rgba(r, v, b, 0xFF);

  write_imagef (tex, pos, color_scatter (c));
}

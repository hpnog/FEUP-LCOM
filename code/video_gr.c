#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "video_gr.h"


/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */


/* Private global variables */

//static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

unsigned getHres() { return h_res; }

unsigned getVres() { return v_res; }

void *vg_init(unsigned short mode){
	struct reg86u r;
	int a;

	phys_bytes buf; /* to store the VBE Mode Info desired */

	r.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	r.u.w.bx = 1<<14|mode; // set bit 14: linear framebuffer
	r.u.b.intno = 0x10;

	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return;
	}

	vbe_mode_info_t vbeInfo;
	vbe_get_mode_info(mode, &vbeInfo);

	//------------------------------
	h_res = vbeInfo.XResolution;
	v_res = vbeInfo.YResolution;
	bits_per_pixel = vbeInfo.BitsPerPixel;
	//------------------------------

	/* Map memory */

	struct mem_range vm;
	vm.mr_base = vbeInfo.PhysBasePtr;
	vm.mr_limit = vm.mr_base + h_res*v_res*(bits_per_pixel/8);

	if( OK != (a = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &vm)))
		panic("video: sys_privctl (ADD_MEM) failed: %d\n", a);

	video_mem = vm_map_phys(SELF, (void *)vm.mr_base, h_res*v_res*(bits_per_pixel/8));

	if(video_mem == MAP_FAILED)
		panic("video couldn't map video memory");

	return video_mem;

}

int vg_exit(void) {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}

void colorPix(unsigned int x, unsigned int y, char color) {

	char *ptr = video_mem;
	ptr = ptr + y*bits_per_pixel/8 * h_res + x*bits_per_pixel/8;
	*ptr = color;

}

void * getAddr() { return video_mem; }

int checkScreen(unsigned short xi, unsigned short xf, unsigned short yi, unsigned short yf) {
	if (xi >= 0)
		if (yi >= 0)
			if (xf <= h_res)
				if (yf <= v_res)
					return 0;
	return 1;
}

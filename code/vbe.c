#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	mmap_t map_vbe;

	if(lm_alloc(sizeof(vbe_mode_info_t), &map_vbe) == NULL)
		return 1;

	struct reg86u reg;
	reg.u.w.ax = 0x4F01; 						//function 01 - returns vbe mode
	reg.u.w.cx = mode;
	reg.u.w.es = PB2BASE(map_vbe.phys);
	reg.u.w.di = PB2OFF(map_vbe.phys);
	reg.u.b.intno = 0x10;

	if( sys_int86(&reg) != OK )
		printf("\nFailed to set_vbe_mode: sys_int86() \n\n");

	*vmi_p = *(vbe_mode_info_t*)map_vbe.virtual;

	lm_free(&map_vbe);

	return 0;

}

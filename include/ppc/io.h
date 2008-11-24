#ifndef _ASM_IO_H
#define _ASM_IO_H

#include "asm/types.h"

extern unsigned long virt_offset;

#define phys_to_virt(phys) ((void *) ((unsigned long) (phys) - virt_offset))
#define virt_to_phys(virt) ((unsigned long) (virt) + virt_offset)

#ifndef BOOTSTRAP

extern uint32_t isa_io_base;

/*
 * The insw/outsw/insl/outsl macros don't do byte-swapping.
 * They are only used in practice for transferring buffers which
 * are arrays of bytes, and byte-swapping is not appropriate in
 * that case.  - paulus
 */
#define insw(port, buf, ns)	_insw((uint16_t *)((port)+isa_io_base), (buf), (ns))
#define outsw(port, buf, ns)	_outsw((uint16_t *)((port)+isa_io_base), (buf), (ns))

#define inb(port)		in_8((uint8_t *)((port)+isa_io_base))
#define outb(val, port)		out_8((uint8_t *)((port)+isa_io_base), (val))
#define inw(port)		in_le16((uint16_t *)((port)+isa_io_base))
#define outw(val, port)		out_le16((uint16_t *)((port)+isa_io_base), (val))
#define inl(port)		in_le32((uint32_t *)((port)+isa_io_base))
#define outl(val, port)		out_le32((uint32_t *)((port)+isa_io_base), (val))

/*
 * 8, 16 and 32 bit, big and little endian I/O operations, with barrier.
 */
static inline int in_8(volatile unsigned char *addr)
{
	int ret;

	__asm__ __volatile__("lbz%U1%X1 %0,%1; eieio":"=r"(ret):"m"(*addr));
	return ret;
}

static inline void out_8(volatile unsigned char *addr, int val)
{
	__asm__ __volatile__("stb%U0%X0 %1,%0; eieio":"=m"(*addr):"r"(val));
}

static inline int in_le16(volatile unsigned short *addr)
{
	int ret;

	__asm__ __volatile__("lhbrx %0,0,%1; eieio":"=r"(ret):
			     "r"(addr), "m"(*addr));
	return ret;
}

static inline int in_be16(volatile unsigned short *addr)
{
	int ret;

	__asm__ __volatile__("lhz%U1%X1 %0,%1; eieio":"=r"(ret):"m"(*addr));
	return ret;
}

static inline void out_le16(volatile unsigned short *addr, int val)
{
	__asm__ __volatile__("sthbrx %1,0,%2; eieio":"=m"(*addr):"r"(val),
			     "r"(addr));
}

static inline void out_be16(volatile unsigned short *addr, int val)
{
	__asm__ __volatile__("sth%U0%X0 %1,%0; eieio":"=m"(*addr):"r"(val));
}

static inline unsigned in_le32(volatile unsigned *addr)
{
	unsigned ret;

	__asm__ __volatile__("lwbrx %0,0,%1; eieio":"=r"(ret):
			     "r"(addr), "m"(*addr));
	return ret;
}

static inline unsigned in_be32(volatile unsigned *addr)
{
	unsigned ret;

	__asm__ __volatile__("lwz%U1%X1 %0,%1; eieio":"=r"(ret):"m"(*addr));
	return ret;
}

static inline void out_le32(volatile unsigned *addr, int val)
{
	__asm__ __volatile__("stwbrx %1,0,%2; eieio":"=m"(*addr):"r"(val),
			     "r"(addr));
}

static inline void out_be32(volatile unsigned *addr, int val)
{
	__asm__ __volatile__("stw%U0%X0 %1,%0; eieio":"=m"(*addr):"r"(val));
}

static inline void _insw_ns(volatile uint16_t * port, void *buf, int ns)
{
	uint16_t *b = (uint16_t *) buf;

	while (ns > 0) {
		*b++ = in_le16(port);
		ns--;
	}
}

static inline void _outsw_ns(volatile uint16_t * port, const void *buf,
			     int ns)
{
	uint16_t *b = (uint16_t *) buf;

	while (ns > 0) {
		out_le16(port, *b++);
		ns--;
	}
}

static inline void _insw(volatile uint16_t * port, void *buf, int ns)
{
	uint16_t *b = (uint16_t *) buf;

	while (ns > 0) {
		*b++ = in_be16(port);
		ns--;
	}
}

static inline void _outsw(volatile uint16_t * port, const void *buf,
			  int ns)
{
	uint16_t *b = (uint16_t *) buf;

	while (ns > 0) {
		out_be16(port, *b++);
		ns--;
	}
}
#endif

#endif /* _ASM_IO_H */

#ifndef __CLTR__
#define __CLTR__

#define CTRL_(C) (0x1f&(C))
#define ASCII_CONTROL_C CTRL_('c')
#define ASCII_CONTROL_D CTRL_('d')
#define ASCII_CONTROL_A CTRL('a')
#define ASCII_CONTROL_E CTRL('e')
#define ASCII_CONTROL_K CTRL('k')

#define ASCII_CONTROL 0x1b
#define ASCII_BACKSPACE 0x08
#define ASCII_DELETE 0x7f
#define ASCII_ENTER 0x0a
#define ASCII_TAB 0x09
#define NOP 0x00

#endif

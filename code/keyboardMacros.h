#ifndef __KEYBOARDMACROS_H
#define __KEYBOARDMACROS_H

/** @defgroup keyboardMacros keyboardMacros
 * @{
 *
 * Constants for programming the keyboard.
 */

#define BIT(n) (0x01<<(n))

#define DELAY_US		20000

#define QQ				3
#define WAIT_KBC		1000

#define KBD_IRQ	        1		    /**< @brief KEYBOARD IRQ line */
#define KBD_ESC_REL 	0x81		/**< @brief KEYBOARD Escape break code */
#define SPACE_REL		0xb9		/**< @brief KEYBOARD Space break code */
#define ENTER_REL		0x9c		/**< @brief KEYBOARD Enter break code */
#define UP_ARROW_REL	0xc8		/**< @brief KEYBOARD Up arrow break code */
#define DOWN_ARROW_REL  0xd0		/**< @brief KEYBOARD Down arrow break code */
#define LEFT_ARROW_REL  0xcb		/**< @brief KEYBOARD left arrow break code */
#define RIGHT_ARROW_REL 0xcd		/**< @brief KEYBOARD right arrow break code */
#define OUT_BUF 		0x60		/**< @brief KEYBOARD Out buffer */
#define STAT_REG		0x64		/**< @brief KEYBOARD Status register */
#define LED_SWITCHES	0xED
#define RESEND			0xFE
#define ERROR			0xFC
#define ACK				0xFA
#define KBC_CMD_REG		0x64
#define WORD			0xE0

#define IBF				BIT(1)
#define OBF				BIT(0)
#define PAR_ERR			BIT(7)
#define TO_ERR			BIT(5)
#define LEDS_ON			BIT(0)|BIT(1)|BIT(2)

/**@}*/

#endif
